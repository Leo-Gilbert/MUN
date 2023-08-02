#include <iostream>
#include <vector>
#include <queue>

template<typename T>
class Graph {
public:
    Graph(int vertices) : numOfVertices(vertices) {
        adjMatrix.resize(vertices);
        vertexData.resize(vertices);
    }

    void addEdge(int source, int destination) {
        adjMatrix[source].push_back(destination);
        adjMatrix[destination].push_back(source);
    }

    void setVertexData(int vertex, const T& data) {
        if (vertex >= 0 && vertex < numOfVertices) {
            vertexData[vertex] = data;
        }
    }

    T getVertexData(int vertex) const {
        if (vertex >= 0 && vertex < numOfVertices) {
            return vertexData[vertex];
        }
        return T();
    }

    std::vector<int> shortestPath(int start) {
        std::vector<int> distances(numOfVertices, -1);
        std::queue<int> q;

        distances[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (const auto& neighbor : adjMatrix[node]) {
                if (distances[neighbor] == -1) {
                    q.push(neighbor);
                    distances[neighbor] = distances[node] + 1;
                }
            }
        }

        return distances;
    }

    void printShortestMatrix() {
        std::vector<int> rows;
        for (int i = 0; i < numOfVertices; i++) {
            rows = shortestPath(i);
            for (auto distances : rows) {
                std::cout << distances << " ";
            }
            std::cout << "  [Data: " << vertexData[i] << "]" << std::endl;
        }
    }

private:
    int numOfVertices;
    std::vector<std::vector<int>> adjMatrix;
    std::vector<T> vertexData;
};

int main() {
    Graph<int> graph(6); 
    graph.addEdge(5, 4);
    graph.addEdge(5, 3);
    graph.addEdge(3, 2);
    graph.addEdge(4, 2);
    graph.addEdge(2, 0);
    graph.addEdge(1, 0);
    graph.addEdge(3, 1);

    graph.setVertexData(0, 0);
    graph.setVertexData(1, 1);
    graph.setVertexData(2, 2);
    graph.setVertexData(3, 3);
    graph.setVertexData(4, 4);
    graph.setVertexData(5, 5);

    graph.printShortestMatrix();

    return 0;
}
