/*
 * ToDo list:
 * [x] Make adaptive window
 * [x] View files in select dir
 * [x] Remove bugs with adaptive window
 * [x] Remove bug with view file in top dir 
 */

#include <iostream>
#include <filesystem>
#include <list>
#include <iterator>
#include <bits/stdc++.h>

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
	scg.execute("rect;  ;true;0;0;" + to_string(sizeX - 1) + ";" + to_string(sizeY - 1));
	scg.execute("line;| ;" + to_string((int)(sizeX * 0.15) + 2) + ";0;" + to_string((int)(sizeX * 0.15) + 2) + ";" + to_string(sizeY));
	scg.execute("line;| ;" + to_string(sizeX / 3 + 2) + ";0;" + to_string(sizeX / 3 + 2) + ";" + to_string(sizeY));
	list <string> contentDir;
	string currentPath = string(current_path()) + "/";

	int cursorRow = 0;
	
	updateDir(&contentDir, &scg, currentPath, sizeX, sizeY);
	print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
	
	bool quit = false;

	while (!quit)
	{
		int action = getch();
		switch (action)
		{
			case 106: // Cursor down
				if (cursorRow < contentDir.size() - 1)
				{
					moveCursorRow(&cursorRow, 1);
					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
					// Print internal folder
					scg.execute("rect;  ;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY - 1));
					if (is_directory(*next(contentDir.begin(), cursorRow)))
					{
						string path = *next(contentDir.begin(), cursorRow);
						if (path == "/..")
						{
							path.erase(path.find_last_of("/"), path.length() + 1);
							if (path == "") path = "/";
						}
						print2Col(path, &scg, cursorRow, sizeX, sizeY);
					}
				}
				break;
			case 107: // Cursor up
				if (cursorRow > 0)
				{
					moveCursorRow(&cursorRow, -1);
					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
					// Print internal folder
					scg.execute("rect;  ;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY - 1));
					if (is_directory(*next(contentDir.begin(), cursorRow)))
					{
						string path = *next(contentDir.begin(), cursorRow);
						if (path == "/..")
						{
							path = currentPath;
							path = path.erase(path.find_last_of("/"), path.length() + 1);
							if (path == "") path = "/";

						}
						print2Col(path, &scg, cursorRow, sizeX, sizeY);
					}
				}
				break;
			case 10: // Enter in dir
				if (is_directory(*next(contentDir.begin(), cursorRow)))
				{
					scg.execute("rect;  ;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY - 1));
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

