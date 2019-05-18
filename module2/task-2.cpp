// Косенков Александр АПО-12
// Алгоритмы и структуры данных - 2-й модуль
// Задача 2_4
//
// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в
// правое поддерево root; иначе в левое поддерево root.
// Рекурсия запрещена.
// Вывести элементы в порядке level-order (по слоям, “в ширину”).

#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>

template <class T>
struct BinaryNode {
    T Data;
    BinaryNode *Left; // NULL, если нет.
    BinaryNode *Right; // NULL, если нет.
    explicit BinaryNode(const T value) : Data(value), Left(nullptr), Right(nullptr) {};
};


template <class T>
class BinaryTree {
public:
    BinaryTree();
    ~BinaryTree();
    // Алгоритм обхода не должен знать про узлы
    void TraverseBFS(void (* foo)(T&));
    void TraverseDFS(BinaryNode<T> *node, void (* foo)(BinaryNode<T> *));
    BinaryNode<T>* Find(T& value) const;
    BinaryNode<T>* FindMin(BinaryNode<T> *node) const;
    void Insert(T value);
    bool Delete(T& value);
private:
    BinaryNode<T> *root;

    void deleteNode(BinaryNode<T> *);
};

template <class T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <class T>
BinaryTree<T>::~BinaryTree() {
    TraverseDFS(root, [](BinaryNode<T> * node) {delete node;});
}

template <class T>
void BinaryTree<T>::TraverseDFS(BinaryNode<T> *node, void (* foo)(BinaryNode<T> *)) {
    if(node == nullptr) {
        return;
    }
    std::stack<BinaryNode<T> *> Nodes;
    BinaryNode<T> *current = root;
    BinaryNode<T> *lastVisited = nullptr;
    while (!Nodes.empty() || current) {
        if (current) {
            Nodes.push(current);
            current = current->Left;
        } else {
            BinaryNode<T> * peek = Nodes.top();
            if (peek->Right && lastVisited != peek->Right) {
                current = peek->Right;
            } else {
                foo(peek);
                lastVisited = Nodes.top();
                Nodes.pop();
            }
        }
    }
}

template <class T>
void BinaryTree<T>::TraverseBFS(void (* foo)(T&)) {
    if (root == nullptr) {
        return;
    }
    std::queue<BinaryNode<T> *> q;
    q.push(root);
    while (!q.empty()) {
        BinaryNode<T> *node = q.front();
        q.pop();
        foo(node->Data);
        if (node->Left != nullptr) {
            q.push(node->Left);

        }
        if (node->Right != nullptr) {
            q.push(node->Right);
        }
    }
}

template <class T>
BinaryNode<T>* BinaryTree<T>::Find(T &value) const {
    BinaryNode<T> *node = root;
    while (node != nullptr && node->Data != value) {
        node = node->Data > value ? node->Left : node->Right;
    }
    return node;
}

template <class T>
BinaryNode<T>* BinaryTree<T>::FindMin(BinaryNode<T> * node) const {
    assert(node != nullptr);
    while (node->Left != nullptr) {
        node = node->Left;
    }
    return node;
}

template <class T>
void BinaryTree<T>::Insert(T value) {
    BinaryNode<T> *nodeParent = root;
    BinaryNode<T> *node = root;
    while (node != nullptr) {
        nodeParent = node;
        node = node->Data > value ? node->Left : node->Right;
    }
    node = new BinaryNode<T>(value);
    if (!root) {
        root = node;
    } else if (nodeParent->Data > node->Data) {
        nodeParent->Left = node;
    } else {
        nodeParent->Right = node;
    }
}

template <class T>
void BinaryTree<T>::deleteNode(BinaryNode<T> *node) {
    if (node->Left == nullptr) { // Если нет левого поддерева.
        BinaryNode<T> *right = node->Right; // Подставляем правое, может быть 0.
        delete node;
        node = right;
    } else if (node->Right == nullptr) { // Если нет правого поддерева.
        BinaryNode<T> *left = node->Left; // Подставляем левое.
        delete node;
        node = left;
    } else { // Есть оба поддерева.
    // Ищем минимальный элемент в правом поддереве и его родителя.
        BinaryNode<T> *minParent = node;
        BinaryNode<T> *min = node->Right;
        while (min->Left != nullptr) {
            minParent = min;
            min = min->Left;
        }
    // Переносим значение.
        node->Data = min->Data;
    // Удаляем min, подставляя на его место min->Right.
        (minParent->Left == min ? minParent->Left : minParent->Right) = min->Right;
        delete min;
    }
}

template <class T>
bool BinaryTree<T>::Delete(T &value) {
    if (root == nullptr) {
        return false;
    }
    BinaryNode<T> *node = root;
    while (node != nullptr && node->Data != value) {
        node = node->Data > value ? node->Left : node->Right;
    }
    if (node != nullptr && node->Data == value) { // Нашли, удаляем.
        DeleteNode(node);
        return true;
    }
    return false;
}

int main() {
    size_t n = 0;
    std::cin >> n;
    BinaryTree<int> Tree;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        Tree.Insert(value);
    }
    Tree.TraverseBFS([](int &node){std::cout << node << " ";});
    std::cout << std::endl;

    return 0;
}