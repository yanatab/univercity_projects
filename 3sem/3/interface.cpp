#include "interface.hpp"

std::vector<SimpleTask> tasks;
std::vector<std::string> taskNames;
int currentTime = 0;
bool isRunning = false;
int speed = 1000;

HWND hMainWnd;
HWND hTaskList;
HWND hAddButton, hRandomButton, hStartButton, hResetButton;
HWND hTaskNameEdit, hDurationEdit, hStatusText;
HWND hTimeSlider;
HWND hDependencyList;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HDC hdc;
    static PAINTSTRUCT ps;
    static RECT rect;
    
    switch (msg) {
    case WM_CREATE:
        CreateControls(hwnd);
        break;
        
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        DrawTimeline(hdc);
        EndPaint(hwnd, &ps);
        break;
        
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        
        if (id == 1) { 
            char name[100];
            GetWindowText(hTaskNameEdit, name, 100);
            
            char durText[20];
            GetWindowText(hDurationEdit, durText, 20);
            int duration = atoi(durText);
            
            if (strlen(name) > 0 && duration > 0) {
                AddTask(name, duration);
                SetWindowText(hTaskNameEdit, "");
                SetWindowText(hDurationEdit, "");
                UpdateTaskList();
                UpdateDependencyList();
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateStatus("Task added");
            }
        }
        else if (id == 2) {
            AddRandomTasks(5 + rand() % 6);
            UpdateTaskList();
            UpdateDependencyList();
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateStatus("Random tasks added");
        }
        else if (id == 3) {
            if (!isRunning) {
                isRunning = true;
                SetTimer(hwnd, 1, speed, NULL);
                SetWindowText(hStartButton, "Stop");
            } else {
                isRunning = false;
                KillTimer(hwnd, 1);
                SetWindowText(hStartButton, "Start");
            }
        }
        else if (id == 4) {
            isRunning = false;
            KillTimer(hwnd, 1);
            currentTime = 0;
            SetWindowText(hStartButton, "Start");
            
            char timeText[50];
            sprintf(timeText, "Current Time: %d", currentTime);
            SetWindowText(hStatusText, timeText);
            
            SetScrollPos(hTimeSlider, SB_CTL, 0, TRUE);
            
            for (auto& task : tasks) {
                task.isActive = false;
                task.isCompleted = false;
            }
            
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateStatus("Reset");
        }
        else if (id == 5) { 
            int taskIndex = SendMessage(hTaskList, LB_GETCURSEL, 0, 0);
            int depIndex = SendMessage(hDependencyList, LB_GETCURSEL, 0, 0);
            
            if (taskIndex != LB_ERR && depIndex != LB_ERR) {
                char taskName[100], depName[100];
                SendMessage(hTaskList, LB_GETTEXT, taskIndex, (LPARAM)taskName);
                SendMessage(hDependencyList, LB_GETTEXT, depIndex, (LPARAM)depName);
                
                std::string taskNameStr = taskName;
                std::string depNameStr = depName;
                
                size_t colonPos = taskNameStr.find(':');
                if (colonPos != std::string::npos) {
                    taskNameStr = taskNameStr.substr(0, colonPos);
                }
                
                if (taskNameStr == depNameStr) {
                    MessageBox(hwnd, "Cannot add self-dependency!", "Error", MB_OK | MB_ICONWARNING);
                    break;
                }
                
               bool taskFound = false;
                for (auto& task : tasks) {
                    if (task.name == taskNameStr) {
                        bool alreadyExists = false;
                        for (const auto& dep : task.dependencies) {
                            if (dep == depNameStr) {
                                alreadyExists = true;
                                break;
                            }
                        }
                        
                        if (!alreadyExists) {
                            bool depExists = false;
                            for (const auto& otherTask : tasks) {
                                if (otherTask.name == depNameStr) {
                                    depExists = true;
                                    break;
                                }
                            }
                            
                            if (depExists) {
                                task.dependencies.push_back(depNameStr);
                                
                                CalculateTimes();
                                UpdateTaskList();
                                InvalidateRect(hwnd, NULL, TRUE);
                                
                                char statusMsg[256];
                                sprintf(statusMsg, "Dependency added: %s -> %s", depNameStr.c_str(), taskNameStr.c_str());
                                UpdateStatus(statusMsg);
                            } else {
                                MessageBox(hwnd, "Dependency task not found!", "Error", MB_OK | MB_ICONWARNING);
                            }
                        } else {
                            MessageBox(hwnd, "Dependency already exists!", "Warning", MB_OK | MB_ICONINFORMATION);
                        }
                        taskFound = true;
                        break;
                    }
                }
                
                if (!taskFound) {
                    MessageBox(hwnd, "Task not found!", "Error", MB_OK | MB_ICONWARNING);
                }
            } else {
                MessageBox(hwnd, "Please select a task and a dependency!", "Warning", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        else if (id == 6) { 
            ClearDependencies();
            CalculateTimes();
            UpdateTaskList();
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateStatus("Dependencies cleared");
        }
        
        else if (id == 7) { 
            int index = SendMessage(hTaskList, LB_GETCURSEL, 0, 0);
            if (index != LB_ERR) {
                tasks.erase(tasks.begin() + index);
                UpdateTaskList();
                UpdateDependencyList();
                CalculateTimes();
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateStatus("Task deleted");
            }
        }
        else if (HIWORD(wParam) == LBN_SELCHANGE) {
            if ((HWND)lParam == hTaskList) {
                int index = SendMessage(hTaskList, LB_GETCURSEL, 0, 0);
                if (index != LB_ERR) {
                    char taskName[100];
                    SendMessage(hTaskList, LB_GETTEXT, index, (LPARAM)taskName);
                    
                    for (const auto& task : tasks) {
                        if (task.name == taskName) {
                            char status[200];
                            sprintf(status, "Task: %s, Duration: %d, Start: %d, Finish: %d", 
                                    task.name.c_str(), task.duration, task.startTime, task.finishTime);
                            
                            if (!task.dependencies.empty()) {
                                strcat(status, ", Depends on: ");
                                for (size_t i = 0; i < task.dependencies.size(); i++) {
                                    if (i > 0) strcat(status, ", ");
                                    strcat(status, task.dependencies[i].c_str());
                                }
                            }
                            UpdateStatus(status);
                            break;
                        }
                    }
                }
            }
        }
        break;
    }
        
    case WM_HSCROLL:
        if ((HWND)lParam == hTimeSlider) {
            int scrollCode = LOWORD(wParam);
            int pos = HIWORD(wParam);
            
            if (!isRunning) {
                currentTime = GetScrollPos(hTimeSlider, SB_CTL);
                
                switch (scrollCode) {
                    case SB_LINELEFT:    currentTime--; break;
                    case SB_LINERIGHT:   currentTime++; break;
                    case SB_PAGELEFT:    currentTime -= 5; break;
                    case SB_PAGERIGHT:   currentTime += 5; break;
                    case SB_THUMBPOSITION: currentTime = pos; break;
                    case SB_THUMBTRACK:   currentTime = pos; break;
                }
                
                if (currentTime < 0) currentTime = 0;
                if (currentTime > 100) currentTime = 100;
                
                SetScrollPos(hTimeSlider, SB_CTL, currentTime, TRUE);
                
                for (auto& task : tasks) {
                    task.isActive = (currentTime >= task.startTime && currentTime < task.finishTime);
                    task.isCompleted = (currentTime >= task.finishTime);
                }
                
                char timeText[50];
                sprintf(timeText, "Current Time: %d", currentTime);
                SetWindowText(hStatusText, timeText);
                
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
        break;
        
    case WM_TIMER:
        {
            currentTime++;
            
            SetScrollPos(hTimeSlider, SB_CTL, currentTime, TRUE);
            
            for (auto& task : tasks) {
                task.isActive = (currentTime >= task.startTime && currentTime < task.finishTime);
                task.isCompleted = (currentTime >= task.finishTime);
            }
            
            char timeText[50];
            sprintf(timeText, "Current Time: %d", currentTime);
            SetWindowText(hStatusText, timeText);
            
            InvalidateRect(hwnd, NULL, TRUE);
            
            bool allDone = true;
            for (const auto& task : tasks) {
                if (!task.isCompleted) {
                    allDone = false;
                    break;
                }
            }
            
            if (allDone && tasks.size() > 0) {
                isRunning = false;
                KillTimer(hwnd, 1);
                SetWindowText(hStartButton, "Start");
                UpdateStatus("All tasks completed!");
            }
        }
        break;
        
    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        break;
        
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    
    return 0;
}

void CreateControls(HWND hwnd) {
    int y = 10;
    int btnWidth = 120;
    int btnHeight = 25;
    
    hAddButton = CreateWindow("BUTTON", "Add Task", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, y, btnWidth, btnHeight, hwnd, (HMENU)1, NULL, NULL);
    
    hRandomButton = CreateWindow("BUTTON", "Random Tasks", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        140, y, btnWidth, btnHeight, hwnd, (HMENU)2, NULL, NULL);
    
    hStartButton = CreateWindow("BUTTON", "Start", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        270, y, btnWidth, btnHeight, hwnd, (HMENU)3, NULL, NULL);
    
    hResetButton = CreateWindow("BUTTON", "Reset", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        400, y, btnWidth, btnHeight, hwnd, (HMENU)4, NULL, NULL);
    
    y += btnHeight + 15;
    
    CreateWindow("BUTTON", "Delete Task", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        530, 10, 120, 25, hwnd, (HMENU)7, NULL, NULL);


    CreateWindow("STATIC", "Task Name:", WS_CHILD | WS_VISIBLE,
        10, y, 70, 20, hwnd, NULL, NULL, NULL);
    
    hTaskNameEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
        90, y, 150, 20, hwnd, NULL, NULL, NULL);
    
    y += 25;
    
    CreateWindow("STATIC", "Duration:", WS_CHILD | WS_VISIBLE,
        10, y, 80, 20, hwnd, NULL, NULL, NULL);
    
    hDurationEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
        90, y, 50, 20, hwnd, NULL, NULL, NULL);
    
    y += 40;
    
    CreateWindow("STATIC", "Task List:", WS_CHILD | WS_VISIBLE,
        10, y, 100, 20, hwnd, NULL, NULL, NULL);
    
    hTaskList = CreateWindow("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | 
        LBS_NOINTEGRALHEIGHT | WS_VSCROLL | LBS_NOTIFY,
        10, y + 20, 250, 150, hwnd, NULL, NULL, NULL);
    
    y += 180;
    
    CreateWindow("STATIC", "All Tasks:", WS_CHILD | WS_VISIBLE,
        10, y, 100, 20, hwnd, NULL, NULL, NULL);
    
    hDependencyList = CreateWindow("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | 
        LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
        10, y + 20, 250, 100, hwnd, NULL, NULL, NULL);
    
    y += 130;
    
    CreateWindow("BUTTON", "Add Dependency", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, y, 120, 25, hwnd, (HMENU)5, NULL, NULL);
    
    CreateWindow("BUTTON", "Clear All Deps", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        140, y, 120, 25, hwnd, (HMENU)6, NULL, NULL);
    
    y += 40;

    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(240, 240, 240)));
    
    CreateWindow("STATIC", "Time:", WS_CHILD | WS_VISIBLE,
        10, y, 50, 20, hwnd, NULL, NULL, NULL);
    
    hTimeSlider = CreateWindow("SCROLLBAR", "", 
        WS_CHILD | WS_VISIBLE | SBS_HORZ,
        60, y - 5, 200, 30, hwnd, NULL, NULL, NULL);
    
    SetScrollRange(hTimeSlider, SB_CTL, 0, 100, TRUE);
    SetScrollPos(hTimeSlider, SB_CTL, 0, TRUE);
    
    y += 50;
    
    hStatusText = CreateWindow("STATIC", "Current Time: 0", 
        WS_CHILD | WS_VISIBLE,
        10, y, 300, 40, hwnd, NULL, NULL, NULL);
}

void UpdateTaskList() {
    SendMessage(hTaskList, LB_RESETCONTENT, 0, 0);
    
    for (const auto& task : tasks) {
        char text[200];
        sprintf(text, "%s: %d-%d (%d)", 
            task.name.c_str(), task.startTime, task.finishTime, task.duration);
        
        if (!task.dependencies.empty()) {
            strcat(text, " [");
            for (size_t i = 0; i < task.dependencies.size(); i++) {
                if (i > 0) strcat(text, ",");
                strcat(text, task.dependencies[i].c_str());
            }
            strcat(text, "]");
        }
        
        SendMessage(hTaskList, LB_ADDSTRING, 0, (LPARAM)text);
    }
}

void UpdateDependencyList() {
    SendMessage(hDependencyList, LB_RESETCONTENT, 0, 0);
    
    for (const auto& task : tasks) {
        SendMessage(hDependencyList, LB_ADDSTRING, 0, (LPARAM)task.name.c_str());
    }
}


void DrawTimeline(HDC hdc) {
    RECT rect;
    GetClientRect(hMainWnd, &rect);
    
    HBRUSH bgBrush = (HBRUSH)GetClassLongPtr(hMainWnd, GCLP_HBRBACKGROUND);
    if (!bgBrush) {
        bgBrush = (HBRUSH)(COLOR_WINDOW + 1);
    }
    FillRect(hdc, &rect, bgBrush);
    
    HBRUSH timelineBgBrush = CreateSolidBrush(RGB(240, 240, 240));
    RECT timelineRect = {300, rect.top, rect.right, rect.bottom};
    FillRect(hdc, &timelineRect, timelineBgBrush);
    DeleteObject(timelineBgBrush);
    
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    SetTextColor(hdc, RGB(0, 0, 120));
    TextOut(hdc, 300, 10, "", 35);
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);
    
    for (int t = 0; t <= 100; t += 10) {
        int x = 300 + t * TIME_SCALE;
        MoveToEx(hdc, x, 50, NULL);
        LineTo(hdc, x, rect.bottom - 50);
        
        char label[10];
        sprintf(label, "%d", t);
        TextOut(hdc, x - 5, rect.bottom - 40, label, strlen(label));
    }
    
    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);
    
    if (currentTime > 0) {
        int x = 300 + currentTime * TIME_SCALE;
        HPEN timePen = CreatePen(PS_SOLID, 2, RGB(220, 20, 60));
        SelectObject(hdc, timePen);
        
        MoveToEx(hdc, x, 50, NULL);
        LineTo(hdc, x, rect.bottom - 50);
        
        SelectObject(hdc, oldPen);
        DeleteObject(timePen);
        
        HBRUSH timeBrush = CreateSolidBrush(RGB(220, 20, 60));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, timeBrush);
        
        Ellipse(hdc, x - 5, 45, x + 5, 55);
        
        SelectObject(hdc, oldBrush);
        DeleteObject(timeBrush);
    }
    
    int y = 80;
    for (const auto& task : tasks) {
        DrawTask(hdc, task, y);
        y += 50;
    }
    
    HPEN arrowPen = CreatePen(PS_SOLID, 1, RGB(28, 28, 17));
    SelectObject(hdc, arrowPen);
    
    for (const auto& task : tasks) {
        int taskY = 80 + (&task - &tasks[0]) * 50;
        int taskStartX = 300 + task.startTime * TIME_SCALE;
        
        for (const auto& depName : task.dependencies) {
            for (const auto& depTask : tasks) {
                if (depTask.name == depName) {
                    int depY = 80 + (&depTask - &tasks[0]) * 50;
                    int depFinishX = 300 + depTask.finishTime * TIME_SCALE;
                    
                    MoveToEx(hdc, depFinishX, depY + TASK_HEIGHT/2, NULL);
                    LineTo(hdc, taskStartX, taskY + TASK_HEIGHT/2);
                    
                    LineTo(hdc, taskStartX - 5, taskY + TASK_HEIGHT/2 - 3);
                    MoveToEx(hdc, taskStartX, taskY + TASK_HEIGHT/2, NULL);
                    LineTo(hdc, taskStartX - 5, taskY + TASK_HEIGHT/2 + 3);
                    
                    break;
                }
            }
        }
    }
    
    DeleteObject(arrowPen);
    
    int legendY = rect.bottom - 120;
    
    HBRUSH planBrush = CreateSolidBrush(TASK_COLOR);
    RECT r1 = {300, legendY, 320, legendY + 15};
    FillRect(hdc, &r1, planBrush);
    DeleteObject(planBrush);
    TextOut(hdc, 325, legendY, "Planned", 7);
    
    HBRUSH activeBrush = CreateSolidBrush(ACTIVE_COLOR);
    RECT r2 = {300, legendY + 20, 320, legendY + 35};
    FillRect(hdc, &r2, activeBrush);
    DeleteObject(activeBrush);
    TextOut(hdc, 325, legendY + 20, "Active", 6);
    
    HBRUSH doneBrush = CreateSolidBrush(DONE_COLOR);
    RECT r3 = {300, legendY + 40, 320, legendY + 55};
    FillRect(hdc, &r3, doneBrush);
    DeleteObject(doneBrush);
    TextOut(hdc, 325, legendY + 40, "Completed", 9);
    
    }

void DrawTask(HDC hdc, const SimpleTask& task, int y) {
    int x = 300 + task.startTime * TIME_SCALE;
    int width = task.duration * TIME_SCALE;
    if (width < 20) width = 20;
    
    COLORREF color;
    if (task.isCompleted) color = DONE_COLOR;
    else if (task.isActive) color = ACTIVE_COLOR;
    else color = TASK_COLOR;
    
    HBRUSH taskBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, taskBrush);
    HPEN borderPen = CreatePen(PS_SOLID, 2, RGB(30, 30, 30));
    HPEN oldPen = (HPEN)SelectObject(hdc, borderPen);
    
    Rectangle(hdc, x, y, x + width, y + TASK_HEIGHT);
    
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(taskBrush);
    DeleteObject(borderPen);
    
    HFONT hFont = CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    std::string text = task.name;
    RECT textRect = {x, y, x + width, y + TASK_HEIGHT};
    DrawText(hdc, text.c_str(), text.length(), &textRect, 
        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    
 
    HFONT smallFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, smallFont);
    
    SetTextColor(hdc, RGB(54, 48, 48));
    char timeText[20];
    sprintf(timeText, "%d-%d", task.startTime, task.finishTime);
    
    textRect.top = y + TASK_HEIGHT + 2;
    textRect.bottom = y + TASK_HEIGHT + 15;
    DrawText(hdc, timeText, strlen(timeText), &textRect, DT_CENTER);
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    DeleteObject(smallFont);
}

void CalculateTimes() {
    for (auto& task : tasks) {
        task.startTime = 0;
        task.finishTime = task.duration;
    }
    
    bool changed;
    do {
        changed = false;
        
        for (auto& task : tasks) {
            int maxDependencyFinish = 0;
            
            for (const auto& depName : task.dependencies) {
                for (const auto& depTask : tasks) {
                    if (depTask.name == depName) {
                        if (depTask.finishTime > maxDependencyFinish) {
                            maxDependencyFinish = depTask.finishTime;
                        }
                        break;
                    }
                }
            }
            
            if (maxDependencyFinish > task.startTime) {
                task.startTime = maxDependencyFinish;
                task.finishTime = task.startTime + task.duration;
                changed = true;
            }
        }
    } while (changed);
    
}

void AddTask(const std::string& name, int duration) {
    tasks.push_back(SimpleTask(name, duration));
    CalculateTimes();
}
void AddRandomTasks(int count) {
    srand((unsigned int)time(NULL));
    std::vector<std::string> names = {
        "Task"
    };
    
    tasks.clear();
    
    for (int i = 0; i < count; i++) {
        std::string name = names[rand() % names.size()];
        name += " " + std::to_string(i + 1);
        int duration = 1 + rand() % 10;
        AddTask(name, duration);
    }
    
    for (size_t i = 0; i < tasks.size(); i++) {
        for (size_t j = 0; j < i; j++) { 
            if (rand() % 100 < 30) { 
                bool alreadyExists = false;
                for (const auto& dep : tasks[i].dependencies) {
                    if (dep == tasks[j].name) {
                        alreadyExists = true;
                        break;
                    }
                }
                if (!alreadyExists) {
                    tasks[i].dependencies.push_back(tasks[j].name);
                }
            }
        }
    }
    
    CalculateTimes();
}

void ClearDependencies() {
    for (auto& task : tasks) {
        task.dependencies.clear();
    }
}

void UpdateStatus(const char* text) {
    char title[200];
    sprintf(title, "Task Manager - Visual (with Dependencies) | %s", text);
    SetWindowText(hMainWnd, title);
}

void RunTaskManagerInterface() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "TaskManager";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    hMainWnd = CreateWindow("TaskManager", "Task Manager - Visual (with Dependencies)", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hMainWnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(hMainWnd, SW_SHOW);
    UpdateWindow(hMainWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
