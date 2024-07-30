#include <Windows.h>
#include <string>
#include "ImGUI/imgui.h"
enum logStatus {
	ERR = 0,
	SUCCESS = 1,
	WARNING = 2,
	NORMAL = 3
};

class Line {
public:
	char* line;
	int color;
	Line(char*, int);
};

class Console {
public:
	HWND* hWnd;
	bool open = true;
	bool finished = false;
	Console(HWND* hwnd);
	void Draw(bool windowFollow=false);
	void AddLog(const char* log, int col);
	void Done();
private:
	bool showStart = false;
	ImVector<Line> Items;
};

