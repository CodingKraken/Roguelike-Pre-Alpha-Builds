#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

using namespace std;

void setCursorPosition(int x, int y);
void showConsoleCursor(bool show);
int random(int lower_bound, int upperbound);
void toScreen(char*& screen, const int index, const char* text);
int return_sector(int x, int y);
bool can_connect(int startsector, int endsector, int sectors[9], bool& xfirst);
void color_tile(HANDLE& hConsole_c, char colour, COORD cord);