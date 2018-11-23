#include "window.h"

UI::Window::Window(HINSTANCE instance, HFONT font, HICON icon, const char *title, int width, int height,
	HMENU menu, std::function<void(WORD)> onMenuSelected)
{
	this->className = Element::GenClassName();
	this->instance = instance;
	this->OnMenuSelected = onMenuSelected;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Element::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = icon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = this->className;
	wcex.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
		throw Exception("RegisterClassEx failed");

	this->handle = CreateWindowEx(NULL, this->className, title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, menu, instance, NULL);

	if (!this->handle)
		throw Exception("CreateWindow failed");

	this->InitializeElement(font);
}

UI::Window::~Window()
{
	UnregisterClass(this->className, this->instance);
	delete[] this->className;
}

void UI::Window::Show()
{
	ShowWindow(this->handle, SW_SHOW);
}

void UI::Window::Close()
{
	SendMessage(this->handle, WM_CLOSE, 0, 0);
}

void UI::Window::StartMessageLoop()
{
	MSG msg;

	if (!UpdateWindow(this->handle))
		throw Exception("UpdateWindow failed");

	while (int result = GetMessage(&msg, NULL, 0, 0))
	{
		if (result == -1)
			throw Exception("GetMessage failed");

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT UI::Window::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND && lParam == 0 && HIWORD(wParam) == 0)
	{
		if (this->OnMenuSelected != nullptr)
			CallbackQueue::GetInstance()->Push([this, wParam]() { this->OnMenuSelected(LOWORD(wParam)); });

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
