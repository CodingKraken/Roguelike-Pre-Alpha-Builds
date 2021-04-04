#include <string>
#include <vector>

using namespace std;

class Dungeon
{
    public: 
        void init_room(int xpos, int ypos, int& roomlength, int& roomheight, bool is_exitroom, int xoffset = 0, int yoffset = 0); 
        void init_corridors(int dungeonsectors, vector<int>& connections, vector<vector<int>>& roomcons, int opensectors[9], int xoffset, int yoffset, int roominfo[9][4]);
        void init_dungeon(int& playerx, int& playery);

        void print_dungeon(char*& screen, int scrnlength, vector<vector<char>> updating_dungeon, int playerx, int playery, bool (&disc_sectors)[9], bool (&disc_tiles)[80*40], bool visible);
        vector<vector<char>> return_dungeon();

        bool is_move_valid(char direction, int playerx, int playery);

        Dungeon(int len, int ht);
    private:
        int length;
        int height;

        vector<vector<char>> dungeon {};
};

class Player
{
    public:

        int playerx;
        int playery;

        void move(char direction, Dungeon dungeon);
        void attack();
        void levelup();

        void set_cords(int x, int y);
        bool has_died();

        Player();

    private:

        int level = 1;
        int health = level+4;
        int defense = level;
        int strength = 1;


};

class Enemy
{
    public:

        void moid();
        void attack();

        bool has_died();

        Enemy(char letter);
        ~Enemy();
    
    private:

        int health = 1;
        int defense = 1;
        int strength = 1;

        char letter;

        int xcord;
        int ycord;
};