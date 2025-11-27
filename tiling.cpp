#include <list>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <random>
#include "tiling.h"
using namespace std;

namespace tiling{
    //This game uses a hexagonal array to store info since the grid is hexagonal. The array is controlled by three variables X, Y, and Z.
        
    // In hexagonal arrays, when one variable is held at a constant value, the other two variables are inversely proportional with equal magnitudes. These
    // arrays require [0,0,0] to be at the center, as opposed to a traditional 2d array which starts at the corner. Since the two non-constant variables in a 
    // hexagonal array have this relationship, and the center point must be [0,0,0], we know that the three coordinates of the array must always sum to 0. Because
    // of this, there are some coordinate points like [1,1,1] which do not exist 


    //For example: Let's say you start at position 0,0,0 of the array (center), moving to a position where x = 1 will require either Y or Z to be -1. As you have 
    // probably already figured, this means that there are two different tiles where x = : [1,-1,0] & [1,0,-1]

    void Tile::applyColor(){
        tileTop = colorMod + tileTop + "\033[0m";
        tileTMid = colorMod + tileTMid + "\033[0m";
        tileBMid = colorMod + tileBMid + "\033[0m";
        tileBottom = colorMod + tileBottom + "\033[0m";
    }

    boardManager::boardManager(int size) : mapSize(size),board(makeBoard()) {};

    unordered_map<struct Cube,Tile, struct CubeHash> boardManager::makeBoard(){
        
        //Create H-Array

        unordered_map<Cube, Tile, CubeHash> Tilemap;
        
        //assign Tiles to Valid spaces

        for ( int x = -mapSize; x < mapSize + 1; x++) {   //create x to start at -X, and end at X""
            for ( int y = -mapSize; y < mapSize + 1; y++) { //create x to start at -X, and end at X
                for ( int z = -mapSize; z < mapSize + 1; z++) { //create x to start at -X, and end at X
                    if((x+y+z) == 0){ //checks to make sure that coordinate point is valid on an H-array *SEE COMMENT AT CLASS DECLARATION*
                        Tile tile;
                        tile.X = x;
                        tile.Y = y;
                        tile.Z = z;
                        Tilemap[{x,y,z}] = tile;
                    };

                };
            };
        };


        return Tilemap;
    }
    
    void boardManager::assignBiome(){
        
        
        //add water base tile      

        board[{mapSize,-mapSize, 0}].Biome = 6; //currently always spawns at the leftmost point of the map
        board[{mapSize,-mapSize, 0}].colorMod = "\033[1;34m";
        
        //add desert tiles and snow tiles 
        
        for (auto&[cube, tile] : board) {
            if(cube.z == mapSize){
                tile.Biome = 4;                        //add desert tiles
                tile.colorMod = "\033[33m";
            }
            else if (cube.z == -mapSize){
                tile.Biome = 5;                        //add snow tiles
                tile.colorMod = "\033[1;37m";
            }
        }
        //add forest tiles
        
        //add mountain tiles
        
    };
    /*This function fucking sucks. I'm really bad at C++ and even I know how bad this is. It NEEDS to be optimized but I'm afraid that doing so will require me to start from scratch. 
    like I note later, Xrelative comes out to X.5 for tiles where (cube.y % 2 != 0) which means that the if else is uneccesary and can be consolidated into one process
    Also, all of the appending is Identical except for the X position modifier, which means the four append functions can be combined into one inside a for loop for neater looking code
    in terms of map and MMap, that is a complete skill issue and can probably be done in like four lines of code. also, in order to combat a range error, map creates way more vector values than is necessary. this might be the laziest fucking thing I've ever done, as fixing this one would only take me about 20 minutes 

    this function technically works so I'm going to focus on more pressing matters and just open an issue on Github about this for later.
    "but Jack, why not just optimize it now?" because IDIOT, all these issues stem from me being lazy, and I haven't gotten any less lazy in the 5 minutes it took me to type this.
    */
    void boardManager::displayTUI(){

        int maxLen = 0;
        
        vector<vector<string>> map(4*(2*mapSize+1), vector<string>(mapSize*4)); 

        for (auto&[cube, tile] : board) {
            tile.applyColor();
            if(cube.y % 2 == 0){
                


                //NOTE: using x relativity means that the x positions actually come out to X.5 for the in between tiles, use this to combine all of this into one efficient system

                //NOTE: make sub-function which uses X value to determine weather the string should be appended to the front or back 

                int Xrelative = cube.x + (cube.y/2);                    //maybe integrate into function 
                //starts at 0 on the top
                map[4 * (mapSize - Xrelative)][mapSize + cube.y].append(tile.tileTop);
                map[4 * (mapSize - Xrelative) + 1][mapSize + cube.y].append(tile.tileTMid);   //brutes forces strings into columns becasue I'm too lazy to make well designed code and it works 
                map[4 * (mapSize - Xrelative) + 2][mapSize + cube.y].append(tile.tileBMid);
                map[4 * (mapSize - Xrelative) + 3][mapSize + cube.y].append(tile.tileBottom); 

               // cout << to_string(cube.x) + "," + to_string(cube.y) + "," + to_string(cube.z);
               // cout << "\n";
            } else {
                float yfloat = cube.y;
                float Xrelative = cube.x + (yfloat/2) -0.25;                    //maybe integrate into function 
                //starts at 0 on the top
                if ((4 * (mapSize - Xrelative) - 1) > 0){
                    map[(4 * (mapSize - Xrelative) - 1)][mapSize + cube.y].append(tile.tileTop);
                    map[(4 * (mapSize - Xrelative)    )][mapSize + cube.y].append(tile.tileTMid);
                    map[(4 * (mapSize - Xrelative) + 1)][mapSize + cube.y].append(tile.tileBMid);
                    map[(4 * (mapSize - Xrelative) + 2)][mapSize + cube.y].append(tile.tileBottom); 
                }
                else cout << Xrelative;


            }
        }
        // you know what this does ;)

        vector<string> MMap(4*(2*mapSize+1));
        
        for(int i = 0; i < map.size(); i++){
            for (const auto& s : map[i]){
                MMap[i].append(s);
            }
            
        }


        for (const auto& s : MMap) {
            if ((int)displayWidth(s) > maxLen)
                maxLen = displayWidth(s);
        }

                

        for (auto& s : MMap) {
            int diffy = maxLen - displayWidth(s);          // total padding needed
            int left = diffy / 2;
            int right = diffy - left;

            s = std::string(left, ' ') + s + std::string(right, ' ');
        }

       // cout << map.size();
        for (auto&string: MMap){
            cout << string;
            cout << "\n";
        }

            

    };

    int shuffle(int x, int y) { //probably should go somewhere else or be integrated into assignBiome but idk
        static std::random_device rd; //random seed
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(x, y);

        return dist(gen);
    };
    
    //returns the length of a string without assigning multiple characters to mutli-byte characters
    size_t boardManager::displayWidth(const std::string& s) {                  
        size_t len = 0;
        const unsigned char* bytes = (const unsigned char*)s.data();
        size_t i = 0;

        while (i < s.size()) {

            // -----------------------
            // Skip ANSI escape codes
            // -----------------------
            if (bytes[i] == 0x1B) {       // ESC = \033 = 0x1B
                i++;                      // skip ESC
                if (i < s.size() && bytes[i] == '[') {
                    i++;                  // skip '['
                    // skip until final letter in CSI sequence
                    while (i < s.size() && 
                        !(bytes[i] >= '@' && bytes[i] <= '~')) {
                        i++;
                    }
                    if (i < s.size()) i++; // skip final letter ('m' or others)
                    continue;              // escape sequence counted as width 0
                }
            }

            // -----------------------
            // Count UTF-8 character normally
            // -----------------------
            unsigned char c = bytes[i];

            if (c < 0x80)           i += 1;      // ASCII
            else if ((c >> 5) == 0b110) i += 2;  // 2-byte UTF-8
            else if ((c >> 4) == 0b1110) i += 3; // 3-byte UTF-8
            else if ((c >> 3) == 0b11110) i += 4;// 4-byte UTF-8
            else                        i += 1;  // fallback

            len++;
        }

        return len;
    }
    




















































































    Tile **tileCreateLine(unsigned height, unsigned width)
    {
        Tile **two_d = 0;
        string tileCreate(bool);
        two_d = new Tile *[height];
        bool alt = false;

        for (int h = 0; h < height; h++)
        {
            two_d[h] = new Tile[width];
            for (int w = 0; w < width; w++)
            {
                two_d[h][w];
                two_d[h][w].X = h;
                two_d[h][w].Y = w; 
            }
        }

        return two_d;
    }


    string **tileAssignBiome(unsigned height, unsigned width, int **biomes)
    {
        string tileCreate(bool);
        for (int h = 0; h < height; h++)
        {
            for (int w = 0; w < width; w++)
            {

                if ((h % 2 == 0) && (w % 2 == 0) && h < 28)
                {
                    switch (tilemap[h / 2][w].Biome)
                    {
                    case 1: // if the tile's biome is set to 1 set the tile to plains
                        map[h][w] = "\033[92m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 1][w] = "\033[92m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 2: // if the tile's biome is set to 2 set the tile to forest
                        map[h][w] = "\033[32m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 1][w] = "\033[32m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 3: // if the tile's biome is set to 3 set the tile to mountains

                        map[h][w] = "\033[90m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 1][w] = "\033[90m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 4: // if the tile's biome is set to 4 set the tile to desert
                        map[h][w] = "\033[33m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 1][w] = "\033[33m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 5: // if the tile's biome is set to  5 set the tile to snow
                        map[h][w] = "\033[1;37m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 1][w] = "\033[1;37m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 6: // if the tile's biome is set to 6 set the tile to water
                        map[h][w] = "\033[1;34m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 1][w] = "\033[1;34m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;
                    }
                }
                else if ((h % 2 == 0) && (w % 2 != 0) && h < 27)
                {
                    switch (tilemap[h / 2][w].Biome)
                    {
                    case 1:
                        map[h + 1][w] = "\033[92m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 2][w] = "\033[92m" + tilemap[h / 2][w].tileBottom + "\033[0m";

                    case 2:
                        map[h + 1][w] = "\033[32m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 2][w] = "\033[32m" + tilemap[h / 2][w].tileBottom + "\033[0m";

                        break;

                    case 3:

                        map[h + 1][w] = "\033[90m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 2][w] = "\033[90m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 4:
                        map[h + 1][w] = "\033[33m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 2][w] = "\033[33m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 5:
                        map[h + 1][w] = "\033[1;37m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 2][w] = "\033[1;37m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;

                    case 6:
                        map[h + 1][w] = "\033[1;34m" + tilemap[h / 2][w].tileTop + "\033[0m";
                        map[h + 2][w] = "\033[1;34m" + tilemap[h / 2][w].tileBottom + "\033[0m";
                        break;
                    }
                }
                if (((w % 2 != 0) && (h == 0)) || ((w % 2 == 0) && (h == 28)))
                {
                    map[h][w] = "    ";
                }
                std::cout << map[h][w];
            }
            std::cout << std::endl;
        }
        std::cout << "\n";
        return map;
    }

    int **cityGenerator(unsigned height, unsigned width)
    {
        int poonta = 0;
        int **two_d = 0;
        two_d = new int *[height];
        bool alt = false;

        for (int h = 0; h < height; h++)
        {
            two_d[h] = new int[width];

            for (int w = 0; w < width; w++)
            {
                two_d[h][w] = poonta;
            }
            std::cout << "\n";
        }

        return two_d;
    }

    string **tileCreate(unsigned height, unsigned width)
{


    string **map = 0;


    map = new string *[height];





    for (int h = 0; h < height; h++)

    {


        map[h] = new string[width];


        for (int w = 0; w < width; w++)


        {


            map[h][w] = "";
        }

    }
    return map;
}
}