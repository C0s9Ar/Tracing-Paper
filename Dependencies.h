#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <Dwmapi.h>
#include <windowsx.h>

using namespace sf;
using namespace std;

// Global constant vars
#define WIDTH 750
#define HEIGHT 600
#define MIN_WIDTH 400
#define MIN_HEIGHT 325
#define MENU_HEIGHT 45
#define OPACITY_RATE 1.25
#define BUTTON_SIZE_VALUE 1.62
#define WIDTH_RATE 0.95
#define HEIGHT_RATE 0.95
#define MAX_OPACITY 250
#define MIN_OPACITY 150

// Functions prototype
bool SetTransparency(HWND, unsigned char);
void DropFile(HDROP);
LRESULT CALLBACK WinApiEventshandle(HWND, UINT, WPARAM, LPARAM);
void SetImage();