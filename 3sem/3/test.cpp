#include <iostream>
#include <cassert>
#include <vector>
#include <utility>
#include "graph.hpp"
#include "taskDependency.hpp"

using namespace std;

void testGraphCreation() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 3);
    graph.addEdge(1, 3, 10);
    
    assert(graph.getVertexCount() == 3);
    assert(graph.getEdgeCount() == 3);
    assert(graph.hasEdge(1, 2) == true);
    assert(graph.hasEdge(2, 3) == true);
    assert(graph.hasEdge(1, 3) == true);
    assert(graph.hasEdge(3, 1) == false);
    
    cout << "testGraphCreation passed" << endl;
}

void testUndirectedGraph() {
    Graph<int> graph(UNDIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 3);
    
    assert(graph.getEdgeCount() == 2);
    assert(graph.hasEdge(1, 2) == true);
    assert(graph.hasEdge(2, 1) == true);
    assert(graph.hasEdge(2, 3) == true);
    assert(graph.hasEdge(3, 2) == true);
    
    cout << "testUndirectedGraph passed" << endl;
}

void testRemoveVertex() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 3);
    graph.addEdge(1, 3, 10);
    
    assert(graph.getVertexCount() == 3);
    assert(graph.getEdgeCount() == 3);
    
    graph.removeVertex(2);
    
    assert(graph.getVertexCount() == 2);
    assert(graph.getEdgeCount() == 1);
    assert(graph.hasEdge(1, 3) == true);
    assert(graph.hasEdge(1, 2) == false);
    
    cout << "testRemoveVertex passed" << endl;
}

void testRemoveEdge() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 3);
    graph.addEdge(1, 3, 10);
    
    assert(graph.getEdgeCount() == 3);
    
    graph.removeEdge(1, 2);
    
    assert(graph.getEdgeCount() == 2);
    assert(graph.hasEdge(1, 2) == false);
    assert(graph.hasEdge(2, 3) == true);
    assert(graph.hasEdge(1, 3) == true);
    
    cout << "testRemoveEdge passed" << endl;
}

void testDFS() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);
    graph.addVertex(5);
    
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);
    graph.addEdge(3, 5, 1);
    
    vector<int> dfsResult = graph.dfs(1);
    
    assert(dfsResult.size() == 5);
    assert(dfsResult[0] == 1);
    
    cout << "testDFS passed" << endl;
}

void testShortestPath() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);
    
    graph.addEdge(1, 2, 5);
    graph.addEdge(1, 3, 10);
    graph.addEdge(2, 4, 2);
    graph.addEdge(3, 4, 1);
    
    vector<int> path = graph.shortestPath(1, 4);
    
    assert(path.size() == 3);
    assert(path[0] == 1);
    assert(path[1] == 2);
    assert(path[2] == 4);
    
    path = graph.shortestPath(1, 3);
    
    assert(path.size() == 2);
    assert(path[0] == 1);
    assert(path[1] == 3);
    
    cout << "testShortestPath passed" << endl;
}

void testTopologicalSort() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);
    graph.addVertex(5);
    
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);
    graph.addEdge(3, 4, 1);
    graph.addEdge(4, 5, 1);
    
    vector<int> sorted = graph.topologicalSort();
    
    assert(sorted.size() == 5);
    
    int index1 = -1, index2 = -1, index4 = -1, index5 = -1;
    for(size_t i = 0; i < sorted.size(); i++) {
        if(sorted[i] == 1) index1 = i;
        if(sorted[i] == 2) index2 = i;
        if(sorted[i] == 4) index4 = i;
        if(sorted[i] == 5) index5 = i;
    }
    
    assert(index1 < index2);
    assert(index2 < index4);
    assert(index4 < index5);
    
    cout << "testTopologicalSort passed" << endl;
}

void testIsConnected() {
    Graph<int> undirectedGraph(UNDIRECTED);
    
    undirectedGraph.addVertex(1);
    undirectedGraph.addVertex(2);
    undirectedGraph.addVertex(3);
    
    undirectedGraph.addEdge(1, 2, 1);
    undirectedGraph.addEdge(2, 3, 1);
    
    assert(undirectedGraph.isConnected() == true);
    
    Graph<int> disconnectedGraph(UNDIRECTED);
    disconnectedGraph.addVertex(1);
    disconnectedGraph.addVertex(2);
    disconnectedGraph.addVertex(3);
    
    disconnectedGraph.addEdge(1, 2, 1);
    
    assert(disconnectedGraph.isConnected() == false);
    
    cout << "testIsConnected passed" << endl;
}

void testStringGraph() {
    Graph<string> graph(DIRECTED);
    
    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");
    
    graph.addEdge("A", "B", 5);
    graph.addEdge("B", "C", 3);
    
    assert(graph.getVertexCount() == 3);
    assert(graph.getEdgeCount() == 2);
    assert(graph.hasEdge("A", "B") == true);
    assert(graph.hasEdge("B", "C") == true);
    
    cout << "testStringGraph passed" << endl;
}

void testTaskDependencyOptimizer() {
    TaskDependencyOptimizer<string> optimizer;
    
    DependentTask<string>* task1 = new DependentTask<string>("A", 5);
    DependentTask<string>* task2 = new DependentTask<string>("B", 3);
    DependentTask<string>* task3 = new DependentTask<string>("C", 7);
    
    optimizer.addTask(task1);
    optimizer.addTask(task2);
    optimizer.addTask(task3);
    
    optimizer.addDependency("A", "B");
    optimizer.addDependency("A", "C");
    
    int minTime = optimizer.calculateMinProjectTime();
    
    assert(minTime == 12);
    
    vector<string> order = optimizer.getTaskOrder();
    
    assert(order.size() == 3);
    
    cout << "testTaskDependencyOptimizer passed" << endl;
}

void testComplexTaskDependencies() {
    TaskDependencyOptimizer<string> optimizer;
    
    DependentTask<string>* task1 = new DependentTask<string>("A", 3);
    DependentTask<string>* task2 = new DependentTask<string>("B", 4);
    DependentTask<string>* task3 = new DependentTask<string>("C", 2);
    DependentTask<string>* task4 = new DependentTask<string>("D", 5);
    DependentTask<string>* task5 = new DependentTask<string>("E", 6);
    
    optimizer.addTask(task1);
    optimizer.addTask(task2);
    optimizer.addTask(task3);
    optimizer.addTask(task4);
    optimizer.addTask(task5);
    
    optimizer.addDependency("A", "B");
    optimizer.addDependency("A", "C");
    optimizer.addDependency("B", "D");
    optimizer.addDependency("C", "D");
    optimizer.addDependency("D", "E");
    
    int minTime = optimizer.calculateMinProjectTime();
    
    assert(minTime == 18);
    
    cout << "testComplexTaskDependencies passed" << endl;
}

void testGraphExceptions() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    
    try {
        graph.addVertex(1);
        assert(false && "Should have thrown duplicate vertex exception");
    } catch(const GraphException& e) {
    }
    
    try {
        graph.addEdge(1, 2, 5);
        assert(false && "Should have thrown vertex not found exception");
    } catch(const GraphException& e) {
    }
    
    graph.addVertex(2);
    graph.addEdge(1, 2, 5);
    
    try {
        graph.addEdge(1, 2, 3);
        assert(false && "Should have thrown duplicate edge exception");
    } catch(const GraphException& e) {
    }
    
    cout << "testGraphExceptions passed" << endl;
}

void testTaskOptimizerExceptions() {
    TaskDependencyOptimizer<string> optimizer;
    
    DependentTask<string>* task1 = new DependentTask<string>("A", 5);
    
    optimizer.addTask(task1);
    
    try {
        optimizer.addTask(task1);
        assert(false && "Should have thrown duplicate task exception");
    } catch(const GraphException& e) {
    }
    
    try {
        optimizer.addDependency("A", "B");
        assert(false && "Should have thrown task not found exception");
    } catch(const GraphException& e) {
    }
    
    cout << "testTaskOptimizerExceptions passed" << endl;
}

void testEdgeWeights() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    
    graph.addEdge(1, 2, 5);
    
    try {
        graph.addEdge(2, 3, -1);
        assert(false && "Should have thrown invalid weight exception");
    } catch(const GraphException& e) {
    }
    
    cout << "testEdgeWeights passed" << endl;
}


void testGraphMove() {
    Graph<int> graph1(DIRECTED);
    
    graph1.addVertex(1);
    graph1.addVertex(2);
    graph1.addEdge(1, 2, 5);
    
    Graph<int> graph2(std::move(graph1));
    
    assert(graph2.getVertexCount() == 2);
    assert(graph2.getEdgeCount() == 1);
    
    cout << "testGraphMove passed" << endl;
}

void testVertexAdjacency() {
    Graph<int> graph(DIRECTED);
    
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    
    graph.addEdge(1, 2, 5);
    graph.addEdge(1, 3, 3);
    
    auto vertex = graph.getVertex(1);
    auto adjacent = vertex->getAdjacentVertices();
    
    assert(adjacent.size() == 2);
    
    cout << "testVertexAdjacency passed" << endl;
}

int main() {
    cout << "Running all tests..." << endl;
    
    testGraphCreation();
    testUndirectedGraph();
    testRemoveVertex();
    testRemoveEdge();
    testDFS();
    testShortestPath();
    testTopologicalSort();
    testIsConnected();
    testStringGraph();
    testTaskDependencyOptimizer();
    testComplexTaskDependencies();
    testGraphExceptions();
    testTaskOptimizerExceptions();
    testEdgeWeights();
    testGraphMove();
    testVertexAdjacency();
    
    cout << "All tests passed successfully!" << endl;
    
    return 0;
}