
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "menuCommands.hpp"
#include "gameCommands.hpp"
#include "commands.cpp"
#include "tiling.cpp"
using namespace std;
using namespace cmds;
using namespace tiling;

int play()
{
    using cmds::Gamecommands;

    
    while (true)
    {
        string **BiomeMap;
        // clear the screen and then print out the grid
        //system("clear");
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

int startGame()
{
    /*
    This Function simply exists to bring the Load and newGame functions right back to the same spot
    */

    // map is made 14x31

    string **map = tileAssignBiome(29, 31, biomes); // uses the biomeMap and the tile objects to create a grid with biomes attached to them and then print it which ended up being A LOT harder than it sounds.
    int game = play();
    return 0;
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
    int start = startGame();
    return 0;
}

string menu()
{
    string input;
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
        else if (input == "test")
        {
            boardManager testManager;
            testManager.mapSize = 4;
            cout << testManager.board[{-1,3,-2}].tileBottom;

            char temp = getchar();
            char ch = getchar(); // I could try to find a better solution but why even bother. calling this function twice works perfectly fine
            cout << ch << endl;
        }
        else
        {
            // if the user types a non valid command, Display the invalid command meassage and wait for the user to press any button
            std::cout << "NOT A VALID COMMAND" << std::endl;
            char temp = getchar(); //because the enter key is used to submit the command via cin, this will detect that as a char input and get skipped.
            char ch = getchar(); // I could try to find a better solution but why even bother. calling this function twice works perfectly fine
            cout << ch << endl;
        }
    }

    return 0;
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
    //cout << "population:" + tilemap[x][y].tileCity.population << endl;             Deprecated. Not worth putting any effort into making a temp quickfix for this
    return "";
}

int main()
{
    menu();
}