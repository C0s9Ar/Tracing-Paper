#include "Dependencies.h"

// Global vars
string path = "";
bool set_img = 0;
unsigned width = WIDTH;
unsigned height = HEIGHT;
Texture img;
RectangleShape background;

// Set window transparency (0 - 255)
bool SetTransparency(HWND hWnd, unsigned char alpha) {
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}

// File drop handle
void DropFile(HDROP files) {
	char name[MAX_PATH];
	int files_amount = DragQueryFile(files, 0xffffffff, 0, 0);
	DragQueryFileA(files, 0, (LPSTR)(name), MAX_PATH);
	path = name;
	cout << path;
	for (int i = 0; i != path.size(); i++)
	{
		if (path[i] == 92)
			path[i] = '/';
	}
	cout << endl << path << endl;
	DragFinish(files);
	set_img = 1;
}

// Winapi events handle
LRESULT CALLBACK WinApiEventshandle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	static RECT border_thickness;
	switch (msg)
	{
	case WM_CREATE:
	{
		SetRectEmpty(&border_thickness);
		if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_THICKFRAME)
		{
			AdjustWindowRectEx(&border_thickness, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
			border_thickness.left *= -0.29;
			border_thickness.top *= -1;
		}
		else if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_BORDER)
		{
			SetRect(&border_thickness, 1, 1, 1, 1);
		}
		MARGINS margins = { 0 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_GETMINMAXINFO:
	{
		lpMMI->ptMinTrackSize.x = MIN_WIDTH;
		lpMMI->ptMinTrackSize.y = MIN_HEIGHT;
		break;
	}
	case WM_DROPFILES:
	{
		DropFile((HDROP)wParam);
		break;
	}
	case WM_NCCALCSIZE:
	{
		if (lParam)
		{
			NCCALCSIZE_PARAMS* sz = (NCCALCSIZE_PARAMS*)lParam;
			sz->rgrc[0].left += border_thickness.left;
			sz->rgrc[0].right -= border_thickness.right;
			sz->rgrc[0].bottom -= border_thickness.bottom;
			return 0;
		}
		break;
	}
	case WM_NCHITTEST:
	{
		//do default processing, but allow resizing from top-border
		LRESULT result = DefWindowProc(hwnd, msg, wParam, lParam);
		if (result == HTCLIENT)
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ScreenToClient(hwnd, &pt);
			if (pt.y < border_thickness.top) return HTTOP;
		}
		return result;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Load dragged image to the program
void SetImage() {
	img.loadFromFile(path);
	background.setFillColor(Color(255, 255, 255, 255));
	background.setTexture(&img, 1);
	width = img.getSize().x;
	height = img.getSize().y;
	height = width * ((float)img.getSize().y / (float)img.getSize().x);
	if (height >= GetSystemMetrics(SM_CYSCREEN) * HEIGHT_RATE) {
		height = GetSystemMetrics(SM_CYSCREEN) * HEIGHT_RATE;
	}
	else if (height <= MIN_HEIGHT)
	{
		height = MIN_WIDTH;
	}

	width = height * ((float)img.getSize().x / (float)img.getSize().y) - MENU_HEIGHT;
	if (width >= GetSystemMetrics(SM_CXSCREEN) * WIDTH_RATE) {
		width = GetSystemMetrics(SM_CXSCREEN) * WIDTH_RATE;
	}
	else if (width <= MIN_WIDTH)
	{
		width = MIN_WIDTH;
	}
	background.setSize(Vector2f(width, height - MENU_HEIGHT));
	set_img = 0;

}
