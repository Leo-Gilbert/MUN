/*

1. A DAG is a graph where no path leads to travelling in a cycle.

2. E = edges, V = verticies 

V * (V - 1) / 2

*/

#include <iostream>
#include <vector>
#include <sstream>
#include <queue>
#include <limits>

template<typename V, typename E>
class Graph
{
public:
	using VertexID = size_t;

    E dijkstraShortestPath(VertexID source, VertexID destination) {
        size_t numVertices = vertices_.size();
        std::vector<E> dist(numVertices, std::numeric_limits<E>::max());
        dist[source] = E{}; // Distance from the source vertex to itself is 0

        std::priority_queue<std::pair<E, VertexID>, std::vector<std::pair<E, VertexID>>, std::greater<>> pq;
        pq.push(std::make_pair(E{}, source));

        while (!pq.empty()) {
            E uDist = pq.top().first;
            VertexID u = pq.top().second;
            pq.pop();

            if (uDist > dist[u]) {
                continue;
            }

            for (VertexID v = 0; v < numVertices; ++v) {
                if (aMatrix[u][v] != E{} && dist[u] + aMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + aMatrix[u][v];
                    pq.push(std::make_pair(dist[v], v));
                }
            }
        }

        return dist[destination];
    }

	VertexID addVertex(V vertex){
        vertices_.push_back(vertex);

        for (auto& row : aMatrix)
            row.resize(vertices_.size());

        aMatrix.resize(vertices_.size(), std::vector<E>(vertices_.size()));

        return vertices_.size() - 1;
    }

	Graph& addEdge(VertexID source, VertexID destination, E edge){
        aMatrix[source][destination] = edge;
        return *this;
    }

	void CSVParse(std::string csvfile, size_t verticenum){

        for (size_t i = 0; i < verticenum; ++i){
            addVertex(i);
        }

        std::stringstream ss(csvfile);
        std::string line;

        std::vector<std::string> lines;

        while (std::getline(ss, line)){
            lines.push_back(line);
        }

        size_t row = 0;
        for (size_t i = 0; i < lines.size(); ++i){
            std::stringstream currentss(lines[i]);
            std::string token;

            size_t column = 0;

            while (std::getline(currentss, token, ',')){
                if (token == ""){
                    token = "\t";
                }
                addEdge(row, column, token);
                ++column;
            }

            ++row;

        }

    }

    std::vector<std::vector<E>> aMatrix = std::vector<std::vector<E>>();
    std::vector<V> vertices_;
};

int main() {
    
    Graph<size_t, std::string> Graph;
    std::string CSV = ",Bay Bulls,CBS,Goulds,Holyrood,Paradise,Portugal Cove,St. John’s \n "
    "Bay Bulls,0,,15.3,31.9,,, \n "
    "CBS,,0,,24.1,6.6,16.4, \n "
    "Goulds,,,0,,17.5,,14.3 \n "
    "Holyrood,,,,0,,, \n "
    "Paradise,,,,,0,12.5,18.6 \n "
    "Portugal Cove,,,,,,0,14.2 \n "
    "St. John’s,,,,,,,0 \n";

    Graph.CSVParse(CSV, 8);

    // Print the adjacency matrix
    for (size_t i = 0; i < Graph.aMatrix.size(); ++i) {
        for (size_t j = 0; j < Graph.aMatrix[i].size(); ++j) {
            std::cout << Graph.aMatrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    size_t sourceVertex = 1; // Bay Bulls
    size_t destinationVertex = 7;

    std::cout<< std::endl << "Shorted Path: " << Graph.dijkstraShortestPath(sourceVertex, destinationVertex) <<std::endl;

    return 0;
}