#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wchar.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include "CircleDrawer.h"

#define UNICODE
#define FILE_PATH  L"C:/Users/Veronika/Documents/work/5 сем/ОСиСП/OSaSP2_2/data.txt"

std::wstring str;
CircleDrawer* drawer;
RECT clientSize;
const COLORREF colorText = RGB(255, 255, 255);
const COLORREF colorBack = RGB(4, 20, 4);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SourceArrayInit(HANDLE hFile);
void InitSources(HWND hwnd);
void Paint(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"LR2_2";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"OSaSP Laboratory work 2.Part 2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg); 
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        switch (uMsg)
        {
        case WM_CREATE:
            InitSources(hwnd);
            break;
        case WM_PAINT: {
            Paint(hwnd);
        }
                     break;
        case WM_SIZE:
        {
            GetClientRect(hwnd, &clientSize);
            drawer->SetClientSize(clientSize);
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
        case WM_GETMINMAXINFO:
        {
            const auto lpMmi = reinterpret_cast<LPMINMAXINFO>(lParam);
            lpMmi->ptMinTrackSize.x = 100;
            lpMmi->ptMinTrackSize.y = 100;
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);//обеспечивает обработку всех сообщений
        }
        return 0;
}

void InitSourceArray(HANDLE hFile) {
    int fileSize = GetFileSize(hFile, NULL);
    wchar_t* cBufferText = new wchar_t[fileSize/2-2];
    DWORD dwByte;
    ReadFile(hFile, cBufferText, fileSize, &dwByte, NULL);
    std::wstring copied(cBufferText);
    str = copied;
}

void InitSources(HWND hwnd) {
    const TCHAR szFileName[] = FILE_PATH;
    HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //?
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hwnd, L"File not found!", L"Error", MB_OK);
    }
    else {
        InitSourceArray(hFile);
        drawer = new CircleDrawer(str);
        CloseHandle(hFile);
    }
}

void Paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    HBRUSH brush;
    brush = CreateSolidBrush(colorBack);
    SelectObject(hdc, brush);
    Rectangle(hdc, clientSize.left, clientSize.top, clientSize.right, clientSize.bottom);
    DeleteObject(brush);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, colorText);
    const auto pen = CreatePen(PS_SOLID, 2, colorText);
    SelectObject(hdc, pen);
    drawer->DrawCircle(hdc);
    DeleteObject(pen);
    DeleteObject(hdc);
    EndPaint(hwnd, &ps);
}

