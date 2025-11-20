#include <list>
#include <iostream>
using namespace std;

namespace tiling
{
    class City
    {
    public:
        int Xpos;
        int Ypos;
        int population = 10;
        string owner;
    };

    class Tile
    {
    public:
        // attributes
        string tileTop = "/‾‾\\";
        string tileBottom = "\\__/";
        int Biome;
        int X;
        int Y;
        // methods
        string tileInspect;
    };

    class gameSpace
    {
    public:
        //This game uses a hexagonal array to store info since the grid is hexagonal. The array is controlled by three variables X, Y, and Z.
        
        // In hexagonal arrays, when one variable is held at a constant value, the other two variables are inversely proportional with equal magnitudes. These
        // arrays require [0,0,0] to be at the center, as opposed to a traditional 2d array which starts at the corner. Since the two non-constant variables in a 
        // hexagonal array have this relationship, and the center point must be [0,0,0], we know that the three coordinates of the array must always sum to 0. Because
        // of this, there are some coordinate points like [1,1,1] which do not exist 


        //For example: Let's say you start at position 0,0,0 of the array (center), moving to a position where x = 1 will require either Y or Z to be -1. As you have 
        // probably already figured, this means that there are two different tiles where x = : [1,-1,0] & [1,0,-1]
        

        int X;
        int Y;
        int Z;
        /*
        
        Stuff goes here

        */


        
    };



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

    int **biomes;
    int **citymap;
    string **BiomeMap;
    string **map;
    list<City> cities;
    Tile **tilemap = tileCreateLine(15,31);


    int **biomeGenerator(unsigned height, unsigned width) // determines which tiles will be allocated to which biome 1 = plains 2 = forest 3 = mountains 4 = desert 5 = snow 6 = water
    {
        int **biomeMap = 0;

        int randList[20];

        biomeMap = new int *[height];

        for (int h = 0; h < height; h++)
        {
            biomeMap[h] = new int[width];

            for (int w = 0; w < width; w++)
            {

                biomeMap[h][w] = 0;
                for (int r = 0; r < 20; r++) // gets 20 random numbers and puts them into a list
                {
                    srand(time(NULL) * (r + 1) * (h + 1) * (w + 1)); // uses 3 variables to scramble the rand seed so that rands outputs aren't repetetive

                    int randNum = rand() % 7;
                    if (randNum != 0)
                    {
                        randList[r] = randNum;
                    }
                }
                for (int biome = 1; biome < 5; biome++) // iterates through each Biome
                {
                    if (h > 1)
                    {
                        if (biomeMap[h - 1][w] == biome) // checks the biome value of surrounding tiles, if a surrounding tile has a biome value, it will take that biome value and replace a random other biome value on the random list. This helps simulate weighted randomness
                        {
                            for (int r = 0; r < 20; r++)
                            {
                                if (randList[r] != biome)
                                {
                                    randList[r] = biome;
                                    break;
                                }
                            }
                        }
                    }
                    else if (h < 14)
                    {
                        if (biomeMap[h][w] == biome)
                        {
                            for (int r = 0; r < 20; r++)
                            {
                                if (randList[r] != biome)
                                {
                                    randList[r] = biome;
                                    break;
                                }
                            }
                        }
                    }
                    else if (h > 1 && w > 1)
                    {
                        if (biomeMap[h - 1][w - 1] == biome)
                        {
                            for (int r = 0; r < 20; r++)
                            {
                                if (randList[r] != biome)
                                {
                                    randList[r] = biome;
                                    break;
                                }
                            }
                        }
                    }
                    else if (h > 1 && w < 14)
                    {
                        if (biomeMap[h - 1][w + 1] == biome)
                        {
                            for (int r = 0; r < 20; r++)
                            {
                                if (randList[r] != biome)
                                {
                                    randList[r] = biome;
                                    break;
                                }
                            }
                        }
                    }
                    else if (h < 14 && w < 1)
                    {
                        if (biomeMap[h + 1][w - 1] == biome)
                        {
                            for (int r = 0; r < 20; r++)
                            {
                                if (randList[r] != biome)
                                {
                                    randList[r] = biome;
                                    break;
                                }
                            }
                        }
                    }
                    else if (h < 14 && w < 14)
                    {
                        if (biomeMap[h + 1][w + 1] == biome)
                        {
                            for (int r = 0; r < 20; r++)
                            {
                                if (randList[r] != biome)
                                {
                                    randList[r] = biome;
                                    break;
                                }
                            }
                        }
                    }
                }
                tilemap[h][w].Biome = randList[rand() % 20]; // sets the tile Biome to a random number in the list
            }
        }
        return biomeMap;
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