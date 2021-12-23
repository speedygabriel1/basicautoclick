#include "windows.h"
#include <iostream>
#include "time.h"
#include <thread>


using namespace std;
HWND hwnd;
POINT pt;

bool toggle = false;
bool firstclick = 1;
int mincps;
int maxcps;
int mincpsdelay;
int maxcpsdelay;

int cpsdelay1;
int cpsdelay2;


int RandomInt(int min, int max)
{
	srand((unsigned int)time(0));
	return ((rand() % (int)(((max)+1) - (min))) + (min));
}

void ac() 
{
	while (TRUE)
	{
		hwnd = GetForegroundWindow();
		if (toggle && GetAsyncKeyState(VK_LBUTTON))
		{
			if (firstclick == 1) {
				cpsdelay1 = RandomInt(maxcpsdelay, mincpsdelay);
				GetCursorPos(&pt);
				Sleep(cpsdelay1);
				PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				firstclick = 0;
			}
			else {
				cpsdelay1 = RandomInt(maxcpsdelay, mincpsdelay);
				
				GetCursorPos(&pt);
				Sleep(cpsdelay1);
				PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				cpsdelay2 = RandomInt(maxcpsdelay, mincpsdelay);
				Sleep(cpsdelay2);
				PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
			}

		}

		if (!GetAsyncKeyState(VK_LBUTTON) && firstclick == 0) {
			firstclick = 1;
			Sleep(1);
		}
		Sleep(1);
	}
}

int main()
{
	timeBeginPeriod(0);
	cout << "min cps" << endl;
	cin >> mincps;
	mincpsdelay = 500 / mincps;
	cout << "max cps" << endl;
	cin >> maxcps;
	maxcpsdelay = 500 / maxcps;

	cout << "z is toggle key" << endl;

	thread first(ac);
	

	while (1)
	{
		if (GetAsyncKeyState(0x5A)) {
			toggle = !toggle;
			cout << toggle << endl;
			Sleep(200);
		}
		Sleep(1);
	}
	return 0;

}

