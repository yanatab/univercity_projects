#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <utility>
#include "errors.hpp"

enum GraphType { DIRECTED, UNDIRECTED };

template<typename VType, typename WType = int>
class Edge {
private:
    VType from;    
    VType to;     
    WType weight;  
    bool directed; 

public:
    Edge(VType from, VType to, WType weight = 0, bool directed = true)
        : from(from), to(to), weight(weight), directed(directed) {
        if (weight < 0) {
            graph_errors_detection(GraphError::INVALID_WEIGHT);
            throw GraphException(GraphError::INVALID_WEIGHT);
        }
    }

    VType getFrom() const { return from; }
    VType getTo() const { return to; }
    WType getWeight() const { return weight; }
    bool isDirected() const { return directed; }
    
    void setWeight(WType newWeight) { 
        if (newWeight < 0) {
            graph_errors_detection(GraphError::INVALID_WEIGHT);
            throw GraphException(GraphError::INVALID_WEIGHT);
        }
        weight = newWeight; 
    }

    bool isIncidentTo(VType vertex) const {
        if (directed) {
            return vertex == from;
        } else {
            return vertex == from || vertex == to;
        }
    }

    VType getOtherVertex(VType vertex) const {
        if (vertex == from) return to;
        if (vertex == to) return from;
        graph_errors_detection(GraphError::INVALID_VERTEX);
        throw GraphException(GraphError::INVALID_VERTEX);
    }
};

template<typename VType>
class Vertex {
private:
    VType id;
    std::vector<Edge<VType, int>*> edges;
    
public:
    Vertex(VType id) : id(id) {}
    ~Vertex() {
        edges.clear();
    }
    

    Vertex& operator=(const Vertex& other) {
        if (this != &other) {
            id = other.id;
            edges = other.edges;
        }
        return *this;
    }

    VType getId() const { return id; }

    void addEdge(Edge<VType, int>* edge) {
        if (!edge) {
            graph_errors_detection(GraphError::NULL_VERTEX);
            throw GraphException(GraphError::NULL_VERTEX);
        }
        
        if (edge->getFrom() == id || (!edge->isDirected() && edge->getTo() == id)) {
            edges.push_back(edge);
        }
    }

    const std::vector<Edge<VType, int>*>& getEdges() const {
        return edges;
    }
    
    std::vector<VType> getAdjacentVertices() const {
        std::vector<VType> adjacent;
        for (const auto& edge : edges) {
            if (edge->getFrom() == id) {
                adjacent.push_back(edge->getTo());
            } else if (!edge->isDirected()) {
                adjacent.push_back(edge->getFrom());
            }
        }
        return adjacent;
    }

    bool isAdjacentTo(VType other) const {
        for (const auto& edge : edges) {
            if ((edge->getFrom() == id && edge->getTo() == other) ||
                (!edge->isDirected() && edge->getFrom() == other && edge->getTo() == id)) {
                return true;
            }
        }
        return false;
    }


    bool removeEdge(Edge<VType, int>* edge) {
        if (!edge) return false;
        auto it = std::find(edges.begin(), edges.end(), edge);
        if (it != edges.end()) {
            edges.erase(it);
            return true;
        }
        return false;
    }
};

template<typename VType, typename WType = int>
class Graph {
private:
    GraphType type;   
    std::map<VType, Vertex<VType>*> vertices;
    std::vector<Edge<VType, WType>*> edges; 
    
public:
    Graph(GraphType graphType = DIRECTED) : type(graphType) {}

    Graph(const Graph&) = delete;

    Graph(Graph&& other) noexcept 
        : type(other.type), vertices(std::move(other.vertices)), edges(std::move(other.edges)) {
        other.vertices.clear();
        other.edges.clear();
    }
    
    ~Graph() {
        for (auto& pair : vertices) {
            delete pair.second; 
        }
        vertices.clear();
       
        for (auto& edge : edges) {
            delete edge;  
        }
        edges.clear();
    }

    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            for (auto& pair : vertices) delete pair.second;
            for (auto& edge : edges) delete edge;
            
            type = other.type;
            vertices = std::move(other.vertices);
            edges = std::move(other.edges);
            
            other.vertices.clear();
            other.edges.clear();
        }
        return *this;
    }

    Vertex<VType>* addVertex(VType id) {
        if (vertices.find(id) != vertices.end()) {
            graph_errors_detection(GraphError::DUPLICATE_VERTEX);
            throw GraphException(GraphError::DUPLICATE_VERTEX);
        }
        
        Vertex<VType>* vertex = new Vertex<VType>(id);
        vertices[id] = vertex;
        return vertex;
    }

    Vertex<VType>* getVertex(VType id) const {
        auto it = vertices.find(id);
        if (it == vertices.end()) {
            graph_errors_detection(GraphError::VERTEX_NOT_FOUND);
            throw GraphException(GraphError::VERTEX_NOT_FOUND);
        }
        return it->second;
    }

    Edge<VType, WType>* addEdge(VType from, VType to, WType weight) {
        auto vFrom = vertices.find(from);
        auto vTo = vertices.find(to);
        
        if (vFrom == vertices.end() || vTo == vertices.end()) {
            graph_errors_detection(GraphError::VERTEX_NOT_FOUND);
            throw GraphException(GraphError::VERTEX_NOT_FOUND);
        }
        
        if (hasEdge(from, to)) {
            graph_errors_detection(GraphError::DUPLICATE_EDGE);
            throw GraphException(GraphError::DUPLICATE_EDGE);
        }
        
        bool directed = (type == DIRECTED);
        Edge<VType, WType>* edge = new Edge<VType, WType>(from, to, weight, directed);
        edges.push_back(edge);
        
        vFrom->second->addEdge(edge);
        if (!directed) {
            vTo->second->addEdge(edge);
        }
        
        return edge;
    }

    const std::map<VType, Vertex<VType>*>& getVertices() const {
        return vertices;
    }
    
    const std::vector<Edge<VType, WType>*>& getEdges() const {
        return edges;
    }
    
    size_t getVertexCount() const {
        return vertices.size();
    }
    
    size_t getEdgeCount() const {
        return edges.size();
    }
    
    bool hasEdge(VType from, VType to) const {
        Vertex<VType>* vertex = getVertex(from);
        return vertex->isAdjacentTo(to);

    }
    


    std::vector<VType> dfs(VType startVertex) const {
        if (vertices.empty()) {
            graph_errors_detection(GraphError::EMPTY_GRAPH);
            throw GraphException(GraphError::EMPTY_GRAPH);
        }
        
        if (vertices.find(startVertex) == vertices.end()) {
            graph_errors_detection(GraphError::VERTEX_NOT_FOUND);
            throw GraphException(GraphError::VERTEX_NOT_FOUND);
        }
        
        std::vector<VType> result;
        std::set<VType> visited;
        std::stack<VType> s;
        
        s.push(startVertex);
        
        while (!s.empty()) {
            VType vertex = s.top();
            s.pop();
            
            if (visited.find(vertex) == visited.end()) {
                visited.insert(vertex);
                result.push_back(vertex);
                
                Vertex<VType>* v = getVertex(vertex);
                auto adjacent = v->getAdjacentVertices();
                for (auto it = adjacent.rbegin(); it != adjacent.rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        s.push(*it);
                    }
                }
            }
        }
        
        return result;
    }

    bool isConnected() const {
        if (vertices.empty()) {
            return true; 
        }
        
        if (type == DIRECTED) {
            graph_errors_detection(GraphError::GRAPH_NOT_UNDIRECTED);
            throw GraphException(GraphError::GRAPH_NOT_UNDIRECTED);
        }
        
        auto startVertex = vertices.begin()->first;
        auto visited = dfs(startVertex);
        
        return visited.size() == vertices.size();
    }

    std::vector<VType> shortestPath(VType start, VType end) const { 
        if (vertices.find(start) == vertices.end() || vertices.find(end) == vertices.end()) {
            graph_errors_detection(GraphError::VERTEX_NOT_FOUND);
            throw GraphException(GraphError::VERTEX_NOT_FOUND);
        }
        
        std::map<VType, WType> distance;
        std::map<VType, VType> previous;
        std::set<VType> unvisited;
        
        for (const auto& pair : vertices) {
            distance[pair.first] = std::numeric_limits<WType>::max();
            previous[pair.first] = VType(); 
            unvisited.insert(pair.first);
        }
        distance[start] = 0;
        
        while (!unvisited.empty()) {
            VType current;
            WType minDist = std::numeric_limits<WType>::max();
            
            for (const auto& v : unvisited) {
                if (distance[v] < minDist) {
                    minDist = distance[v];
                    current = v;
                }
            }
            
            if (minDist == std::numeric_limits<WType>::max()) {
                break; 
            }
            
            unvisited.erase(current);
            
            Vertex<VType>* vertex = getVertex(current);
            for (const auto& edge : vertex->getEdges()) {
                VType neighbor = edge->getTo();
                if (edge->getFrom() != current && edge->isDirected()) {
                    continue; 
                }
                
                if (!edge->isDirected() && edge->getFrom() != current) {
                    neighbor = edge->getFrom();
                }
                
                WType newDist = distance[current] + edge->getWeight();
                if (newDist < distance[neighbor]) {
                    distance[neighbor] = newDist;
                    previous[neighbor] = current;
                }
            }
        }
        
        if (distance[end] == std::numeric_limits<WType>::max()) {
            return {};
        }
        
        std::vector<VType> path;
        for (VType v = end; ; v = previous[v]) {
            path.push_back(v);
            if (v == start) break;
            if (previous.find(v) == previous.end()) break;
        }
        
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<VType> topologicalSort() const {
        if (type != DIRECTED) {
            graph_errors_detection(GraphError::GRAPH_NOT_DIRECTED);
            throw GraphException(GraphError::GRAPH_NOT_DIRECTED);
        }
        
        std::map<VType, int> inDegree;
        
        for (const auto& pair : vertices) {
            inDegree[pair.first] = 0;
        }
        
        for (const auto& edge : edges) {
            inDegree[edge->getTo()]++;
        }
        
        std::queue<VType> zeroInDegreeQueue;
        for (const auto& pair : inDegree) {
            if (pair.second == 0) {
                zeroInDegreeQueue.push(pair.first);
            }
        }
        
        std::vector<VType> result;
        int visitedCount = 0;
        
        while (!zeroInDegreeQueue.empty()) {
            VType current = zeroInDegreeQueue.front();
            zeroInDegreeQueue.pop();
            result.push_back(current);
            visitedCount++;
            
            Vertex<VType>* vertex = getVertex(current);
            
            for (const auto& edge : vertex->getEdges()) {
                if (edge->getFrom() != current) {
                    continue;
                }
                
                VType neighbor = edge->getTo();
                inDegree[neighbor]--;
                
                if (inDegree[neighbor] == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }
        
        if (visitedCount != vertices.size()) {
            graph_errors_detection(GraphError::CYCLE_DETECTED);
            throw GraphException(GraphError::CYCLE_DETECTED);
        }
        
        return result;
    }

    //удаление:

    void removeEdge(VType from, VType to) {
        auto vFrom = vertices.find(from);
        auto vTo = vertices.find(to);
        
        if (vFrom == vertices.end() || vTo == vertices.end()) {
            graph_errors_detection(GraphError::VERTEX_NOT_FOUND);
            throw GraphException(GraphError::VERTEX_NOT_FOUND);
        }
        
        Edge<VType, WType>* edgeToRemove = nullptr;
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            Edge<VType, WType>* edge = *it;
            
            if (edge->isDirected()) {
                if (edge->getFrom() == from && edge->getTo() == to) {
                    edgeToRemove = edge;
                    edges.erase(it);
                    break;
                }
            } else {
                if ((edge->getFrom() == from && edge->getTo() == to) ||
                    (edge->getFrom() == to && edge->getTo() == from)) {
                    edgeToRemove = edge;
                    edges.erase(it);
                    break;
                }
            }
        }
        
        if (!edgeToRemove) {
            graph_errors_detection(GraphError::INVALID_ARGUMENT, "Edge not found");
            throw GraphException(GraphError::INVALID_ARGUMENT, "Edge not found");
        }
    
        delete edgeToRemove;
    }

    
    void removeVertex(VType id) {
        auto it = vertices.find(id);
        if (it == vertices.end()) {
            graph_errors_detection(GraphError::VERTEX_NOT_FOUND);
            throw GraphException(GraphError::VERTEX_NOT_FOUND);
        }
        
        Vertex<VType>* vertexToRemove = it->second;
        
        
        std::vector<Edge<VType, WType>*> edgesToRemove;
        
        for (auto edgeIt = edges.begin(); edgeIt != edges.end();) {
            Edge<VType, WType>* edge = *edgeIt;
            
            if (edge->getFrom() == id || edge->getTo() == id) {
                VType otherVertexId = (edge->getFrom() == id) ? edge->getTo() : edge->getFrom();
                auto otherVertex = vertices.find(otherVertexId);
                otherVertex->second->removeEdge(edge);
                
                edgesToRemove.push_back(edge);
                edgeIt = edges.erase(edgeIt);
            } else {
                ++edgeIt;
            }
        }
        
        for (auto edge : edgesToRemove) {
            delete edge;
        }
        
        delete vertexToRemove;
        vertices.erase(it);
    }



};

#endif