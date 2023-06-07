#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>

using namespace sf;
using namespace std;

#define WIDTH 750
#define HEIGHT 600
#define MIN_WIDTH 400
#define MIN_HEIGHT 325
#define MENU_HEIGHT 45
#define OPACITY_RATE 1.25
#define BUTTON_SIZE_VALUE 1.6180339887498948482
#define WIDTH_RATE 0.95
#define HEIGHT_RATE 0.95
#define MAX_OPACITY 250
#define MIN_OPACITY 150
string path = "";
bool set_img = 0;
unsigned width = WIDTH;
unsigned height = HEIGHT;
Texture img;
RectangleShape background;

bool set_transparency(HWND, unsigned char);
LRESULT CALLBACK drop_files_wind_class_process(HWND, UINT, WPARAM, LPARAM);
void set_image();

int wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR lpCmdLine, int nShowCmd)
{
	background.setFillColor(Color::Transparent);
	srand(time(NULL));
	unsigned wind_pos[2] = { rand() % 100 + 50 };
L1:
	unsigned char opacity;
	unsigned mouse_pos[2] = { 0, 0 };
	bool menu_btns_state[4] = { 0, 0, 0, 0 };
	bool wind_to_top = 0;
	bool drop_file[3] = { 0 };
	bool wnd_switched = 0;
	bool wind_no_react[2] = { 0 };
	bool cb_color_changed = 0;

	RenderWindow wind(VideoMode(width, height), "", Style::None);
	wind.setPosition(Vector2i(wind_pos[0], wind_pos[1]));
	wind.setFramerateLimit(100);

	RectangleShape menu_canv;
	menu_canv.setSize(Vector2f(width, MENU_HEIGHT));
	menu_canv.setFillColor(Color(32, 32, 32));

	RectangleShape close_btn;
	close_btn.setSize(Vector2f(MENU_HEIGHT * BUTTON_SIZE_VALUE, MENU_HEIGHT));
	close_btn.setFillColor(Color(42, 42, 42));
	close_btn.setPosition(Vector2f(wind.getSize().x - close_btn.getSize().x, 0));

	RectangleShape minimize_btn;
	minimize_btn.setSize(Vector2f(MENU_HEIGHT * BUTTON_SIZE_VALUE, MENU_HEIGHT));
	minimize_btn.setFillColor(Color(42, 42, 42));
	minimize_btn.setPosition(Vector2f(wind.getSize().x - minimize_btn.getSize().x * 2, 0));

	RectangleShape slider_line;
	slider_line.setSize(Vector2f(150, MENU_HEIGHT / 5));
	slider_line.setFillColor(Color(42, 42, 42));
	slider_line.setPosition(floor(slider_line.getSize().x / 5), MENU_HEIGHT / 2 - slider_line.getSize().y / 2);

	RectangleShape slider;
	slider.setSize(Vector2f(10, MENU_HEIGHT / 3));
	slider.setFillColor(Color(42, 42, 42));
	slider.setPosition(slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x, MENU_HEIGHT / 2 - slider.getSize().y / 2);

	RectangleShape slider_trace;
	slider_trace.setFillColor(Color(52, 52, 52));
	slider_trace.setPosition(slider.getPosition().x + slider.getSize().x, slider_line.getPosition().y);
	opacity = 255 - ((slider_line.getPosition().x + slider_line.getSize().x - slider.getPosition().x) / OPACITY_RATE);
	set_transparency(wind.getSystemHandle(), opacity);

	RectangleShape checkbox;
	checkbox.setOutlineColor(Color(42, 42, 42));
	checkbox.setOutlineThickness(3);
	checkbox.setFillColor(Color(32, 32, 32));
	checkbox.setSize(Vector2f(MENU_HEIGHT / 2, MENU_HEIGHT / 2));
	checkbox.setPosition(slider.getSize().x + slider.getPosition().x + checkbox.getSize().x, MENU_HEIGHT / 2 - checkbox.getSize().y / 2);

	static const char* class_name = "WIN_CAP";
	WNDCLASSEX wx = {};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = drop_files_wind_class_process;
	wx.hInstance = hInst;
	wx.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wx.lpszClassName = (LPWSTR)"MSG";
	wx.lpszMenuName = NULL;
	wx.style = CS_HREDRAW;
	RegisterClassEx(&wx);
	HWND hwnd = CreateWindowEx(0, (LPWSTR)"MSG", (LPWSTR)"msg", 0, wind.getPosition().x, wind.getPosition().y, wind.getSize().x, wind.getSize().y, 0, NULL, NULL, NULL);
	MSG msg;
	DragAcceptFiles(hwnd, 1);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	set_transparency(hwnd, 1);
	ShowWindow(hwnd, 1);
	ShowWindow(hwnd, 0);

	background.setSize(Vector2f(wind.getSize().x, wind.getSize().y - MENU_HEIGHT));
	background.setPosition(Vector2f(0, MENU_HEIGHT));
	/*
	SetWindowPos(wind.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(wind.getSystemHandle(), SW_NORMAL);

	SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(wind.getSystemHandle(), SW_NORMAL);

	SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);

	SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, 0);
	*/
	while (wind.isOpen()) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
		if ((Mouse::isButtonPressed(Mouse::Button::Left))) {
			drop_file[1] = 1;
		}
		else
			drop_file[1] = 0;
		if (((Mouse::getPosition().x >= wind.getPosition().x) && (Mouse::getPosition().x <= wind.getPosition().x + wind.getSize().x)) && (Mouse::getPosition().y >= wind.getPosition().y) && (Mouse::getPosition().y <= wind.getPosition().y + wind.getSize().y))
			drop_file[2] = 1;
		else
			drop_file[2] = 0;
		Event event;
		if (wind.pollEvent(event))
		{
			//if (!wind_no_react[0]) 
			{
				if (event.type == Event::Closed)
					wind.close();
				if (event.type == Event::MouseLeft) {
					close_btn.setFillColor(Color(42, 42, 42));
					minimize_btn.setFillColor(Color(42, 42, 42));
					drop_file[0] = 1;
					drop_file[2] = 0;
				}
				if ((event.type == Event::MouseEntered) && !(drop_file[1]))
					drop_file[0] = 0;

				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::Escape)
					{
						wind.close();
					}
				}
				if (event.type == Event::MouseMoved)
				{
					if (!cb_color_changed && ((Mouse::getPosition().x >= wind.getPosition().x + checkbox.getPosition().x - checkbox.getOutlineThickness()) && (Mouse::getPosition().x <= wind.getPosition().x + checkbox.getPosition().x + checkbox.getSize().x + checkbox.getOutlineThickness()) && (Mouse::getPosition().y >= wind.getPosition().y + checkbox.getPosition().y - checkbox.getOutlineThickness()) && (Mouse::getPosition().y <= wind.getPosition().y + checkbox.getPosition().y + checkbox.getSize().y + checkbox.getOutlineThickness())))
					{
						checkbox.setFillColor(Color(checkbox.getFillColor().r + 5, checkbox.getFillColor().g + 5, checkbox.getFillColor().b + 5));
						cb_color_changed = 1;
					}
					else if (cb_color_changed && !((Mouse::getPosition().x >= wind.getPosition().x + checkbox.getPosition().x - checkbox.getOutlineThickness()) && (Mouse::getPosition().x <= wind.getPosition().x + checkbox.getPosition().x + checkbox.getSize().x + checkbox.getOutlineThickness()) && (Mouse::getPosition().y >= wind.getPosition().y + checkbox.getPosition().y - checkbox.getOutlineThickness()) && (Mouse::getPosition().y <= wind.getPosition().y + checkbox.getPosition().y + checkbox.getSize().y + checkbox.getOutlineThickness()))) {
						checkbox.setFillColor(Color(checkbox.getFillColor().r - 5, checkbox.getFillColor().g - 5, checkbox.getFillColor().b - 5));
						cb_color_changed = 0;
					}
					if (menu_btns_state[3]) {
						opacity = 255 - ((slider_line.getPosition().x + slider_line.getSize().x - slider.getPosition().x) / OPACITY_RATE);
						set_transparency(wind.getSystemHandle(), opacity);
						if (((Mouse::getPosition().x - wind.getPosition().x - mouse_pos[0]) >= slider_line.getPosition().x) && ((Mouse::getPosition().x - wind.getPosition().x - mouse_pos[0]) <= slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x)) {
							slider.setPosition(Vector2f(Mouse::getPosition().x - wind.getPosition().x - mouse_pos[0], slider.getPosition().y));
						}
						else if (Mouse::getPosition().x < wind.getPosition().x + slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x) {
							slider.setPosition(Vector2f(slider_line.getPosition().x, slider.getPosition().y));
						}
						else if (Mouse::getPosition().x > wind.getPosition().x + slider_line.getPosition().x) {
							slider.setPosition(Vector2f(slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x, slider.getPosition().y));

						}
						slider_trace.setSize(Vector2f(slider_line.getPosition().x + slider_line.getSize().x - (slider.getPosition().x + slider.getSize().x), slider_line.getSize().y));
						slider_trace.setPosition(slider.getPosition().x + slider.getSize().x, slider_trace.getPosition().y);
					}
					if ((Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().x >= wind.getPosition().x + wind.getSize().x - close_btn.getSize().x)))
						close_btn.setFillColor(Color(50, 50, 50));
					else
						close_btn.setFillColor(Color(42, 42, 42));

					if ((Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().x >= wind.getPosition().x + wind.getSize().x - minimize_btn.getSize().x * 2) && (Mouse::getPosition().x <= wind.getPosition().x + wind.getSize().x - minimize_btn.getSize().x)))
						minimize_btn.setFillColor(Color(50, 50, 50));
					else
						minimize_btn.setFillColor(Color(42, 42, 42));
				}
				if (event.type == Event::MouseButtonPressed) {
					if ((event.mouseButton.button == Mouse::Left))
					{
						if ((Mouse::getPosition().x >= wind.getPosition().x + checkbox.getPosition().x - checkbox.getOutlineThickness()) && (Mouse::getPosition().x <= wind.getPosition().x + checkbox.getPosition().x + checkbox.getSize().x + checkbox.getOutlineThickness()) && (Mouse::getPosition().y >= wind.getPosition().y + checkbox.getPosition().y - checkbox.getOutlineThickness()) && (Mouse::getPosition().y <= wind.getPosition().y + checkbox.getPosition().y + checkbox.getSize().y + checkbox.getOutlineThickness()))
						{
							if (!wind_no_react[0])
							{
								checkbox.setFillColor(Color(62, 62, 62));
								SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);
								SetWindowPos(wind.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
								ShowWindow(wind.getSystemHandle(), SW_NORMAL);
								wind_no_react[0] = 1;
							}
							else
							{
								checkbox.setFillColor(Color(32, 32, 32));
								wind_no_react[0] = 0;
							}
						}
						else if ((Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().y >= wind.getPosition().y + slider.getPosition().y) && (Mouse::getPosition().x <= wind.getPosition().x + slider.getPosition().x + slider.getSize().x)) && ((Mouse::getPosition().x >= wind.getPosition().x + slider.getPosition().x) && (Mouse::getPosition().x <= wind.getPosition().x + slider.getPosition().x + slider.getSize().x))) {
							menu_btns_state[3] = 1;
							mouse_pos[0] = Mouse::getPosition().x - wind.getPosition().x - slider.getPosition().x;
						}
						else if (((Mouse::getPosition().y >= wind.getPosition().y + slider_line.getPosition().y) && (Mouse::getPosition().y <= wind.getPosition().y + slider_line.getPosition().y + slider_line.getSize().y)) && ((Mouse::getPosition().x >= wind.getPosition().x + slider_line.getPosition().x) && (Mouse::getPosition().x <= wind.getPosition().x + slider_line.getPosition().x + slider_line.getSize().x))) {
							if ((Mouse::getPosition().x >= wind.getPosition().x + slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x)) {
								slider.setPosition(Vector2f(slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x, slider.getPosition().y));
							}
							else
								slider.setPosition(Vector2f(Mouse::getPosition().x - wind.getPosition().x, slider.getPosition().y));
							opacity = 255 - ((slider_line.getPosition().x + slider_line.getSize().x - slider.getPosition().x) / OPACITY_RATE);
							set_transparency(wind.getSystemHandle(), opacity);
							slider_trace.setSize(Vector2f(slider_line.getPosition().x + slider_line.getSize().x - (slider.getPosition().x + slider.getSize().x), slider_line.getSize().y));
							slider_trace.setPosition(slider.getPosition().x + slider.getSize().x, slider_trace.getPosition().y);
							menu_btns_state[3] = 1;
							mouse_pos[0] = Mouse::getPosition().x - wind.getPosition().x - slider.getPosition().x;
						}
						else if ((Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().x >= wind.getPosition().x + wind.getSize().x - close_btn.getSize().x)))
							menu_btns_state[1] = 1;
						else if ((Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().x >= wind.getPosition().x + wind.getSize().x - minimize_btn.getSize().x * 2) && (Mouse::getPosition().x <= wind.getPosition().x + wind.getSize().x - minimize_btn.getSize().x)))
							menu_btns_state[2] = 1;
						else if (Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) {
							menu_btns_state[0] = 1;
							mouse_pos[0] = Mouse::getPosition().x - wind.getPosition().x;
							mouse_pos[1] = Mouse::getPosition().y - wind.getPosition().y;
						}
					}
				}
				if (event.type == Event::MouseButtonReleased)
				{
					if ((event.mouseButton.button == Mouse::Left))
					{
						if (menu_btns_state[1] && (Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().x >= wind.getPosition().x + wind.getSize().x - close_btn.getSize().x)))
							wind.close();
						if (menu_btns_state[2] && (Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && ((Mouse::getPosition().x >= wind.getPosition().x + wind.getSize().x - minimize_btn.getSize().x * 2) && (Mouse::getPosition().x <= wind.getPosition().x + wind.getSize().x - minimize_btn.getSize().x))) {
							ShowWindow(wind.getSystemHandle(), SW_MINIMIZE);
							checkbox.setFillColor(Color(32, 32, 32));
							wind_no_react[0] = 0;
							SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
						}
						menu_btns_state[0] = 0;
						menu_btns_state[1] = 0;
						menu_btns_state[2] = 0;
						menu_btns_state[3] = 0;
					}
				}
			}
			if ((Mouse::isButtonPressed(Mouse::Button::Left)) && (menu_btns_state[0]) && !(menu_btns_state[3])) {
				wind.setPosition(Vector2i(Mouse::getPosition().x - mouse_pos[0], Mouse::getPosition().y - mouse_pos[1]));
				MoveWindow(hwnd, wind.getPosition().x, wind.getPosition().y, wind.getSize().x, wind.getSize().y, 0);
			}
		}

		wind.clear(Color(22, 22, 22));
		wind.draw(menu_canv);
		wind.draw(close_btn);
		wind.draw(minimize_btn);
		wind.draw(slider_line);
		wind.draw(slider_trace);
		wind.draw(slider);
		wind.draw(background);
		wind.draw(checkbox);
		wind.display();

		if (((drop_file[0] && drop_file[1]) && drop_file[2]) && !(wnd_switched) && !IsIconic(wind.getSystemHandle()))
		{
			POINT mouse_point;
			HWND Handle;
			GetCursorPos(&mouse_point);
			if (WindowFromPoint(mouse_point) == wind.getSystemHandle()) {
				wnd_switched = 1;
				ShowWindow(hwnd, 1);
			}
		}
		if (!(Mouse::isButtonPressed(Mouse::Button::Left)) && (wnd_switched))
		{
			drop_file[0] = 0;
			drop_file[1] = 0;
			drop_file[2] = 0;
			wnd_switched = 0;
			ShowWindow(hwnd, 0);
		}
		if (set_img)
		{
			set_image();
			goto L1;
		}
		if (wind_no_react[0]) {
			if ((Mouse::getPosition().x >= wind.getPosition().x) && (Mouse::getPosition().x <= wind.getPosition().x + wind.getSize().x)) {
				if ((Mouse::getPosition().y >= wind.getPosition().y) && (Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && !(IsIconic(wind.getSystemHandle()))) {
					wind_no_react[1] = 0;
					SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_LAYERED);
					//SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
					ShowWindow(wind.getSystemHandle(), SW_NORMAL);
				}
				else if ((Mouse::getPosition().y >= wind.getPosition().y + MENU_HEIGHT) && (Mouse::getPosition().y <= wind.getPosition().y + wind.getSize().y) && !(IsIconic(wind.getSystemHandle())))
				{
					wind_no_react[1] = 1;
					SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);
					SetWindowPos(wind.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
					ShowWindow(wind.getSystemHandle(), SW_NORMAL);
				}
			}
		}
		else {
			SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		}
		/*
		if (wind_no_react[0] && wind_no_react[1]) {
			cout << 1;
			SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);
			SetWindowPos(wind.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			ShowWindow(wind.getSystemHandle(), SW_NORMAL);
			//set_transparency(wind.getSystemHandle(), opacity);
			wind_no_react[1] = 0;
		}
		else if (!wind_no_react[0] && !wind_no_react[1]) {
			cout << 2;
			SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_LAYERED);
			SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			ShowWindow(wind.getSystemHandle(), SW_NORMAL);
			//set_transparency(wind.getSystemHandle(), opacity);
			wind_no_react[1] = 1;
		}
		//HWND test = (GetWindow(wind.getSystemHandle(), GW_HWNDFIRST));
		//cout << drop_file[0] << drop_file[1] << drop_file[2] << endl;
		SetWindowPos(wind.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		ShowWindow(wind.getSystemHandle(), SW_NORMAL);
		*/
	}
	return 0;
}

bool set_transparency(HWND hWnd, unsigned char alpha) {
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}
void drop_file(HDROP files) {
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
LRESULT CALLBACK drop_files_wind_class_process(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		break;
	case WM_DROPFILES:
		drop_file((HDROP)wParam);
		break;
	}

	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}
void set_image() {
	img.loadFromFile(path);
	background.setFillColor(Color(255, 255, 255, 150));
	background.setTexture(&img);
	float img_ratio = (float)img.getSize().x / (float)img.getSize().y;
	width = img.getSize().x;
	height = img.getSize().y;
	if (width >= GetSystemMetrics(SM_CXSCREEN) * WIDTH_RATE) {
		width = GetSystemMetrics(SM_CXSCREEN) * WIDTH_RATE;
	}
	else if (width <= MIN_WIDTH)
	{
		width = MIN_WIDTH;
	}
	height = width * ((float)img.getSize().y / (float)img.getSize().x);
	if (height >= GetSystemMetrics(SM_CYSCREEN) * HEIGHT_RATE) {
		height = GetSystemMetrics(SM_CYSCREEN) * HEIGHT_RATE;
	}
	else if (height <= MIN_HEIGHT)
	{
		height = MIN_WIDTH;
	}
	width = height * ((float)img.getSize().x / (float)img.getSize().y);
	set_img = 0;
	cout << width << "	" << height << endl;
}