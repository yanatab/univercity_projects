#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <stdexcept>
#include <string>

enum class GraphError {
    OK = 0,
    INVALID_VERTEX = 1,
    INVALID_ARGUMENT = 2,
    NULL_VERTEX = 3,
    CYCLE_DETECTED = 4,
    DUPLICATE_VERTEX = 5,
    DUPLICATE_EDGE = 6,
    VERTEX_NOT_FOUND = 7,
    INVALID_WEIGHT = 8,
    INVALID_DURATION = 9,
    DUPLICATE_TASK = 10,
    TASK_NOT_FOUND = 11,
    INVALID_DEPENDENCY = 12,
    TASK_CYCLE_DETECTED = 13,
    EMPTY_GRAPH = 14,
    GRAPH_NOT_UNDIRECTED = 15,
    GRAPH_NOT_DIRECTED = 16
};

void graph_errors_detection(GraphError error, const std::string& additional_info = "");

class GraphException : public std::exception {
private:
    GraphError error_code;
    std::string message;
    
public:
    GraphException(GraphError err, const std::string& msg = "") 
        : error_code(err), message(msg) {}
    
    GraphError getErrorCode() const { return error_code; }
    
    const char* what() const noexcept override {
        return message.c_str();
    }
    
    void print() const {
        graph_errors_detection(error_code, message);
    }
};

void graph_errors_detection(GraphError error, const std::string& additional_info) {
    std::string error_msg;
    
    switch (error) {
        case GraphError::OK:
            error_msg = "No errors.";
            break;
        case GraphError::INVALID_VERTEX:
            error_msg = "Invalid vertex.";
            break;
        case GraphError::INVALID_ARGUMENT:
            error_msg = "Invalid argument.";
            break;
        case GraphError::NULL_VERTEX:
            error_msg = "Null vertex.";
            break;
        case GraphError::CYCLE_DETECTED:
            error_msg = "Cycle detected.";
            break;
        case GraphError::DUPLICATE_VERTEX:
            error_msg = "Duplicate vertex.";
            break;
        case GraphError::DUPLICATE_EDGE:
            error_msg = "Duplicate edge.";
            break;
        case GraphError::VERTEX_NOT_FOUND:
            error_msg = "Vertex not found.";
            break;
        case GraphError::INVALID_WEIGHT:
            error_msg = "Invalid edge weight.";
            break;
        case GraphError::INVALID_DURATION:
            error_msg = "Invalid task duration.";
            break;
        case GraphError::DUPLICATE_TASK:
            error_msg = "Duplicate task.";
            break;
        case GraphError::TASK_NOT_FOUND:
            error_msg = "Task not found.";
            break;
        case GraphError::INVALID_DEPENDENCY:
            error_msg = "Invalid task dependency.";
            break;
        case GraphError::TASK_CYCLE_DETECTED:
            error_msg = "Cycle detected in task dependencies.";
            break;
        case GraphError::EMPTY_GRAPH:
            error_msg = "Empty graph.";
            break;
        case GraphError::GRAPH_NOT_UNDIRECTED:
            error_msg = "Graph is not undirected.";
            break;
        case GraphError::GRAPH_NOT_DIRECTED:
            error_msg = "Graph is not directed.";
            break;
        default:
            error_msg = "Unknown graph error.";
    }
    
    std::cout << "[Graph Error] " << error_msg;
    if (!additional_info.empty()) {
        std::cout << " Additional info: " << additional_info;
    }
    std::cout << std::endl;
}

#endif 