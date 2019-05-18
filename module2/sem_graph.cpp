#include <vector>
#include <iostream>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(int count);
    ListGraph(const IGraph& graph);

    ~ListGraph();
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacencyLists;
//    std::vector<std::vector<int>>
    // Можно пожертвовать памятью и хранить еще и входящие верщины?
};

ListGraph::ListGraph(int count) {
    adjacencyLists.resize(count);
}

ListGraph::ListGraph(const IGraph& graph) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjacencyLists.push_back(graph.GetNextVertices(i));
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
    adjacencyLists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return adjacencyLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    std::vector<int> next;
    for (int i = 0; i < adjacencyLists[vertex].size(); ++i) {
        next.push_back(adjacencyLists[vertex][i]);
    }
    return next;
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev;
    for (int i = 0; i < adjacencyLists.size(); ++i) {
        for (int j = 0; j < adjacencyLists[i].size(); ++j) {
            if (adjacencyLists[i][j] == vertex) {
                prev.push_back(i);
            }
        }
    }
    return prev;
}
// visit (номер вершины)
void BFS(const IGraph& graph, int vertex, void (* visit)(int)) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    // Тут храним текуший фронт - ?
    std::queue<int> qu;
    // Кладем вершину, с которой наиснаем обход
    qu.push(vertex);
    visited[vertex] = true;

    while (qu.size() != 0) {
        int current = qu.front();
        qu.pop();
        visit(current);
        std::vector<int> adjacentVertices = graph.GetNextVertices(current);
        for (int i = 0; i < adjacentVertices.size(); ++i) {
            if (!visited[adjacentVertices[i]]) {
                qu.push(adjacentVertices[i]);
                visited[adjacentVertices[i]] = true;
            }
        }
    }
}

int main() {
    ListGraph gr(5);
    gr.AddEdge(0, 1);
    gr.AddEdge(0, 2);
    gr.AddEdge(0, 3);
    gr.AddEdge(1, 4);
    gr.AddEdge(2, 1);
    gr.AddEdge(2, 3);
    gr.AddEdge(2, 4);
    gr.AddEdge(4, 3);
    BFS(gr, 0, [](int vertex){ std::cout << vertex << " ";});
    return 0;
}