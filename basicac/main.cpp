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

double fcpsdelay1;
double realcpsdelay1;
double fcpsdelay2;
double realcpsdelay2;

int RandomInt(int min, int max)
{
	srand((unsigned int)time(0));
	return ((rand() % (int)(((max)+1) - (min))) + (min));
}

void timerSleep(double seconds) { //credits to https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/ its very good function this one
	using namespace std::chrono;

	static HANDLE timer = CreateWaitableTimer(NULL, FALSE, NULL);
	static double estimate = 5e-3;
	static double mean = 5e-3;
	static double m2 = 0;
	static int64_t count = 1;

	while (seconds - estimate > 1e-7) {
		double toWait = seconds - estimate;
		LARGE_INTEGER due;
		due.QuadPart = -int64_t(toWait * 1e7);
		auto start = high_resolution_clock::now();
		SetWaitableTimerEx(timer, &due, 0, NULL, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		auto end = high_resolution_clock::now();

		double observed = (end - start).count() / 1e9;
		seconds -= observed;

		++count;
		double error = observed - toWait;
		double delta = error - mean;
		mean += delta / count;
		m2 += delta * (error - mean);
		double stddev = sqrt(m2 / (count - 1));
		estimate = mean + stddev;
	}

	// spin lock
	auto start = high_resolution_clock::now();
	while ((high_resolution_clock::now() - start).count() / 1e9 < seconds);
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
				fcpsdelay1 = cpsdelay1;
				realcpsdelay1 = fcpsdelay1 / 1000;
				GetCursorPos(&pt);
				timerSleep(realcpsdelay1);
				PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				firstclick = 0;
			}
			else {
				cpsdelay1 = RandomInt(maxcpsdelay, mincpsdelay);
				fcpsdelay1 = cpsdelay1;
				realcpsdelay1 = fcpsdelay1 / 1000;
				cpsdelay2 = RandomInt(maxcpsdelay, mincpsdelay);
				fcpsdelay2 = cpsdelay2;
				realcpsdelay2 = fcpsdelay2 / 1000;
				cout << fcpsdelay1 << endl;
				cout << realcpsdelay1 << endl;
				GetCursorPos(&pt);
				timerSleep(realcpsdelay1);
				PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				timerSleep(realcpsdelay2);
				PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
			}

		}

		if (!GetAsyncKeyState(VK_LBUTTON) && firstclick == 0) {
			firstclick = 1;
			Sleep(1);
		}
		timerSleep(0.001);
	}
}

int main()
{
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

