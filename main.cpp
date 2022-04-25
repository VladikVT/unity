/*
 * ToDo list:
 * [x] Moving cursor rows
 * [x] Scrolling rows
 * [x] Moving on dirs 
 */

#include <iostream>
#include <filesystem>
#include <list>
#include <iterator>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "libs/libSCG.h"
#include "libs/unity.h"

using namespace std;
using namespace std::filesystem;

int main()
{
	int sizeX = 83;
	int sizeY = 32;
	libSCG scg(sizeX, sizeY, false, true);
	scg.execute("bgcolor;0;0;0");
	scg.execute("rect;  ;true;0;0;" + to_string(sizeX--) + ";" + to_string(sizeY--));
	scg.execute("line;||;12;0;12;" + to_string(sizeY));

	list <string> contentDir;
	string currentPath = string(current_path());

	int cursorCol = 0;
	int cursorRow = 0;
	
	updateDir(&contentDir, &scg,currentPath, sizeY);
	print1Col(&contentDir, &scg, cursorRow, sizeY);
	
	bool quit = false;

	while (!quit)
	{
		int action = getch();
		switch (action)
		{
			case 106: // Cursor up
				if (cursorRow < contentDir.size() - 1)
				{
					moveCursorRow(&cursorRow, 1);
					print1Col(&contentDir, &scg, cursorRow, sizeY);
				}
				break;
			case 107: // Cursor down
				if (cursorRow > 0)
				{
					moveCursorRow(&cursorRow, -1);
					print1Col(&contentDir, &scg, cursorRow, sizeY);
				}
				break;
			case 10: // Enter in dir
				if (is_directory(*next(contentDir.begin(), cursorRow)))
				{
					// Entering to top dir
					if (*next(contentDir.begin(), cursorRow) == "/..")
					{
						// Clear all after last '/'
						// Exapmle: "home/arch/folder" --> "home/arch"
						currentPath.erase(currentPath.find_last_of("/"), currentPath.length() + 1);
						// If you in root dir
						if (currentPath == "") currentPath = "/";
					// Entering in internal dir
					} else {
						currentPath = *next(contentDir.begin(), cursorRow);
					}
					cursorRow = 0;
					updateDir(&contentDir, &scg, currentPath, sizeY);
					print1Col(&contentDir, &scg, cursorRow, sizeY);
				}
				break;
			case 113: // Quit
				quit = true;
				break;
			default:
				break;
		}
	}

	return 0;
}

