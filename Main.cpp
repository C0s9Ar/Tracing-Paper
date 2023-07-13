#include "Dependencies.h"

// Extern global variables
extern string path;
extern bool set_img;
extern unsigned width;
extern unsigned height;
extern Texture img;
extern RectangleShape background;

#if NDEBUG
// Console disabled (Release version)
int wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR lpCmdLine, int nShowCmd)
#else
// Console enabled (Debug version)
int main(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR lpCmdLine, int nShowCmd)
#endif
{	
	background.setFillColor(Color::Transparent);
	// Generate random window position
	srand(time(NULL));
	unsigned wind_pos[2] = { rand() % 200, rand() % 200 };
L1:
	// Creating and register Win32 api class 
	WNDCLASS WindowClass;
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = &WinApiEventshandle;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hInst;
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"Tracing Paper";
	RegisterClass(&WindowClass);

	// Window behavior miscellaneous variables
	unsigned char opacity;
	unsigned mouse_pos[2] = { 0, 0 };
	bool menu_btns_state[4] = { 0, 0, 0, 0 };
	bool wind_no_react[2] = { 0 };
	bool cb_color_changed = 0;

	// Creating SFML window
	RenderWindow wind(CreateWindow(L"Tracing Paper", L"Tracing Paper", WS_SIZEBOX | WS_VISIBLE | WS_SYSMENU | WS_BORDER, wind_pos[0], wind_pos[1], width, height, NULL, NULL, hInst, NULL));
	wind.setFramerateLimit(75);

	DragAcceptFiles(wind.getSystemHandle(), 1);

	// Main window GUI
	// Window menu canvas
	RectangleShape menu_canv;
	menu_canv.setSize(Vector2f(width, MENU_HEIGHT));
	menu_canv.setFillColor(Color(30, 30, 30));

	// Window close button
	RectangleShape close_btn;
	close_btn.setSize(Vector2f(MENU_HEIGHT * BUTTON_SIZE_VALUE, MENU_HEIGHT));
	close_btn.setFillColor(Color(42, 42, 42));
	close_btn.setPosition(Vector2f(wind.getSize().x - close_btn.getSize().x, 0));

	// Window minimize button
	RectangleShape minimize_btn;
	minimize_btn.setSize(Vector2f(MENU_HEIGHT * BUTTON_SIZE_VALUE, MENU_HEIGHT));
	minimize_btn.setFillColor(Color(42, 42, 42));
	minimize_btn.setPosition(Vector2f(wind.getSize().x - minimize_btn.getSize().x * 2, 0));

	// Opacity slider
	RectangleShape slider_line;
	slider_line.setSize(Vector2f(150, MENU_HEIGHT / 5));
	slider_line.setFillColor(Color(42, 42, 42));
	slider_line.setPosition(floor(slider_line.getSize().x / 5), MENU_HEIGHT / 2 - slider_line.getSize().y / 2);

	RectangleShape slider;
	slider.setSize(Vector2f(10, MENU_HEIGHT * 0.3));
	slider.setFillColor(Color(42, 42, 42));
	slider.setPosition(slider_line.getPosition().x + slider_line.getSize().x - slider.getSize().x, MENU_HEIGHT / 2 - slider.getSize().y / 2);

	RectangleShape slider_trace;
	slider_trace.setFillColor(Color(52, 52, 52));
	slider_trace.setPosition(slider.getPosition().x + slider.getSize().x, slider_line.getPosition().y);
	opacity = 255 - ((slider_line.getPosition().x + slider_line.getSize().x - slider.getPosition().x) / OPACITY_RATE);
	SetTransparency(wind.getSystemHandle(), opacity);

	// Inactive state checkbox
	RectangleShape checkbox;
	checkbox.setOutlineColor(Color(42, 42, 42));
	checkbox.setOutlineThickness(3);
	checkbox.setFillColor(Color(32, 32, 32));
	checkbox.setSize(Vector2f(MENU_HEIGHT / 2, MENU_HEIGHT / 2));
	checkbox.setPosition(slider.getSize().x + slider.getPosition().x + checkbox.getSize().x, MENU_HEIGHT / 2 - checkbox.getSize().y / 2);

	// Setting the image canvas size
	if (path == "") {
		background.setSize(Vector2f(wind.getSize().x, wind.getSize().y - MENU_HEIGHT));
		background.setPosition(Vector2f(0, MENU_HEIGHT));
	}

	// Win32 api, SFML events
	MSG msg;
	Event event;

	while (wind.isOpen()) {
		// Win32 api events handle
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}
		// SMFL events handle
		if (wind.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				wind.close();
			}
			if (event.type == Event::MouseLeft) {
				close_btn.setFillColor(Color(42, 42, 42));
				minimize_btn.setFillColor(Color(42, 42, 42));
			}
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
					SetTransparency(wind.getSystemHandle(), opacity);
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
						SetTransparency(wind.getSystemHandle(), opacity);
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
			if (event.type == Event::Resized)
			{
				FloatRect view(0, 0, event.size.width, event.size.height);
				wind.setView(View(view));
				menu_canv.setSize(Vector2f(wind.getSize().x, MENU_HEIGHT));
				close_btn.setPosition(Vector2f(wind.getSize().x - close_btn.getSize().x, 0));
				minimize_btn.setPosition(Vector2f(wind.getSize().x - minimize_btn.getSize().x * 2, 0));
				background.setSize(Vector2f(wind.getSize().x, wind.getSize().y - MENU_HEIGHT));
			}
			if ((Mouse::isButtonPressed(Mouse::Button::Left)) && (menu_btns_state[0]) && !(menu_btns_state[3])) {
				wind.setPosition(Vector2i(Mouse::getPosition().x - mouse_pos[0], Mouse::getPosition().y - mouse_pos[1]));
			}
		}

		// Draw window GUI
		wind.clear(Color(22, 22, 22));
		wind.draw(menu_canv);
		wind.draw(close_btn);
		wind.draw(minimize_btn);
		wind.draw(slider_line);
		wind.draw(slider_trace);
		wind.draw(slider);
		wind.draw(checkbox);
		wind.draw(background);
		wind.display();

		// Load image to the image canvas
		if (set_img)
		{
			SetImage();
			DestroyWindow(wind.getSystemHandle());
			UnregisterClass(L"Tracing Paper", hInst);
			goto L1;
		}

		// Set window inactive state
		if (wind_no_react[0]) {
			if ((Mouse::getPosition().x >= wind.getPosition().x) && (Mouse::getPosition().x <= wind.getPosition().x + wind.getSize().x)) {
				if ((Mouse::getPosition().y >= wind.getPosition().y) && (Mouse::getPosition().y <= wind.getPosition().y + MENU_HEIGHT) && !(IsIconic(wind.getSystemHandle()))) {
					wind_no_react[1] = 0;
					SetWindowLong(wind.getSystemHandle(), GWL_EXSTYLE, WS_EX_LAYERED);
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
			DragAcceptFiles(wind.getSystemHandle(), 1);
		}
		else {
			SetWindowPos(wind.getSystemHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		}
	}

	UnregisterClass(L"Tracing Paper", hInst);
	return 0;
}

