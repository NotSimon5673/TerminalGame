#ifndef COMMANDS_H
#define COMMANDS_H

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

    class cmdManager // I don't know if a class is actually even needed here, maybe just make this a namespace with funcs inside
    {
    public:

        //base commands, will be added along with the command system itself
        vector<string> cmds;
        void listCmds();
        void settings();

        //game commands, will be added in a saving overhaul branch
        void saveGame();
        void newGame();
        void loadGame();

        //some basic game commands, this will probably be the longest set of commands by far but I'd like to get the basic systems done first
        void inspect();
        void display();


        


    };

#endif
