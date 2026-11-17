#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib") // Для современного вида элементов управления

// Структура задачи
struct SimpleTask {
    std::string name;
    int duration;
    int startTime;
    int finishTime;
    bool isActive;
    bool isCompleted;
    std::vector<std::string> dependencies; // Зависимости
    
    SimpleTask(const std::string& n, int d) : name(n), duration(d), 
        startTime(0), finishTime(d), isActive(false), isCompleted(false) {}
};

// Константы для отрисовки
const int TASK_HEIGHT = 40;
const int TIME_SCALE = 15;
const COLORREF TASK_COLOR = RGB(70, 130, 180);
const COLORREF ACTIVE_COLOR = RGB(220, 20, 60);
const COLORREF DONE_COLOR = RGB(34, 139, 34);
const COLORREF BACKGROUND_COLOR = RGB(240, 240, 240);

// Глобальные переменные
extern std::vector<SimpleTask> tasks;
extern std::vector<std::string> taskNames; // Для выбора зависимостей
extern int currentTime;
extern bool isRunning;
extern int speed; // миллисекунды на единицу времени

extern HWND hMainWnd;
extern HWND hTaskList;
extern HWND hAddButton, hRandomButton, hStartButton, hResetButton;
extern HWND hTaskNameEdit, hDurationEdit, hStatusText;
extern HWND hTimeSlider;
extern HWND hDependencyList;
extern HWND hDeleteButton;

// Прототипы функций
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void UpdateTaskList();
void UpdateDependencyList();
void DrawTimeline(HDC hdc);
void DrawTask(HDC hdc, const SimpleTask& task, int y);
void CalculateTimes();
void AddTask(const std::string& name, int duration);
void AddRandomTasks(int count);
void UpdateStatus(const char* text);
void ClearDependencies();
void RunTaskManagerInterface();