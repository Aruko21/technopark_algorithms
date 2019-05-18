/*
	Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
	Хранимые строки непустые и состоят из строчных латинских букв.
	Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
	Начальный размер таблицы должен быть равным 8-ми. 
	Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
	Структура данных должна поддерживать операции добавления строки в множество, 
	удаления строки из множества и проверки принадлежности данной строки множеству.
	Для разрешения коллизий используйте квадратичное пробирование. 
	i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
	https://en.wikipedia.org/wiki/Quadratic_probing
*/

#include <vector>
#include <string>
#include <iostream>
#include <exception>

class HashTableException : public std::exception
{
	public:
		HashTableException(const std::string& mes) : message_(mes) {}
		std::string what();
	private:
		std::string message_;
};

class HashTable {
	public:
		HashTable();
		HashTable(const HashTable&) = delete;
		HashTable(const HashTable&&) = delete;
		HashTable& operator=(const HashTable&) = delete;
		HashTable&& operator=(const HashTable&&) = delete;

		bool Add(const std::string& line);
		bool Delete(const std::string& line);
		bool Has(const std::string& line);

		void Print();

	private:
		size_t Hash(const std::string& line);
		size_t QuadProbe(const size_t& hash, const size_t& iteration);
		void Rehash();

		std::vector<std::pair<bool, std::string>> data_;
		const static size_t start_size_ = 8;
		size_t max_to_fill_;
		size_t elems_num_;
};

std::string HashTableException::what() {
	return message_;
}

HashTable::HashTable() : data_(start_size_), elems_num_(0), max_to_fill_(static_cast<size_t>(start_size_ * 0.75)) {
}

size_t HashTable::Hash(const std::string& line) {
	if(data_.size() == 0) {
		return 0;
	}
	size_t hash = 0;
	const size_t a_const = 31;		// (a = 2^i - 1) >= N, N - мощность алфавита
	for(size_t i = 0; i < line.length(); ++i) {
		hash = (hash * a_const + line[i]) % data_.size();
	}
	return hash;
}

size_t HashTable::QuadProbe(const size_t& prev, const size_t& iteration) {
	return (prev + iteration * iteration * ((iteration % 2 == 0) ? -1 : 1)) % data_.size();
	// return (prev + iteration) % data_.size();
}

void HashTable::Rehash() {
	std::vector<std::pair<bool, std::string>> temp_data = data_;
	data_.clear();
	data_.resize(temp_data.size() * 2);
	max_to_fill_ = static_cast<size_t>(data_.size() * 0.75);
	elems_num_ = 0;
	for(size_t i = 0; i < temp_data.size(); ++i) {
		if(!temp_data[i].second.empty() && !temp_data[i].first) {
			bool exit_stat = false;
			size_t iteration = 0;
			for(size_t j = Hash(temp_data[i].second); iteration < data_.size() 
					&& !exit_stat; j = QuadProbe(j, ++iteration)) {
				if(data_[j].second.empty()) {
					data_[j].second = temp_data[i].second;
					++elems_num_;
					exit_stat = true;
				}
			}
		}
	}
}

bool HashTable::Add(const std::string& line) {
	bool has_line = false;
	bool exit_stat = false;
	size_t iteration = 0;
	if(elems_num_ >= max_to_fill_){
		Rehash();
	}
	if(Find(line)) {
		return false;
	}
	for(size_t j = Hash(line); iteration < data_.size() 
			&& !exit_stat; j = QuadProbe(j, ++iteration)) {
		if(data_[j].second.empty()) {
			data_[j].first = false;
			data_[j].second = line;
			++elems_num_;
			exit_stat = true;
		} else {
			if(data_[j].first) {
				data_[j].first = false;
				data_[j].second.clear();
				data_[j].second = line;
				++elems_num_;
				exit_stat = true;
			} else {
				if(data_[j].second == line) {
					has_line = true;
					exit_stat = true;
				}
			}
		}
		/*if(!data_[j].second.empty()) {
			if(data_[j].first) {
				data_[j].first = false;
				data_[j].second.clear();
				data_[j].second = line;
				++elems_num_;
				exit_stat = true;
			} else {
				if(data_[j].second == line) {
					has_line = true;
					exit_stat = true;
				}
			}
		} else {
			data_[j].first = false;
			data_[j].second = line;
			++elems_num_;
			exit_stat = true;
		}*/
	}
	if (!exit_stat) {
		throw HashTableException("Overflow");
	}
	return !has_line;
}

bool HashTable::Delete(const std::string& line) {
	bool has_line = false;
	bool exit_stat = false;
	size_t iteration = 0;
	for (size_t j = Hash(line); iteration < data_.size() 
			&& !exit_stat; j = QuadProbe(j, ++iteration)) {
		if(!data_[j].first) {
			if(!data_[j].second.empty()) {
				if(data_[j].second == line) {
					data_[j].first = true;
					--elems_num_;
					has_line = true;
					exit_stat = true;
				}
			} else {
				exit_stat = true;
			}
		}
	}
	return has_line;
}

bool HashTable::Has(const std::string& line) {
	bool has_line = false;
	bool exit_stat = false;
	size_t iteration = 0;
	for (size_t j = Hash(line); iteration < data_.size() 
			&& !exit_stat; j = QuadProbe(j, ++iteration)) {
			if(!data_[j].first) {
				if(!data_[j].second.empty()) {
					if(data_[j].second == line) {
						has_line = true;
						exit_stat = true;
					}
				} else {
					exit_stat = true;
				}
			}
	}
	return has_line;
}


void HashTable::Print() {
	for(auto it = data_.begin(); it != data_.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}
	std::cout << "end ------------" << std::endl;
}

int main(void) {
	HashTable table;
	while(std::cin.good()) {
		char command = '\0';
		std::cin >> command;
		std::string line;
		std::cin >> line;
		if(!line.empty()) {
			switch(command) {
				case '+':
					try {
							std::cout << (table.Add(line) ? "OK" : "FAIL") << std::endl;
					}
					catch (HashTableException& e) {
						std::cout << e.what() << std::endl;
					 	std::cin.setstate(std::ios::failbit);
					}
					break;
				case '-':
					std::cout << (table.Delete(line) ? "OK" : "FAIL") << std::endl;
					break;
				case '?':
					std::cout << (table.Has(line) ? "OK" : "FAIL") << std::endl;
					break;
				default:
					if(!std::cin.eof()) {
						std::cerr << "smth wrong" << std::endl;
					}
						std::cin.setstate(std::ios::failbit);
					break;
			}
		}	
	}
	return 0;
}
