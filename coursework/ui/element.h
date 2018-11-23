#pragma once

#include <windows.h>
#include <random>
#include "../exception.h"

namespace UI
{
	class Element
	{
	public:
		virtual ~Element();
		virtual LRESULT OnMessage(UINT, WPARAM, LPARAM);
		HWND GetHandle();
		void SetEnabled(bool);
		void Lock();
		void Unlock();

	protected:
		void InitializeElement(HFONT);
		HWND handle;

		static const char *GenClassName();
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	};
}
