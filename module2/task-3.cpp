// Вычислить количество узлов в самом широком слое декартового дерева и количество узлов в самом широком слое
// наивного дерева поиска. Вывести их разницу. Разница может быть отрицательна.

#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>

template <class T>
struct TreapNode {
    T Data;
    int Priority;
    TreapNode *Left;
    TreapNode *Right;
    explicit TreapNode(const T value, const int prio) : Data(value), Priority(prio), Left(nullptr), Right(nullptr) {};
};


template <class T>
class DecartTree {
public:
    DecartTree();
    ~DecartTree();
    int getMaxWidthNodes();
    void TraverseDFS(TreapNode<T> *node, void (* foo)(TreapNode<T> *));
    TreapNode<T>* Find(T& value);
    TreapNode<T>* FindMin(TreapNode<T> *node);
    void Insert(T value, int prio);
//    bool Delete(T& value);
private:
    TreapNode<T> *root;
//    void deleteNode(TreapNode<T> *);
    void split(TreapNode<T> *currentNode, T key, TreapNode<T> *&left, TreapNode<T> *&right);
    TreapNode<T> *merge(TreapNode<T> *left, TreapNode<T> *right);
};

template <class T>
DecartTree<T>::DecartTree() : root(nullptr) {}

template <class T>
DecartTree<T>::~DecartTree() {
    TraverseDFS(root, [](TreapNode<T> * node) {delete node;});
}

template <class T>
void DecartTree<T>::TraverseDFS(TreapNode<T> *node, void (* foo)(TreapNode<T> *)) {
    if(node == nullptr) {
        return;
    }
    std::stack<TreapNode<T> *> Nodes;
    TreapNode<T> *current = root;
    TreapNode<T> *lastVisited = nullptr;
    while (!Nodes.empty() || current) {
        if (current) {
            Nodes.push(current);
            current = current->Left;
        } else {
            TreapNode<T> * peek = Nodes.top();
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
void DecartTree<T>::Insert(T value, int prio) {
    TreapNode<T> *nodeParent = nullptr;
    TreapNode<T> *node = root;
    while (node != nullptr && node->Priority >= prio) {
        nodeParent = node;
        node = node->Data > value ? node->Left : node->Right;
    }

    TreapNode<T> *leftT = nullptr;
    TreapNode<T> *rightT = nullptr;
    std::cout << "test1\n";
    split(node, value, leftT, rightT);
    std::cout << "test2\n";

    TreapNode<T> *insertNode = new TreapNode<T>(value, prio);

    insertNode->Left = leftT;
    insertNode->Right = rightT;

    if (!nodeParent) {
        root = insertNode;
    } else if (nodeParent->Data > insertNode->Data) {
        nodeParent->Left = insertNode;
    } else {
        nodeParent->Right = insertNode;
    }
}

template <class T>
TreapNode<T>* DecartTree<T>::Find(T &value) {
    TreapNode<T> *node = root;
    while (node != nullptr && node->Data != value) {
        node = node->Data > value ? node->Left : node->Right;
    }
    return node;
}

template <class T>
TreapNode<T>* DecartTree<T>::FindMin(TreapNode<T> *node) {
    assert(node != nullptr);
    while (node->Left != nullptr) {
        node = node->Left;
    }
    return node;
}

template <class T>
void DecartTree<T>::split(TreapNode<T> *currentNode, T key, TreapNode<T> *&left, TreapNode<T> *&right) {
    if (currentNode == nullptr) {
        left = nullptr;
        right = nullptr;
    } else if (currentNode->Data <= key) {
        split(currentNode->Right, key, currentNode->Right, right);
        left = currentNode;
    } else {
        split(currentNode->Left, key, left, currentNode->Left);
        right = currentNode;
    }
}

template <class T>
TreapNode<T>* DecartTree<T>::merge(TreapNode<T> *left, TreapNode<T> *right) {
    if (left == 0 || right == 0) {
        return left == 0 ? right : left;
    }
    if (left->Priority > right->Priority) {
        left->Right = merge(left->Right, right);
        return left;
    } else {
        right->Left = merge(left, right->Left);
        return right;
    }
}

template <class T>
int DecartTree<T>::getMaxWidthNodes() {
    if (root == nullptr) {
        return 0;
    }
    std::queue<TreapNode<T> *> q;
    q.push(root);
    q.push(nullptr);
    int maxNodes = 0;
    int nodes = 0;
    int maxCount = 0;
    while (!q.empty()) {
        TreapNode<T> *node = q.front();
        q.pop();
//        foo(node);
        if (node == nullptr) {
            int count = q.size() - 1;
            if (count > maxCount) {
                maxCount = count;
                maxNodes = nodes;
            }
            nodes = 0;
            q.push(nullptr);
            q.pop();
        } else {
            if (node->Left || node->Right) {
                nodes++;
            }
            if (node->Left != nullptr) {
                q.push(node->Left);

            }
            if (node->Right != nullptr) {
                q.push(node->Right);
            }
        }
    }
    return maxNodes;
}

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
    int getMaxWidthNodes();
    void TraverseDFS(BinaryNode<T> *node, void (* foo)(BinaryNode<T> *));
    BinaryNode<T>* Find(T& value);
    BinaryNode<T>* FindMin(BinaryNode<T> *node);
    void Insert(T value);
//    bool Delete(T& value);
private:
    BinaryNode<T> *root;

//    void deleteNode(BinaryNode<T> *);
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
int BinaryTree<T>::getMaxWidthNodes() {
    if (root == nullptr) {
        return 0;
    }
    std::queue<BinaryNode<T> *> q;
    q.push(root);
    q.push(nullptr);
    int maxNodes = 0;
    int nodes = 0;
    int maxCount = 0;
    while (!q.empty()) {
        BinaryNode<T> *node = q.front();
        q.pop();
//        foo(node);
        if (node == nullptr) {
            int count = q.size() - 1;
            if (count > maxCount) {
                maxCount = count;
                maxNodes = nodes;
            }
            nodes = 0;
            q.push(nullptr);
            q.pop();
        } else {
            if (node->Left || node->Right) {
                nodes++;
            }
            if (node->Left != nullptr) {
                q.push(node->Left);

            }
            if (node->Right != nullptr) {
                q.push(node->Right);
            }
        }
    }
    return maxNodes;
}

template <class T>
BinaryNode<T>* BinaryTree<T>::Find(T &value) {
    BinaryNode<T> *node = root;
    while (node != nullptr && node->Data != value) {
        node = node->Data > value ? node->Left : node->Right;
    }
    return node;
}

template <class T>
BinaryNode<T>* BinaryTree<T>::FindMin(BinaryNode<T> * node) {
    assert(node != nullptr);
    while (node->Left != nullptr) {
        node = node->Left;
    }
    return node;
}

template <class T>
void BinaryTree<T>::Insert(T value) {
    BinaryNode<T> *nodeParent = nullptr;
    BinaryNode<T> *node = root;
    while (node != nullptr) {
        nodeParent = node;
        node = node->Data > value ? node->Left : node->Right;
    }
    node = new BinaryNode<T>(value);
    if (!nodeParent) {
        root = node;
    } else if (nodeParent->Data > node->Data) {
        nodeParent->Left = node;
    } else {
        nodeParent->Right = node;
    }
}

//template <class T>
//void BinaryTree<T>::deleteNode(BinaryNode<T> *node) {
//    if (node->Left == nullptr) { // Если нет левого поддерева.
//        BinaryNode<T> *right = node->Right; // Подставляем правое, может быть 0.
//        delete node;
//        node = right;
//    } else if (node->Right == nullptr) { // Если нет правого поддерева.
//        BinaryNode<T> *left = node->Left; // Подставляем левое.
//        delete node;
//        node = left;
//    } else { // Есть оба поддерева.
//        // Ищем минимальный элемент в правом поддереве и его родителя.
//        BinaryNode<T> *minParent = node;
//        BinaryNode<T> *min = node->Right;
//        while (min->Left != nullptr) {
//            minParent = min;
//            min = min->Left;
//        }
//        // Переносим значение.
//        node->Data = min->Data;
//        // Удаляем min, подставляя на его место min->Right.
//        (minParent->Left == min ? minParent->Left : minParent->Right) = min->Right;
//        delete min;
//    }
//}
//
//template <class T>
//bool BinaryTree<T>::Delete(T &value) {
//    if (root == nullptr) {
//        return false;
//    }
//    BinaryNode<T> *node = root;
//    while (node != nullptr && node->Data != value) {
//        node = node->Data > value ? node->Left : node->Right;
//    }
//    if (node != nullptr && node->Data == value) { // Нашли, удаляем.
//        DeleteNode(node);
//        return true;
//    }
//    return false;
//}

int main() {
    size_t n = 0;
    std::cin >> n;
    DecartTree<int> DecTree;
    BinaryTree<int> BinTree;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        int priority = 0;
        std::cin >> value >> priority;
        DecTree.Insert(value, priority);
        BinTree.Insert(value);
    }
//    Tree.TraverseDFS([](TreapNode<int> *node){std::cout << node->Data << " ";});
    int result = DecTree.getMaxWidthNodes() - BinTree.getMaxWidthNodes();
    std::cout << result << std::endl;

    return 0;
}