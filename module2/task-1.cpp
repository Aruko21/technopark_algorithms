// Косенков Александр АПО-12
// Алгоритмы и структуры данных - 2-й модуль
// Задача 1_1
// Реализовать структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
// и проверки принадлежности данной строки множеству.
//
// Для разрешения коллизий использовать квадратичное пробирование. i-ая проба
// g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

#include <iostream>
#include <cassert>
#include <vector>
#include <string>

// Размер таблицы - степень двойки
const int DefaultHashTableSize = 8;
const double MaxAlpha = 3./4;
const double MaxDeleted = 1./2;
const int GrowFactor = 2;

int Nod(int a, int b) {
    while (a != b) {
        if (a > b) {
            std::swap(a, b);
        }
        b = b - a;
    }
    return a;
}

int checkSimple(const int constant, size_t size) {
    int resultConst = constant;
    while (Nod(resultConst, size) != 1) {
        resultConst++;
    }
    return resultConst;
}
template <class T>
int Hash(const T& key, size_t tableSize) {
//    std::cout << "wasuuuup" << std::endl;
    return 0;
}
template <>
int Hash<std::string>(const std::string& key, size_t tableSize) {
    int hash = 0;
    static int a = 127;
    a = checkSimple(a, tableSize);
    for (int i = 0; i < key.size(); ++i) {
        hash = (hash * a + key[i]) % static_cast<int>(tableSize);
    }
//    std::cout << "striiiing hash " << hash << std::endl;
    return hash;
}

template <class T>
struct HashTableLine {
    T Data;
    bool isDeleted;
    bool isEmpty;
    HashTableLine() : isDeleted(false), Data(), isEmpty(true) {};
};

template <class T>
class HashTable {
public:
    HashTable();
    ~HashTable();

    bool Has(const T& key, int hash = -1) const;
    bool Add(const T& key);
    bool Delete(const T& key);

private:
//    struct HashTableLine {
//        T Data;
//        bool isDeleted;
//    };

    std::vector<HashTableLine<T>> table;
    float c1, c2;
    void reHash();
    void growTable();
    int quadProbe(const int index, const int iteration) const {
        // Поправить на рекуернтню
        return static_cast<int>(index + iteration * c1 + iteration * iteration * c2) % static_cast<int>(table.size());
    }
    int keysCount;
    int deletedCount;
};

template <class T>
void HashTable<T>::growTable() {
//    std::vector<HashTableLine<T>> newTable(table.size() * GrowFactor);
    table.resize(table.size() * GrowFactor);
    reHash();
}

template <class T>
void HashTable<T>::reHash() {
    std::vector<HashTableLine<T>> newTable(table.size());
    for (int i = 0; i < table.size(); ++i) {
        if (!table.at(i).isDeleted && !table.at(i).isEmpty) {
            int hash = Hash<T>(table.at(i).Data, newTable.size());
            // Проблема была не в рекурентности, а в первом параметре пробы (он должен быть одним для всех итераций)
            for (int j = hash, iteration = 0; iteration < newTable.size(); j = quadProbe(hash, ++iteration)) {
                if (newTable.at(j).isEmpty) {
                    newTable.at(j) = table.at(i);
                    break;
                }
            }
        }
    }
    table = newTable;
}

template <class T>
HashTable<T>::HashTable() : keysCount(0), deletedCount(0), c1(0.5), c2(0.5) {
    table.resize(DefaultHashTableSize);
//    c1 = checkSimple(c1, table.size());
//    c2 = checkSimple(c2, table.size());
}

template <class T>
HashTable<T>::~HashTable() = default; /*{
    for (int i = 0; i < table.size(); ++i) {
        delete table.at(i).Data;
    }
}*/
template <class T>
bool HashTable<T>::Has(const T &key, int hash) const {
    if (hash < 0) {
        hash = Hash<T>(key, table.size());
    }
//    std::cout << "test Has: hash = " << hash << std::endl;
    bool status = false;
    for (int i = hash, iteration = 0; iteration < table.size(); i = quadProbe(i, ++iteration)) {
        if (!table.at(i).isDeleted && table.at(i).isEmpty) {
//            std::cout << "test Has: bad!\n";
            status = false;
            break;
        }
        if (!table.at(i).isEmpty) {
            if (table.at(i).Data == key) {
                status = true;
//                std::cout << "index: " << i << std::endl;
                break;
            }
        }
    }
    return status;
}

template <class T>
bool HashTable<T>::Add(const T &key) {
    bool status = false;
    int hash = Hash<T>(key, table.size());
//    std::cout << "test Add: hash = " << hash;
// 2 прохода - has и add - это плохо. Но в данном случае Has не обрабатывает isDeleted, поэтому такую ячейку надо искать заново.
    if (Has(key, hash)) {
        return false;
    }
    if (keysCount + 1 > MaxAlpha * table.size()) {
        growTable();
    }
    for (int i = hash, iteration = 0; iteration < table.size(); i = quadProbe(i, ++iteration)) {
//        std::cout << "check\n";
        if (table.at(i).isEmpty) {
            if (table.at(i).isDeleted) {
                deletedCount--;
            }
//            std::cout << " index: " << i << std::endl;
            table.at(i).isDeleted = false;
            // new - тоже плохо
//            T* tmp = new T(key);
            table.at(i).Data = key;
            table.at(i).isEmpty = false;
            keysCount++;
            status = true;
            break;
        }
    }
    return status;
}

template <class T>
bool HashTable<T>::Delete(const T &key) {
    bool status = false;
    int hash = Hash<T>(key, table.size());
//    std::cout << "test Delete: hash = " << hash;
    for (int i = hash, iteration = 0; iteration < table.size(); i = quadProbe(i, ++iteration)) {
        if (!table.at(i).isDeleted && table.at(i).isEmpty) {
            status = false;
            break;
        }
        if (!table.at(i).isDeleted) {
            if (!table.at(i).isEmpty) {
                if (table.at(i).Data == key) {
//                    delete table.at(i).Data;
//                    table.at(i).Data = nullptr;
                    table.at(i).isDeleted = true;
                    table.at(i).isEmpty = true;
                    deletedCount++;
                    keysCount--;
                    status = true;
//                    std::cout << " index: " << i << std::endl;
                    break;
                } else {
                    // TODO: Mismatch!
                }
            } else {
                // TODO: empty!
            }
        }
    }
    if (deletedCount > MaxDeleted * table.size()) {
//        std::cout << "test rehash from delete\n";
        reHash();
    }
    return status;
}


int main() {
    HashTable<std::string> table;
    char command = 0;
    std::string key;
    while (std::cin >> command >> key) {
        switch (command) {
            case '+': {
                std::cout << ((table.Add(key)) ? "OK" : "FAIL" ) << std::endl;
                break;
            }
            case '-': {
                std::cout << ((table.Delete(key)) ? "OK" : "FAIL" ) << std::endl;
                break;
            }
            case '?': {
                std::cout << ((table.Has(key)) ? "OK" : "FAIL" ) << std::endl;
                break;
            }
            default: {
//                std::cout << "Unknown command" << std::endl;
                assert(false);
            }
        }
    }
    return 0;
}
