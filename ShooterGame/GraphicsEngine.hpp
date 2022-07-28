#pragma once
#include <vector>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class InputRegistry;

class Paintable;

class GraphicsEngine {

public:

	GraphicsEngine(InputRegistry* inputRegistry, int x = 75, int y = 25, int width = 1200, int height = 700);
	~GraphicsEngine();

	void createWindow(const wchar_t* name);
	void createWindow();
	void paint(Gdiplus::Graphics* graphics);

	static GraphicsEngine* defaultWindow;
	static GraphicsEngine* getDefault();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

	void paintElements(Gdiplus::Graphics* graphics);

	void addPaintable(Paintable* paintable);
	void removePaintable(Paintable* paintable);

	int getWidth();
	int getHeight();
	int getWidthOffset();
	int getHeightOffset();

	int getID();

	HWND* getWindowHandle();
	
private:

	InputRegistry* inputRegistry;

	// Painting Variables
	Gdiplus::Color* backgroundColor;
	std::vector<Paintable*>* paintables;

	// Gdiplus Variables
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Window Variables
	int width;
	int height;
	int x;
	int y;

	int timePerFrame = 5; // milliseconds

	WNDCLASS windowClass;
	HWND windowHandle;

	double xOffset = 16;
	double yOffset = 38;

	int id;

	static int windowCounter;
	static std::vector<GraphicsEngine*>* graphicsEngines;

};

class Paintable {
public:
	virtual void paint(Gdiplus::Graphics* graphics) = 0;
};