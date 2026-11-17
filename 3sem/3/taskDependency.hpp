#ifndef TASK_DEPENDENCY_HPP
#define TASK_DEPENDENCY_HPP

#include "graph.hpp"
#include "errors.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <random> 
#include <ctime>

template<typename VType>
class DependentTask {
private:
    VType id;
    size_t time;
    
public:
    DependentTask(VType id, size_t time) : id(id), time(time) {
        if (time <= 0) {
            throw GraphException(GraphError::INVALID_DURATION);
        }
    }
    
    VType getId() const { return id; }
    int getTime() const { return time; }
};

template<typename VType>
class TaskDependencyOptimizer {
private:
    Graph<VType, int> dependencyGraph;
    std::map<VType, DependentTask<VType>*> tasks;
    std::map<VType, std::vector<VType>> predecessors;
    std::map<VType, int> earlyStart;
    std::map<VType, int> earlyFinish;
    
public:
    TaskDependencyOptimizer() : dependencyGraph(DIRECTED) {}
    
    ~TaskDependencyOptimizer() {
        for (auto& pair : tasks) delete pair.second;
    }
    
    void addTask(DependentTask<VType>* task) {
        if (!task) {
            throw GraphException(GraphError::NULL_VERTEX);
        }
        
        VType taskId = task->getId();
        if (tasks.find(taskId) != tasks.end()) {
            throw GraphException(GraphError::DUPLICATE_TASK);
        }
        
        tasks[taskId] = task;
        dependencyGraph.addVertex(taskId);
    }
    
    void addDependency(VType fromTaskId, VType toTaskId) {
        if (tasks.find(fromTaskId) == tasks.end() || tasks.find(toTaskId) == tasks.end()) {
            throw GraphException(GraphError::TASK_NOT_FOUND);
        }
        
        if (dependencyGraph.hasEdge(fromTaskId, toTaskId)) {
            throw GraphException(GraphError::INVALID_DEPENDENCY);
        }
        
        dependencyGraph.addEdge(fromTaskId, toTaskId, tasks[fromTaskId]->getTime());
        predecessors[toTaskId].push_back(fromTaskId);
    }
    
    int calculateMinProjectTime() {
        std::vector<VType> taskOrder = dependencyGraph.topologicalSort();
        
        if (taskOrder.empty()) {
            return 0;
        }
        
        for (const auto& pair : tasks) {
            earlyStart[pair.first] = 0;
            earlyFinish[pair.first] = 0;
        }
        
        for (VType taskId : taskOrder) {
            int maxPredFinish = 0;
            
            if (predecessors.find(taskId) != predecessors.end()) {
                for (VType predId : predecessors[taskId]) {
                    if (earlyFinish[predId] > maxPredFinish) {
                        maxPredFinish = earlyFinish[predId];
                    }
                }
            }
            
            earlyStart[taskId] = maxPredFinish;
            earlyFinish[taskId] = earlyStart[taskId] + tasks[taskId]->getTime();
        }
        
        int projectFinish = 0;
        for (const auto& pair : earlyFinish) {
            if (pair.second > projectFinish) {
                projectFinish = pair.second;
            }
        }
        
        return projectFinish;
    }
    
    std::vector<VType> getTaskOrder() {
        return dependencyGraph.topologicalSort();
    }
//вывод менеджер задач которое будет говорить какие в текущий момент выполняются задачи и в какой момент какая завершилась
//рандом генерация

    void generateRandomTasks(int numTasks, int minTime, int maxTime) {
        srand(time(NULL));
        
        for (auto& pair : tasks) delete pair.second;
        tasks.clear();
        dependencyGraph = Graph<VType, int>(DIRECTED);
        predecessors.clear();
        earlyStart.clear();
        earlyFinish.clear();
        
        for (int i = 0; i < numTasks; ++i) {
            std::string taskId = "Task" + std::to_string(i + 1);
            
            int time = minTime + (rand() % (maxTime - minTime + 1));
            
            DependentTask<VType>* task = new DependentTask<VType>(static_cast<VType>(taskId), time);
            try {
                addTask(task);
            } catch (const GraphException& e) {
                delete task;
                continue;
            }
        }
        
        std::vector<VType> taskIds;
        for (const auto& pair : tasks) {
            taskIds.push_back(pair.first);
        }
        
        for (size_t i = 0; i < taskIds.size(); ++i) {
            VType currentTask = taskIds[i];
            
            int numDependencies = rand() % 3;
            
            for (int j = 0; j < numDependencies; ++j) {
                int randomIndex = rand() % taskIds.size();
                VType dependentTask = taskIds[randomIndex];
                
                if (dependentTask != currentTask) {
                    try {
                        addDependency(currentTask, dependentTask);
                    } catch (const GraphException& e) {}
                }
            }
        }
    }

    
};

#endif