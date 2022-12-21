#include <list>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "menuCommands.hpp"
#include "gameCommands.hpp"
#include "commands.cpp"
using namespace std;
using namespace cmds;

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
    City tileCity;
    int Biome;
    int X;
    int Y;
    // methods
    string tileInspect;
};

Tile **tilemap;
int **biomes;
int **citymap;
string **BiomeMap;
string **map;
list<City> cities;

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

int **Load()
{
    /*
        takes an .msf(map save file) script and converts it into arrays that the program can use to fill in data

        WARNING: THIS FUNCTION DOES NOT WORK PROPERLY, IF YOU TRY TO USE IT WHILE PLAYING IT WILL ALMOST DEFINITELY CRASH
    */
    string Bmap;
    string Cmap;
    int **biomeMap = 0;

    // Read from the text file
    std::ifstream Bfile("saves/biomeMap.msf");
    std::ifstream Cfile("saves/citiesMap.msf");
    int startGame();
    biomeMap = new int *[15];

    string chr;

    for (int h = 0; h < 15; h++)
    {
        biomeMap[h] = new int[31];

        for (int w = 0; w < 31; w++)
        {
            getline(Bfile, Bmap);
            getline(Cfile, Cmap);

            // Print current character
            biomeMap[h][w] = stoi(Bmap);
            biomes = biomeMap;
            Bfile.close();
            Cfile.close();
            citymap[h][w] = stoi(Cmap);
            std::cout << biomeMap[h][w] << endl;
        }
    }

    int start = startGame();
    return biomeMap;
}

int newGame()
{
    map = tileCreate(29, 31);         // initialize a 'map' containing many tile objects
    tilemap = tileCreateLine(15, 31); // initialize the 2d array used for the tileassignbiome function
    biomes = biomeGenerator(15, 31);  // creates a 2d array of biomes based on weighted randomness
    citymap = cityGenerator(15, 31);  // creates a 2d array where every value is 0

    int startGame();
    int start = startGame();
    return 0;
}

string menu()
{
    char ch;
    string input;
    int newGame();
    int **Load();
    while (true)
    {
        // get rid of everything currently on the screen, then display the welcome message
        system("clear");
        std::cout << "Welcome to Conquest of Nations. \n to see a list of commands type commands." << std::endl;
        std::cin >> input;
        if (input == "createNew")
        {
            int createNew = newGame();
        }
        else if (input == "Load")
        {
            int **bioome = Load();
        }
        else
        {
            // if the user types a non valid command, Display the invalid command meassage and wait for the user to press any button
            std::cout << "NOT A VALID COMMAND" << std::endl;
            ch = getch();
            cout << ch << endl;
        }
    }

    return 0;
}

int startGame()
{
    /*
    This Function simply exists to bring the Load and newGame functions right back to the same spot
    */

    int play();
    // map is made 14x31

    string **map = tileAssignBiome(29, 31, biomes); // uses the biomeMap and the tile objects to create a grid with biomes attached to them and then print it which ended up being A LOT harder than it sounds.
    int game = play();
    return 0;
}

int play()
{
    using cmds::Gamecommands;
    while (true)
    {
        // clear the screen and then print out the grid
        system("clear");
        for (int i = 0; i > 15; i++)
        {
            for (int j = 0; j > 31; j++)
            {
                std::cout << BiomeMap[i][j];
            }
        }

        Gamecommands cmd;
        string command;
        int X;
        int Y;

        std::cin >> command;

        if (command == "save")
        {
            cmd.save(biomes, citymap);
        }

        if (command == "settle")
        {
            // all of this stuff should be done in the function that is meant to do all of this but tbh that would be way harder than just leaving it all out here
            std::cout << "type the coordinates of the tile you'd like to settle on" << endl;
            std::cin >> X;
            std::cin >> Y;

            cmd.settle(X, Y);
            City myCity;
            myCity.Xpos = X;
            myCity.Ypos = Y;
            citymap[X][Y] = 1;
            cities.assign(1, myCity);
        }

        if (command == "inspect")
        {
            string inspect(int x, int y);
            std::cout << "type the coordinates of the tile you'd like to inspect" << endl;
            std::cin >> X;
            std::cin >> Y;
            string tileInspect = inspect(X, Y);
        }
    }
}

string inspect(int x, int y)
{
    /*
        Just prints out a bunch of info about the tile

        Hopefully there will eventually be a lot more stuff to put under city info and it will all be displayed in a border(refer to borders/bea.txt)
    */
    cout << "Tile info:" << endl;
    cout << "Biome: " + tilemap[x][y].Biome << endl;
    cout << "City info:" << endl;
    cout << "population:" + tilemap[x][y].tileCity.population << endl;
    return "";
}

int main()
{
    menu();
}