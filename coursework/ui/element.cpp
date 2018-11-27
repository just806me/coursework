#include "element.h"

const char *UI::Element::GenClassName()
{
	static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	static const size_t size = 16;

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 62);

	char *name = new char[size];
	name[0] = alphabet[0];

	for (size_t i = 1; i < size; ++i)
		name[i] = alphabet[distr(eng)];

	return name;
}

UI::Element::~Element()
{
	DestroyWindow(this->handle);
}

void UI::Element::InitializeElement(HFONT font)
{
	if (SetProp(this->handle, "instance", this) == FALSE)
		throw Exception("SetProp instance failed");

	SendMessage(this->handle, WM_SETFONT, (WPARAM)font, TRUE);
}

HWND UI::Element::GetHandle()
{
	return this->handle;
}

void UI::Element::SetEnabled(bool enabled)
{
	EnableWindow(this->handle, (BOOL)enabled);
}

void UI::Element::Lock()
{
	LockWindowUpdate(this->handle);
}

void UI::Element::Unlock()
{
	LockWindowUpdate(NULL);
}

LRESULT UI::Element::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(this->handle, message, wParam, lParam);
}

LRESULT UI::Element::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Element *instance = (Element *)GetProp(handle, "instance");

	if (instance == nullptr)
		return DefWindowProc(handle, message, wParam, lParam);

	switch (message)
	{
	case WM_NOTIFY:
		instance = (Element *)GetProp(((NMHDR *)lParam)->hwndFrom, "instance");
		if (instance != nullptr)
			return instance->OnMessage(message, wParam, lParam);
		break;

	case WM_COMMAND:
		return lParam != 0
			? Element::WndProc((HWND)lParam, message, wParam, 0)
			: instance->OnMessage(message, wParam, lParam);

	case WM_DESTROY:
		RemoveProp(instance->handle, "instance");
		PostQuitMessage(0);
		break;

	default:
		return instance->OnMessage(message, wParam, lParam);
	}

	return 0;
}
