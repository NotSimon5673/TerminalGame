#include <curses.h>
#include <list>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
    int ch;
    std::cout << "Test" << std::endl;
    ch = getch();
    cbreak();
    cout << ch << endl;
    return 0;
}