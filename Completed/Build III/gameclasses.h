#include <string>
#include <vector>

#pragma once

using namespace std;

class Player;
class Enemy;
struct Potion;

class Dungeon {
    public:

        void init_room(int xpos, int ypos, int& roomlength, int& roomheight, bool is_exitroom, 
        int xoffset = 0, int yoffset = 0); 
        void init_corridors(int startsector, int targetsector, bool horizontal, int roominfo[9][4]);
        void init_dungeon(int& playerx, int& playery, vector<int>& roomsecs);

        void print_dungeon(char*& screen, int scrnlength, vector<vector<char>> updating_dungeon, 
        Player &player, vector<Enemy*> enemyarr, vector<Potion*> potionarr, bool visible = false);
        vector<vector<char>> return_dungeon();

        bool is_move_valid(char direction, int playerx, int playery);

        Dungeon(int len, int ht);

    private:

        int length;
        int height;

        vector<vector<char>> dungeon {};

};

class Player {
    public:

        Player();

        void move(char direction, Dungeon dungeon);
        void attack(int damage);
        void levelup();
        void heal(int amount);
        bool has_died();

        void set_cords(int x, int y);
        void set_name(string name);
        void reset_map();
        void reset_stats();

        int playerx;
        int playery;

        int level = 1;
        int maxhealth = 10;
        int health;
        int damage = 4;
        int defense = 1;
        int exp = 0;
        int expperlvl = 4;
        int gold = 0;

        bool disc_sectors[9] {false};
        bool disc_tiles[80*40] {false};
        
        string playername;

};

class Enemy {
    public:

        Enemy(int enemyid, int xpos, int ypos);
        ~Enemy();

        int getx();
        int gety();
        int get_damage();

        void attack(int damage);
        void move(Dungeon dungeon, Player player, vector<Enemy*> enarr);
        bool has_died();
        bool can_attack(Player player);

        char letter;

    private:

        int health;
        int defense;
        int damage;
        int id;

        int xcord;
        int ycord;

};

struct Potion {

    int type;
    char letter;

    int value;
    int xcord,ycord;

    Potion(int x, int y, int type);

};