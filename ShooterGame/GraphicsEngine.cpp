#include "GraphicsEngine.hpp"
#include <iostream>
#include "InputRegistry.hpp"
#define TIMERID_1 1

std::vector<GraphicsEngine*>* GraphicsEngine::graphicsEngines;
int GraphicsEngine::windowCounter;

GraphicsEngine::GraphicsEngine(InputRegistry* inputRegistry, int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
	this->height = height;
    this->timePerFrame = timePerFrame;
    this->inputRegistry = inputRegistry;
    defaultWindow = this;

    paintables = new std::vector<Paintable*>();
    backgroundColor = new Gdiplus::Color(230, 230, 230);

    if (graphicsEngines == nullptr) {
        graphicsEngines = new std::vector<GraphicsEngine*>();
    }
    if (windowCounter < 0) {
        windowCounter = 0;
    }
    graphicsEngines->push_back(this);
    id = windowCounter;
    windowCounter++;

    // Initialize GDI+.
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

GraphicsEngine::~GraphicsEngine() {
    delete backgroundColor;
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void GraphicsEngine::createWindow(const wchar_t* name) {
    windowClass = { 0 };

    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = NULL;
    windowClass.lpfnWndProc = WndProc;
//    windowClass.lpszClassName = L"Window in Console";
    windowClass.lpszClassName = name;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.cbWndExtra = sizeof(int);

    //windowClass.style = CS_HREDRAW | CS_VREDRAW;
    //windowClass.lpfnWndProc = WndProc;
    //windowClass.cbClsExtra = 0;
    //windowClass.cbWndExtra = 0;
    //windowClass.hInstance = NULL;
    //windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    //windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    //windowClass.lpszMenuName = NULL;
    //windowClass.lpszClassName = TEXT("GettingStarted");

    if (!RegisterClass(&windowClass)) {
        MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
    }

    windowHandle = CreateWindow(
//        L"Window in Console",
        name,
        name,
        WS_OVERLAPPEDWINDOW,
        x,
        y,
        width,
        height,
        NULL,
        NULL,
        NULL,
        NULL);
    ShowWindow(windowHandle, SW_RESTORE);

    SetWindowLongPtrA(windowHandle, 0, id);

    SetTimer(windowHandle, TIMERID_1, timePerFrame, (TIMERPROC)WndProc);
}

void GraphicsEngine::createWindow() {
    createWindow(L"Window");
}

void GraphicsEngine::paint(Gdiplus::Graphics* graphics) {
    Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*backgroundColor);
    graphics->FillRectangle(brush, 0, 0, width, height);

    paintElements(graphics);
}

GraphicsEngine* GraphicsEngine::defaultWindow;
GraphicsEngine* GraphicsEngine::getDefault() {
    return GraphicsEngine::defaultWindow;
}

HDC* hdc = new HDC();
PAINTSTRUCT  ps;

HDC* memHDC = new HDC();
HBITMAP memMap{};
HANDLE hOld;

LRESULT CALLBACK GraphicsEngine::WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam) {

    long index = GetWindowLongPtrA(hWnd, 0);
    GraphicsEngine* graphicsEngine = graphicsEngines->at(index);

    switch (message)
    {
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
    {
        *hdc = BeginPaint(hWnd, &ps);

        *memHDC = CreateCompatibleDC(*hdc);
        memMap = CreateCompatibleBitmap(*hdc, graphicsEngine->width, graphicsEngine->height);
        hOld = SelectObject(*memHDC, memMap);

        //Paint(memHDC);
        Gdiplus::Graphics* graphics = new Gdiplus::Graphics(*memHDC);
        graphicsEngine->paint(graphics);
        //std::cout << "Painted\n";

        //BitBlt(*hdc, 0, -graphicsEngine->getHeightOffset(), graphicsEngine->width, graphicsEngine->height, *memHDC, 0, 0, SRCCOPY);
        BitBlt(*hdc, 0, 0, graphicsEngine->width, graphicsEngine->height, *memHDC, 0, 0, SRCCOPY);

        SelectObject(*memHDC, hOld);

        DeleteObject(memMap);
        DeleteDC(*memHDC);
        EndPaint(hWnd, &ps);
        return 0;
    }
        break;
    case WM_CHAR:
        if (wparam == VK_ESCAPE)
        {
            DestroyWindow(hWnd);
        }
        //break;
    case WM_KEYDOWN:
        graphicsEngine->inputRegistry->resgisterKeyInput(new KeyInfo(wparam, true));
        //std::cout << "Key Code: " << wparam << std::endl;
        break;
    case WM_KEYUP:
        graphicsEngine->inputRegistry->resgisterKeyInput(new KeyInfo(wparam, false));
        break;
    case WM_DESTROY:
        graphicsEngines->at(graphicsEngine->getID()) = nullptr;
        for (int i = 0; i < graphicsEngines->size(); i++) {
            if (graphicsEngines->at(i) != nullptr) {
                return 0;
            }
        }

        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        graphicsEngine->inputRegistry->resgisterKeyInput(new KeyInfo(Mouse_Left_Button, true));
        break;
    case WM_LBUTTONUP:
        graphicsEngine->inputRegistry->resgisterKeyInput(new KeyInfo(Mouse_Left_Button, false));
        break;
    case WM_RBUTTONDOWN:
        graphicsEngine->inputRegistry->resgisterKeyInput(new KeyInfo(Mouse_Right_Button, true));
        break;
    case WM_RBUTTONUP:
        graphicsEngine->inputRegistry->resgisterKeyInput(new KeyInfo(Mouse_Right_Button, false));
        break;
    default:
        return DefWindowProc(hWnd, message, wparam, lparam);
    }

    return 0;
}

void GraphicsEngine::paintElements(Gdiplus::Graphics* graphics) {
    for (int i = 0; i < paintables->size(); i++) {
        paintables->at(i)->paint(graphics);  
    }
}

void GraphicsEngine::addPaintable(Paintable* paintable) {
    paintables->push_back(paintable);
}

void GraphicsEngine::removePaintable(Paintable* paintable) {
    for (int i = 0; i < paintables->size(); i++) {
        if (paintables->at(i) == paintable) {
            paintables->erase(paintables->begin() + i);
            return;
        }
    }
}

int GraphicsEngine::getWidth() {
    return width;
}

int GraphicsEngine::getHeight() {
    return height;
}

int GraphicsEngine::getWidthOffset() {
    return xOffset;
}

int GraphicsEngine::getHeightOffset() {
    return yOffset;
}

int GraphicsEngine::getID() {
    return id;
}

HWND* GraphicsEngine::getWindowHandle() {
    return &windowHandle;
}
