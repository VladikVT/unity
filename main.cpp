/*
 * ToDo list:
 * [x] Make adaptive window
 * [ ] View files in select dir
 */

#include <iostream>
#include <filesystem>
#include <list>
#include <iterator>

#include "libs/libSCG.h"
#include "libs/unity.h"

using namespace std;
using namespace std::filesystem;

int main(int argc, char **argv)
{
	int sizeX;
	int sizeY;
	setvbuf(stdout, NULL, _IONBF, 0);
	fputs("\x1B[255;255H\x1B[6n", stdout);
	scanf("\x1B[%d;%dR", &sizeY, &sizeX);

	sizeX = sizeX / 2 - 2;
	sizeY = sizeY - 3;
	libSCG scg(sizeX, sizeY, false, true);
	scg.execute("bgcolor;0;0;0");
	scg.execute("rect;  ;true;0;0;" + to_string(sizeX--) + ";" + to_string(sizeY--));
	scg.execute("line;||;" + to_string(sizeX / 5 + 2) + ";0;" + to_string(sizeX / 5 + 2) + ";" + to_string(sizeY));
	list <string> contentDir;
	string currentPath = string(current_path());

	int cursorCol = 0;
	int cursorRow = 0;
	
	updateDir(&contentDir, &scg, currentPath, sizeX, sizeY);
	print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
	
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
					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
				}
				break;
			case 107: // Cursor down
				if (cursorRow > 0)
				{
					moveCursorRow(&cursorRow, -1);
					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
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
					updateDir(&contentDir, &scg, currentPath, sizeX, sizeY);
					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
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

