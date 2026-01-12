#ifndef TILING_H
#define TILING_H
#include <iostream>
#include <unordered_map>
#include <tuple>

using namespace std;


struct Cube {
    int x, y, z;
    bool operator==(const Cube& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct CubeHash {
    size_t operator()(const Cube& c) const {
        return ((size_t)c.x * 73856093) ^
               ((size_t)c.y * 19349663) ^
               ((size_t)c.z * 83492791);
    }
};

namespace tiling
{


    class Tile{
    public:
        int X;   //remove
        int Y;   //remove
        int Z;   //remove
        int Biome = 1;


        string tileTop =   "/‾‾‾‾‾\\";
        string tileTMid = "/       \\";     //integrate these two into the function so that
        string tileBMid = "\\       /";     //customizable text can be added to the middle
        string tileBottom ="\\_____/";
        
        string colorMod = "\033[92m";   

        string tileInspect;

        void applyColor();
    };

    class City{

    public:
        int Xpos;
        int Ypos;
        int population;
        string owner;
    };

    class boardManager
    {

    public:
        
       int mapSize;
        unordered_map<struct Cube, Tile, struct CubeHash> board;

        boardManager(int size);

        unordered_map<struct Cube, Tile, struct CubeHash> makeBoard();

        void assignBiome();

        void displayTUI();

        int shuffle(int x, int y);

        size_t displayWidth(const std::string& s);

        Cube Zf(int x, int y);
        
    private:
        
    };


     

    int **citymap;
    string **map;
    Tile **tilemap;

    Tile **tileCreateLine(unsigned height, unsigned width);

    string **tileAssignBiome(unsigned height, unsigned width, int **biomes);

    int **cityGenerator(unsigned height, unsigned width);

    string **tileCreate(unsigned height, unsigned width);
}

#endif