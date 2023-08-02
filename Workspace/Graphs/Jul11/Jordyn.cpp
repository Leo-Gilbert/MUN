#include <iostream>
#include <vector>
#include <sstream>

template<typename V, typename E>
class Graph
{
public:
    
    
    using VertexID = size_t;
    
    

    VertexID addVertex(V vertex) {
        vertices.push_back(vertex);

// resize each row to the new size of vertices
        for (auto& row : adjacencyMatrix)
            row.resize(vertices.size());


        adjacencyMatrix.resize(vertices.size(), std::vector<E>(vertices.size()));

        return vertices.size() - 1;
        
    }

    Graph& addEdge(VertexID row, VertexID column, E edge) {
        adjacencyMatrix[row][column] = edge;
        return *this;
    }


    std::vector<std::vector<E>> adjacencyMatrix = std::vector<std::vector<E>>();
    std::vector<V> vertices;
    
    void CSV(std::string CSVString, int vertices){
        
        for(int i = 0; i < vertices; i++){
            addVertex(i);
            
        }
        
        
        std::stringstream ss(CSVString);
        std::string line;
        
        std::vector<std::string> lines;
        
        // create vector of lines
        while (std::getline(ss, line)) {
            
            lines.push_back(line);
        }
        
        int row = 0;
        for (std::size_t i = 0; i < lines.size(); ++i) {
            
            std::string currentLine = lines[i];
            
            std::stringstream ss(currentLine);
            std::string token;
            
            int column = 0;
            while(std::getline(ss, token, ',')){
                
                if(token == ""){
                    token = "\t";
                }
                
                addEdge(row, column, token);
                column ++;
            }
            row++;
            
        }
              
        }
    
    
};

int main() {
    
    Graph<int, std::string> graph;
    std::string CSV = ",Bay Bulls,CBS,Goulds,Holyrood,Paradise,Portugal Cove,St. John’s \n Bay Bulls,0,,15.3,31.9,,, \n CBS,,0,,24.1,6.6,16.4, \n Goulds,,,0,,17.5,,14.3 \n Holyrood,,,,0,,, \n Paradise,,,,,0,12.5,18.6 \n Portugal Cove,,,,,,0,14.2 \n St. John’s,,,,,,,0 \n";

    
    graph.CSV(CSV,8);

    // Print adjacency matrix
    std::cout << "Adjacency Matrix:\n";
    for (const auto& row : graph.adjacencyMatrix) {
        for (const auto& value : row)
            std::cout << value << '\t';
        std::cout << '\n';
    }
    


    return 0;
}