#include <windows.h>
#include <functional>
#include "element.h"
#include "../callback_queue.h"

namespace UI
{
	class Button : public Element
	{
	public:
		Button(HINSTANCE, HFONT, HWND, const char *, int, int, int, int, std::function<void()>);
		std::function<void()> OnClick;

	protected:
		LRESULT OnMessage(UINT, WPARAM, LPARAM) override;
	};
}