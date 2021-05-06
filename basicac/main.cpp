#include "windows.h"
#include <iostream>
#include "time.h"

using namespace std;
HWND hwnd;
POINT pt;

bool toggle = false;
bool firstclick = 1;
int mincps;
int maxcps;
int mincpsdelay;
int maxcpsdelay;

int RandomInt(int min, int max)
{
	srand(time(0));
	return ((rand() % (int)(((max)+1) - (min))) + (min));
}

int main() {
	cout << "min cps" << endl;
	cin >> mincps;
	mincpsdelay = 500 / mincps;
	cout << "max cps" << endl;
	cin >> maxcps;
	maxcpsdelay = 500 / maxcps;

	cout << "z is toggle key" << endl;

	while (1)
	{
		hwnd = GetForegroundWindow();
		if (toggle && GetAsyncKeyState(VK_LBUTTON))
		{
			if (firstclick == 1) {

				GetCursorPos(&pt);
				Sleep(RandomInt(maxcpsdelay, mincpsdelay));
				PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				firstclick = 0;
			}
			else {
				GetCursorPos(&pt);
				Sleep(RandomInt(maxcpsdelay, mincpsdelay));
				PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				Sleep(RandomInt(maxcpsdelay, mincpsdelay));
				PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
			}

		}

		if (!GetAsyncKeyState(VK_LBUTTON) && firstclick == 0) {
			firstclick = 1;
			Sleep(1);
		}

		if (GetAsyncKeyState(0x5A)) {
			toggle = !toggle;
			cout << toggle << endl;
			Sleep(200);
		}
		Sleep(1);
	}
	return 0;
}

