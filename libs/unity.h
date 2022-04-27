#ifndef UNITY_H
#define UNITY_H
#include <iostream>

#include <filesystem>
#include <list>
#include <iterator>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "libSCG.h"

using namespace std;
using namespace std::filesystem;

int getch();
void print1Col(list<string> *contentDir, libSCG *scg, int cursorRow, int sizeX, int sizeY);
void updateDir(list<string> *contentDir, libSCG *scg, string path, int sizeX, int sizeY);
void moveCursorRow(int *cursorRow, int direction);
void print2Col(string path, libSCG *scg, int cursorRow, int sizeX, int sizeY);

#endif // UNITY_H