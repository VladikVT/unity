#ifndef UNITY_H
#define UNITY_H
#include <iostream>

#include <filesystem>
#include <fstream>
#include <list>
#include <iterator>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include "libSCG.h"

using namespace std;
using namespace std::filesystem;

int getch();
void print1Col(list<string> *contentDir, libSCG *scg, int cursorRow, int sizeX, int sizeY);
void updateDir(list<string> *contentDir, libSCG *scg, string path, bool hidenFiles, int sizeX, int sizeY);
void moveCursorRow(int *cursorRow, int direction);
void print2Col(string path, libSCG *scg, bool hidenFiles, int cursorRow, int sizeX, int sizeY);
void printFileContent(libSCG *scg, string path, int sizeX, int sizeY);
void printMetadata(libSCG *scg, string path, int sizeX, int sizeY);
string byteConverter(int byte);

#endif // UNITY_H
