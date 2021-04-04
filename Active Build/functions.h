#include <cstdlib>
#include <math.h>
#include <string>
#include <vector>
#include <windows.h>

#include "gameclasses.h"

int random(int lower_bound, int upperbound);
int dist(int x, int y, int x2, int y2);

void toScreen(char*& screen, int column, int row, const char* text);
void init_screen(HANDLE hConsole_c);
void write_screen(char*& screen, HANDLE hConsole_c, int screensize, DWORD dwBytesWritten);
void typing_handler(string& inputstring, int& cursor, int limit, int &keypressed);
void color_tile(HANDLE& hConsole_c, char colour, COORD cord);
void color_string(HANDLE hConsole_c, char colour, COORD cord, int length);

void display_ui(char*& screen, HANDLE hConsole_c, Player player);
void display_mainmenu(char*& screen, HANDLE hConsole_c, int cursor, int screensize, DWORD dwBytesWritten);
void display_build(char*& screen, HANDLE hConsole_c);
void display_gamemenu(char*& screen, HANDLE hConsole_c, int cursor, int screensize, DWORD dwBytesWritten);
void display_deathscreen(char*& screen, HANDLE hConsole_c, Player player);
void colour_objects(char*& screen, int screenwidth, int screenheight, HANDLE hConsole_c);

int return_sector(int x, int y);
int return_roomcord(int roomsector, bool x);
bool can_connect(int startsector, int endsector, int sectors[9], bool& horizontal);
bool have_connected(vector<vector<int>> connections, int startsector, int endsector);

void update_enemies(vector<Enemy*> &enarr, Dungeon dungeon, Player &player, int turns_since_hit);
void initialize_enemies(Dungeon dungeon, vector<Enemy*> &enarr, Player player, vector<int> sectors);
bool is_enemy(vector<Enemy*> &enarr, Player player, int x, int y);
bool occupied_enemy(vector<Enemy*> &enarr, Player player, char direction, int &enid);

void initialize_potions(Dungeon dungeon, vector<Potion*> &potionarr, vector<int> sectors);
void update_potion(vector<Potion*> &potionarr, Player &player);

void update_objects(vector<Enemy*> &enarr, vector<Potion*> &potionarr, Player &player, Dungeon dungeon);