#include <fstream>
#include <iostream>
#include "menuCommands.hpp"
#include "gameCommands.hpp"

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

using namespace std;

namespace cmds
{
    class menuCommands
    {
        // string settings() {}
        // string tutorial() {}
    };

    class Gamecommands
    {
    public:
        string scree = "eodmdmdm";

        string commands()
        {
            string command = "help \n inspect \n save \n endTurn";
            std::cout << command;
            return command;
        }
        // string help(command) {}
        int save(int **biomeMap, int **citiesMap)
        {
            ofstream outfile("saves/biomeMap.msf");
            ofstream file("saves/citiesMap.msf");

            // outfile.open("saves/biomeMap.msf", std::ios::trunc);
            //  file.open("CoN/saves/citiesMap.msf", std::ios::trunc);
            for (int i = 0; i < 15; i++)
            {
                for (int j = 0; j < 31; j++)
                {
                    outfile << biomeMap[i][j];
                    outfile << "\n";
                    file << citiesMap[i][j] << endl;
                }
            }

            outfile.close();
            file.close();
            std::cout << "Game Saved" << endl;
            cin >> scree;
            return 0;
        }

        int settle(int x, int y)
        {
            // this doesn't do anything. Oh well!
        }
    };

}

#endif