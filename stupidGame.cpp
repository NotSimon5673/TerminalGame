
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "commands.cpp"
#include "tiling.h"
#include "tiling.cpp"
using namespace std;
using namespace tiling;


void menu()
{
    string input;
    while (true)
    {
        // get rid of everything currently on the screen, then display the welcome message
        system("clear");
        std::cout << "Welcome to TerminalGame. \nto see a list of commands type commands." << std::endl;
        std::cin >> input;


        if (input == "test")
        {
            system("clear");
            boardManager testManager(5);
            testManager.assignBiome();
            testManager.displayTUI();
            char temp = getchar();
            char ch = getchar();
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
}


int main()
{
    menu();
    return 0;
}




