#include <list>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    int population;
    string owner;
};
int **biomes;
int **citymap;
string **BiomeMap;

string **tileCreateLine(unsigned height, unsigned width)
{
    string **two_d = 0;
    string tileCreate(bool);
    two_d = new string *[height];
    bool alt = false;

    for (int h = 0; h < height; h++)
    {
        two_d[h] = new string[width];

        for (int w = 0; w < width; w++)
        {

            if ((w % 2 != 0) && h != 0)
            {
                if (h % 2 == 0)
                {
                    two_d[h][w] = "\\__/";
                }
                else
                {
                    two_d[h][w] = "/‾‾\\";
                }
            }
            else if ((w % 2 == 0) && h != 28)
            {
                if (h % 2 == 0)
                {
                    two_d[h][w] = "/‾‾\\";
                }
                else
                {
                    two_d[h][w] = "\\__/";
                }
            }
            else
            {
                two_d[h][w] = "    ";
            }
            std::cout << two_d[h][w];
        }
        std::cout << "\n";
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
            for (int biome = 1; biome < 5; biome++)
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
            biomeMap[h][w] = randList[rand() % 20];

            std::cout << biomeMap[h][w];
        }
        std::cout << "\n";
    }
    return biomeMap;
}

string **tileAssignBiome(unsigned height, unsigned width, string **tileMap, int **biomes)
{
    string **map = tileMap;
    string tileCreate(bool);
    /*
        ATTENTION!

        This function is terrible. Needs optimization

        Note: instead of using it's position in the map, checks it's value to see if it is euqal to "/‾‾\\"
    */
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            if ((h > 1 && h < 29))
            {
                if (biomes[h / 2][w] == 1)
                {
                    if (w % 2 == 0)
                    {
                        map[h][w] = "\033[92m" + map[h][w] + "\033[0m"; // sets the tile color to yellow; indicates desert

                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[92m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                    else
                    {
                        map[h][w] = "\033[92m" + map[h][w] + "\033[0m";

                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[92m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                }
                else if (biomes[h / 2][w] == 2)
                {
                    if (w % 2 == 0)
                    {
                        map[h][w] = "\033[32m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[32m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                    else
                    {
                        map[h][w] = "\033[32m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[32m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                }

                else if (biomes[h / 2][w] == 3)
                {
                    if (w % 2 == 0)
                    {
                        map[h][w] = "\033[90m" + map[h][w] + "\033[0m";
                        if (w % 2 != 0)
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[90m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                    else
                    {
                        map[h][w] = "\033[90m" + map[h][w] + "\033[0m";
                        if (w % 2 != 0)
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[90m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                }

                else if (biomes[h / 2][w] == 4)
                {
                    if (w % 2 == 0)
                    {
                        map[h][w] = "\033[33m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[33m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                    else
                    {
                        map[h][w] = "\033[33m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[33m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                }

                else if (biomes[h / 2][w] == 5)
                {
                    if (w % 2 == 0)
                    {
                        map[h][w] = "\033[1;37m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[1;37m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                    else
                    {
                        map[h][w] = "\033[1;37m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[1;37m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                }

                else if (biomes[h / 2][w] == 6)
                {
                    if (w % 2 == 0)
                    {
                        map[h][w] = "\033[1;34m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[1;34m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                    else
                    {
                        map[h][w] = "\033[1;34m" + map[h][w] + "\033[0m";
                        if ((h % 2 == 0 && w % 2 == 0) || (h % 2 != 0 && w % 2 != 0))
                        {
                            // checks if the tile is top or bottom and if it's top, color the tile below
                            map[h + 1][w] = "\033[1;34m" + map[h + 1][w] + "\033[0m";
                        }
                    }
                }

                std::cout << map[h][w];
            }
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
    string myText;
    int **biomeMap = 0;

    // Read from the text file
    std::ifstream infile("saves/biomeMap.msf");
    int startGame();
    biomeMap = new int *[15];
    // Use a while loop together with the getline() function to read the file line by line

    string chr;

    for (int h = 0; h < 15; h++)
    {
        biomeMap[h] = new int[31];

        for (int w = 0; w < 31; w++)
        {
            getline(infile, myText);
            // Print current character
            biomeMap[h][w] = stoi(myText);
            biomes = biomeMap;
            std::cout << biomeMap[h][w] << endl;
        }
    }

    // Close the file
    infile.close();

    int start = startGame();
    return biomeMap;
}

int newGame()
{
    biomes = biomeGenerator(15, 31);
    citymap = cityGenerator(15, 31);
    int startGame();
    int start = startGame();
    return 0;
}

string menu()
{

    string input;
    string e;
    int newGame();
    int **Load();
    while (true)
    {
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
            std::cout << "NOT A VALID COMMAND" << std::endl;
            std::cin >> e;
        }
    }

    return 0;
}

int startGame()
{
    int play();
    // map is made 14x31
    string **tileMap = tileCreateLine(29, 31);
    string **map = tileAssignBiome(29, 31, tileMap, biomes);
    int game = play();
    return 0;
}

int play()
{
    using cmds::Gamecommands;
    while (true)
    {
        // system("clear");
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
            std::cout << "type the coordinates of the tile you'd like to settle on" << endl;
            std::cin >> X;
            std::cin >> Y;

            cmd.settle(X, Y);
            City myCity;
            myCity.Xpos = X;
            myCity.Ypos = Y;
            citymap[X][Y] = 1;
        }
    }
}

int main()
{
    menu();
}