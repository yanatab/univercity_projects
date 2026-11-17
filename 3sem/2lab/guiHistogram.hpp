#ifndef GUI_HISTOGRAM_HPP
#define GUI_HISTOGRAM_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>
#include "histogram.hpp"
#include "IDictionary.hpp"

#define IDC_DATA_EDIT        1001
#define IDC_MIN_EDIT         1002
#define IDC_MAX_EDIT         1003
#define IDC_BINS_EDIT        1004
#define IDC_WIDTH_EDIT       1005
#define IDC_TYPE_COMBO       1006
#define IDC_ORIENTATION_COMBO 1007
#define IDC_BUILD_BTN        1008
#define IDC_HISTOGRAM_VIEW   1009
#define IDC_STATUS_LABEL     1010

class HistogramGUI {
private:
    HWND hwndMain;
    HWND hwndDataEdit;
    HWND hwndMinEdit;
    HWND hwndMaxEdit;
    HWND hwndBinsEdit;
    HWND hwndWidthEdit;
    HWND hwndTypeCombo;
    HWND hwndOrientationCombo;
    HWND hwndBuildBtn;
    HWND hwndHistogramView;
    HWND hwndStatusLabel;
    
    HINSTANCE hInstance;
    std::vector<int> currentData;
    std::pair<int, int> currentRange;
    int currentBins;
    int currentMaxWidth;
    bool useHashHistogram;
    bool useHorizontal;
    
    static const int HISTOGRAM_HEIGHT = 300;
    static const int HISTOGRAM_WIDTH = 600;
    
public:
    HistogramGUI(HINSTANCE hInst) : hInstance(hInst), 
                                   currentBins(10),
                                   currentMaxWidth(50),
                                   useHashHistogram(true),
                                   useHorizontal(true) {
        currentRange = std::make_pair(0, 100);
    }
    
    int Run() {
        WNDCLASSEXA wc = {0};
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wc.lpszClassName = "HistogramWindowClass";
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.lpszMenuName = NULL;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        
        if (!RegisterClassExA(&wc)) {
            MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }
        
        hwndMain = CreateWindowExA(
            0,
            "HistogramWindowClass",
            "Histogram Builder",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            NULL,
            NULL,
            hInstance,
            this
        );
        
        if (!hwndMain) {
            MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }
        
        CreateControls();
        ShowWindow(hwndMain, SW_SHOW);
        UpdateWindow(hwndMain);
        
        MSG msg = {0};
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        return (int)msg.wParam;
    }
    
private:
    void CreateControls() {
        CreateWindowA("STATIC", "Data (comma or space separated):", 
                     WS_VISIBLE | WS_CHILD, 20, 20, 250, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        CreateWindowA("STATIC", "Min value:", 
                     WS_VISIBLE | WS_CHILD, 20, 100, 150, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        CreateWindowA("STATIC", "Max value:", 
                     WS_VISIBLE | WS_CHILD, 20, 140, 150, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        CreateWindowA("STATIC", "Number of bins:", 
                     WS_VISIBLE | WS_CHILD, 200, 100, 120, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        CreateWindowA("STATIC", "Max width:", 
                     WS_VISIBLE | WS_CHILD, 200, 140, 120, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        CreateWindowA("STATIC", "Histogram type:", 
                     WS_VISIBLE | WS_CHILD, 350, 100, 120, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        CreateWindowA("STATIC", "Orientation:", 
                     WS_VISIBLE | WS_CHILD, 350, 140, 120, 20, 
                     hwndMain, NULL, hInstance, NULL);
        
        hwndDataEdit = CreateWindowA("EDIT", "2 15 19 2", 
                                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                    20, 45, 500, 25, 
                                    hwndMain, (HMENU)IDC_DATA_EDIT, hInstance, NULL);
        
        hwndMinEdit = CreateWindowA("EDIT", "1", 
                                   WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                   20, 120, 120, 25, 
                                   hwndMain, (HMENU)IDC_MIN_EDIT, hInstance, NULL);
        
        hwndMaxEdit = CreateWindowA("EDIT", "20", 
                                   WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                   20, 160, 120, 25, 
                                   hwndMain, (HMENU)IDC_MAX_EDIT, hInstance, NULL);
        
        hwndBinsEdit = CreateWindowA("EDIT", "5", 
                                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                    200, 120, 120, 25, 
                                    hwndMain, (HMENU)IDC_BINS_EDIT, hInstance, NULL);
        
        hwndWidthEdit = CreateWindowA("EDIT", "50", 
                                     WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                     200, 160, 120, 25, 
                                     hwndMain, (HMENU)IDC_WIDTH_EDIT, hInstance, NULL);
        
        hwndTypeCombo = CreateWindowA("COMBOBOX", NULL,
                                     WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP,
                                     350, 120, 150, 200,
                                     hwndMain, (HMENU)IDC_TYPE_COMBO, hInstance, NULL);
        
        SendMessageA(hwndTypeCombo, CB_ADDSTRING, 0, (LPARAM)"Hash Histogram");
        SendMessageA(hwndTypeCombo, CB_ADDSTRING, 0, (LPARAM)"Tree Histogram");
        SendMessageA(hwndTypeCombo, CB_SETCURSEL, 0, 0);
        
        hwndOrientationCombo = CreateWindowA("COMBOBOX", NULL,
                                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP,
                                            350, 160, 150, 200,
                                            hwndMain, (HMENU)IDC_ORIENTATION_COMBO, hInstance, NULL);
        
        SendMessageA(hwndOrientationCombo, CB_ADDSTRING, 0, (LPARAM)"Horizontal");
        SendMessageA(hwndOrientationCombo, CB_ADDSTRING, 0, (LPARAM)"Vertical");
        SendMessageA(hwndOrientationCombo, CB_SETCURSEL, 0, 0);
        
        hwndBuildBtn = CreateWindowA("BUTTON", "Build Histogram",
                                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP,
                                    550, 120, 200, 60,
                                    hwndMain, (HMENU)IDC_BUILD_BTN, hInstance, NULL);
        
        hwndHistogramView = CreateWindowA("STATIC", NULL,
                                         WS_VISIBLE | WS_CHILD | WS_BORDER,
                                         20, 200, HISTOGRAM_WIDTH, HISTOGRAM_HEIGHT,
                                         hwndMain, (HMENU)IDC_HISTOGRAM_VIEW, hInstance, NULL);
        
        hwndStatusLabel = CreateWindowA("STATIC", "Ready - Enter data and click 'Build Histogram'",
                                       WS_VISIBLE | WS_CHILD,
                                       20, 520, 600, 30,
                                       hwndMain, (HMENU)IDC_STATUS_LABEL, hInstance, NULL);
    }
    
    void SetStatus(const std::string& message) {
        SetWindowTextA(hwndStatusLabel, message.c_str());
    }
    
    void BuildHistogram() {
        try {
            char buffer[1024];
            GetWindowTextA(hwndDataEdit, buffer, 1024);
            currentData = ParseData(buffer);
            
            if (currentData.empty()) {
                SetStatus("Error: No valid data to build histogram");
                return;
            }
            
            GetWindowTextA(hwndMinEdit, buffer, 100);
            int minVal = atoi(buffer);
            
            GetWindowTextA(hwndMaxEdit, buffer, 100);
            int maxVal = atoi(buffer);
            
            bool autoAdjusted = false;
            if (minVal >= maxVal) {
                std::pair<int, int> range = CalculateDataRange();
                minVal = range.first;
                maxVal = range.second;
                
                SetWindowTextA(hwndMinEdit, std::to_string(minVal).c_str());
                SetWindowTextA(hwndMaxEdit, std::to_string(maxVal).c_str());
                
                SetStatus("Auto-adjusted range: " + std::to_string(minVal) + " to " + std::to_string(maxVal));
                autoAdjusted = true;
            }
            
            GetWindowTextA(hwndBinsEdit, buffer, 100);
            currentBins = atoi(buffer);
            if (currentBins <= 0) currentBins = 10;
            
            GetWindowTextA(hwndWidthEdit, buffer, 100);
            currentMaxWidth = atoi(buffer);
            if (currentMaxWidth <= 0) currentMaxWidth = 50;
            
            currentRange = std::make_pair(minVal, maxVal);
            
            int selectedType = SendMessageA(hwndTypeCombo, CB_GETCURSEL, 0, 0);
            useHashHistogram = (selectedType == 0);
            
            int selectedOrientation = SendMessageA(hwndOrientationCombo, CB_GETCURSEL, 0, 0);
            useHorizontal = (selectedOrientation == 0);
            
            std::string dataInfo = "Data: ";
            for (size_t i = 0; i < currentData.size(); ++i) {
                dataInfo += std::to_string(currentData[i]);
                if (i < currentData.size() - 1) dataInfo += ", ";
            }
            dataInfo += " (Total: " + std::to_string(currentData.size()) + " values)";
            
            if (autoAdjusted) {
                dataInfo += " [Range auto-adjusted]";
            }
            
            SetStatus(dataInfo);
            
            if (useHashHistogram) {
                HashHistogram<int> histogram(
                    [](int x) { return x; },
                    minVal, maxVal, currentBins, currentMaxWidth
                );
                
                auto histData = histogram.buildHistogram(currentData);
                DrawHistogram(histData, useHorizontal);
                
                auto keys = histData.keys();
                std::string histInfo = "Histogram built with " + std::to_string(keys.size()) + " bins. ";
                SetStatus(dataInfo + " | " + histInfo);
            } else {
                TreeHistogram<int> histogram(
                    [](int x) { return x; },
                    minVal, maxVal, currentBins, currentMaxWidth
                );
                
                auto histData = histogram.buildHistogram(currentData);
                DrawHistogram(histData, useHorizontal);
                
                auto keys = histData.keys();
                std::string histInfo = "Histogram built with " + std::to_string(keys.size()) + " bins. ";
                SetStatus(dataInfo + " | " + histInfo);
            }
            
        } catch (const std::exception& e) {
            std::string error = "Error: ";
            error += e.what();
            SetStatus(error);
        }
    }
    
    std::vector<int> ParseData(const char* dataStr) {
        std::vector<int> data;
        std::string input(dataStr);
        
        if (input.empty()) {
            return data;
        }
        
        std::replace(input.begin(), input.end(), ',', ' ');
        
        std::stringstream ss(input);
        std::string token;
        
        while (ss >> token) {
            try {
                token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
                token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
                
                if (!token.empty()) {
                    int value = std::stoi(token);
                    data.push_back(value);
                }
            } catch (const std::exception&) {
                continue;
            }
        }
        
        return data;
    }
    
    std::pair<int, int> CalculateDataRange() {
        if (currentData.empty()) {
            return std::make_pair(0, 100);
        }
        
        int minVal = currentData[0];
        int maxVal = currentData[0];
        
        for (size_t i = 1; i < currentData.size(); ++i) {
            int val = currentData[i];
            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
        
        int range = maxVal - minVal;
        if (range == 0) {
            minVal = minVal - 1;
            maxVal = maxVal + 1;
        }
        
        return std::make_pair(minVal, maxVal);
    }
    
    template<typename DictType>
    void DrawHistogram(const DictType& histogram, bool horizontal) {
        HDC hdc = GetDC(hwndHistogramView);
        
        RECT rect;
        GetClientRect(hwndHistogramView, &rect);
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        
        if (histogram.empty()) {
            TextOutA(hdc, 10, 10, "Histogram is empty", 18);
            ReleaseDC(hwndHistogramView, hdc);
            return;
        }
        
        auto keys = histogram.keys();
        if (keys.empty()) {
            ReleaseDC(hwndHistogramView, hdc);
            return;
        }
        
        std::sort(keys.begin(), keys.end());
        
        int maxCount = 0;
        int totalCount = 0;
        for (const auto& key : keys) {
            int count = histogram.at(key);
            totalCount += count;
            if (count > maxCount) maxCount = count;
        }
        
        if (maxCount == 0 || totalCount == 0) {
            TextOutA(hdc, 10, 10, "No data in histogram", 20);
            ReleaseDC(hwndHistogramView, hdc);
            return;
        }
        
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        int padding = 60;
        int bottomSpace = 40;
        int topSpace = 40;
        int leftSpace = 150;
        
        HPEN hAxisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN hBarPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hBarBrush = CreateSolidBrush(RGB(252, 136, 173));
        HBRUSH hTextBrush = CreateSolidBrush(RGB(0, 0, 0));
        HPEN hGridPen = CreatePen(PS_DOT, 1, RGB(200, 200, 200));
        
        HPEN hOldPen = (HPEN)SelectObject(hdc, hAxisPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hTextBrush);
        
        SetBkMode(hdc, TRANSPARENT);
        
        std::string title = "Histogram (Total: " + std::to_string(totalCount) + " values)";
        TextOutA(hdc, width/2 - 50, 10, title.c_str(), (int)title.length());
        
        if (horizontal) {
            int barHeight = 25;
            int spacing = 5;
            int chartHeight = (int)keys.size() * (barHeight + spacing);
            int startY = topSpace + 20;
            int maxBarWidth = width - leftSpace - 50;
            
            MoveToEx(hdc, leftSpace, startY, NULL);
            LineTo(hdc, leftSpace, startY + chartHeight);
            LineTo(hdc, width - 20, startY + chartHeight);
            
            SelectObject(hdc, hGridPen);
            for (int i = 1; i <= 5; ++i) {
                int gridX = leftSpace + (i * maxBarWidth) / 5;
                MoveToEx(hdc, gridX, startY, NULL);
                LineTo(hdc, gridX, startY + chartHeight);
            }
            
            SelectObject(hdc, hAxisPen);
            SelectObject(hdc, hBarPen);
            SelectObject(hdc, hBarBrush);
            
            for (size_t i = 0; i < keys.size(); ++i) {
                int y = startY + (int)i * (barHeight + spacing);
                int count = histogram.at(keys[i]);
                float percentage = (totalCount > 0) ? (100.0f * count / totalCount) : 0;
                
                int barWidth = (count * maxBarWidth) / maxCount;
                if (barWidth < 1) barWidth = 1;
                
                Rectangle(hdc, leftSpace, y, leftSpace + barWidth, y + barHeight);
                
                std::string label = keys[i];
                TextOutA(hdc, 10, y + 8, label.c_str(), (int)label.length());
                
                std::string countStr = std::to_string(count) + " (" + 
                                      std::to_string((int)percentage) + "%)";
                TextOutA(hdc, leftSpace + barWidth + 5, y + 8, countStr.c_str(), (int)countStr.length());
            }
            
            std::string xLabel = "Frequency (max: " + std::to_string(maxCount) + ")";
            TextOutA(hdc, width/2 - 50, startY + chartHeight + 20, xLabel.c_str(), (int)xLabel.length());
            
        } else {
            int barWidth = 30;
            int spacing = 10;
            int chartWidth = (int)keys.size() * (barWidth + spacing);
            int startX = (width - chartWidth) / 2;
            if (startX < leftSpace) startX = leftSpace;
            int maxBarHeight = height - bottomSpace - topSpace - 40;
            
            MoveToEx(hdc, startX, height - bottomSpace, NULL);
            LineTo(hdc, startX + chartWidth, height - bottomSpace);
            LineTo(hdc, startX + chartWidth, topSpace);
            
            SelectObject(hdc, hGridPen);
            for (int i = 1; i <= 5; ++i) {
                int gridY = height - bottomSpace - (i * maxBarHeight) / 5;
                MoveToEx(hdc, startX, gridY, NULL);
                LineTo(hdc, startX + chartWidth, gridY);
            }
            
            SelectObject(hdc, hAxisPen);
            SelectObject(hdc, hBarPen);
            SelectObject(hdc, hBarBrush);
            
            for (size_t i = 0; i < keys.size(); ++i) {
                int x = startX + (int)i * (barWidth + spacing);
                int count = histogram.at(keys[i]);
                float percentage = (totalCount > 0) ? (100.0f * count / totalCount) : 0;
                
                int barHeight = (count * maxBarHeight) / maxCount;
                if (barHeight < 1) barHeight = 1;
                
                int y = height - bottomSpace - barHeight;
                Rectangle(hdc, x, y, x + barWidth, height - bottomSpace);
                
                std::string label = keys[i];
                if (label.length() > 10) {
                    label = label.substr(0, 7) + "...";
                }
                
                LOGFONTA lf;
                GetObject(GetCurrentObject(hdc, OBJ_FONT), sizeof(LOGFONTA), &lf);
                lf.lfEscapement = 450;
                lf.lfOrientation = 450;
                HFONT hFont = CreateFontIndirectA(&lf);
                HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
                
                TextOutA(hdc, x + 5, height - bottomSpace + 20, label.c_str(), (int)label.length());
                
                SelectObject(hdc, hOldFont);
                DeleteObject(hFont);
                
                std::string countStr = std::to_string(count);
                TextOutA(hdc, x + barWidth/2 - 5, y - 20, countStr.c_str(), (int)countStr.length());
                
                if (barHeight > 30) {
                    std::string percentStr = std::to_string((int)percentage) + "%";
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, RGB(255, 255, 255));
                    TextOutA(hdc, x + barWidth/2 - 10, y + barHeight/2 - 5, percentStr.c_str(), (int)percentStr.length());
                    SetTextColor(hdc, RGB(0, 0, 0));
                }
            }
        }
        
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        
        DeleteObject(hAxisPen);
        DeleteObject(hBarPen);
        DeleteObject(hBarBrush);
        DeleteObject(hTextBrush);
        DeleteObject(hGridPen);
        
        ReleaseDC(hwndHistogramView, hdc);
    }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        HistogramGUI* pThis = nullptr;
        
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCTA* pCreate = (CREATESTRUCTA*)lParam;
            pThis = (HistogramGUI*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        } else {
            pThis = (HistogramGUI*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        
        if (pThis) {
            return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
        }
        
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
            case WM_COMMAND: {
                if (LOWORD(wParam) == IDC_BUILD_BTN) {
                    BuildHistogram();
                    return 0;
                }
                break;
            }
            
            case WM_SIZE: {
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                
                int newWidth = clientRect.right - clientRect.left - 40;
                int newHeight = clientRect.bottom - clientRect.top - 250;
                
                if (newWidth < 100) newWidth = 100;
                if (newHeight < 100) newHeight = 100;
                
                SetWindowPos(hwndHistogramView, NULL, 
                            20, 200, 
                            newWidth, newHeight,
                            SWP_NOZORDER | SWP_NOACTIVATE);
                
                SetWindowPos(hwndStatusLabel, NULL,
                            20, clientRect.bottom - 40,
                            clientRect.right - 40, 30,
                            SWP_NOZORDER | SWP_NOACTIVATE);
                
                InvalidateRect(hwndHistogramView, NULL, TRUE);
                break;
            }
            
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                
                RECT rect;
                GetClientRect(hwnd, &rect);
                HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
                
                std::string header = "Histogram Builder - Data Visualization Tool";
                TextOutA(hdc, 20, 5, header.c_str(), (int)header.length());
                
                EndPaint(hwnd, &ps);
                return 0;
            }
            
            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            }
            
            case WM_CLOSE: {
                DestroyWindow(hwnd);
                return 0;
            }
        }
        
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
};

int WINAPI WinMainGUI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HistogramGUI app(hInstance);
    return app.Run();
}

#endif