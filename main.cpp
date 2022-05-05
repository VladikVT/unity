/*
 * ToDo list:
 * [x] Make command line
 * [ ] Make create file command
 * [ ] Make delete file command
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
	int sizeYcopy;
	setvbuf(stdout, NULL, _IONBF, 0);
	fputs("\x1B[255;255H\x1B[6n", stdout);
	scanf("\x1B[%d;%dR", &sizeY, &sizeX);
	sizeX = sizeX / 2 - 2;
	sizeY = sizeY - 4;
	sizeYcopy = sizeY;

	libSCG scg(sizeX, sizeY, false, true);
	sizeY -= 3;
	scg.execute("bgcolor;0;0;0");
	scg.execute("rect;true;0;0;" + to_string(sizeX - 1) + ";" + to_string(sizeY + 2), "  ");
	scg.execute("line;" + to_string((int)(sizeX * 0.15) + 2) + ";0;" + to_string((int)(sizeX * 0.15) + 2) + ";" + to_string(sizeY),  "│ ");
	scg.execute("line;" + to_string(sizeX / 3 + 2) + ";0;" + to_string(sizeX / 3 + 2) + ";" + to_string(sizeY), "│ ");
	scg.execute("line;0;" + to_string(sizeY + 1) + ";" + to_string(sizeX) + ";" + to_string(sizeY + 1), "──");
	list <string> contentDir;
	string currentPath = string(current_path()) + "/";

	int cursorRow = 0;
	bool col3flag = false;
	bool hidenFiles = false;

	updateDir(&contentDir, &scg, currentPath, hidenFiles, sizeX, sizeY);
	print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
	
	bool commandMode = false;
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
					// Clear 2 and 3 column
					scg.execute("rect;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY), "  ");
					if (col3flag)
					{
						scg.execute("rect;true;" + to_string(sizeX / 3 + 3) + ";0;" + to_string(sizeX) + ";" + to_string(sizeY), "  ");
						col3flag = false;
					}
					// Print internal folder
					if (is_directory(*next(contentDir.begin(), cursorRow)))
					{
						string path = *next(contentDir.begin(), cursorRow);
						if (path == "/..")
						{
							path.erase(path.find_last_of("/"), path.length() + 1);
							if (path == "") path = "/";
						}
						print2Col(path, &scg, hidenFiles, cursorRow, sizeX, sizeY);
					}
				}
				printMetadata(&scg, *next(contentDir.begin(), cursorRow), sizeX, sizeY);
				break;
			case 107: // Cursor up
				if (cursorRow > 0)
				{
					moveCursorRow(&cursorRow, -1);

					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
					// Clear 2 and 3 column
					scg.execute("rect;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY), "  ");
					if (col3flag)
					{
						scg.execute("rect;true;" + to_string((int)(sizeX / 3 + 3)) + ";0;" + to_string(sizeX) + ";" + to_string(sizeY), "  ");
						col3flag = false;
					}
					// Print internal folder
					if (is_directory(*next(contentDir.begin(), cursorRow)))
					{
						scg.execute("rect;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY - 1), "  ");
						string path = *next(contentDir.begin(), cursorRow);
						if (path == "/..")
						{
							path = currentPath;
							path = path.erase(path.find_last_of("/"), path.length() + 1);
							if (path == "") path = "/";

						}
						print2Col(path, &scg, hidenFiles, cursorRow, sizeX, sizeY);
					}
				}
				printMetadata(&scg, *next(contentDir.begin(), cursorRow), sizeX, sizeY);
				break;
			case 115: // Toggle show and hide hide files
				hidenFiles = !hidenFiles;
				updateDir(&contentDir, &scg, currentPath, hidenFiles, sizeX, sizeY);
				if (cursorRow >= contentDir.size())
				{
					cursorRow = contentDir.size() - 1;
				}
				print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
				if (col3flag)
				{
					scg.execute("rect;true;" + to_string(sizeX / 3 + 3) + ";0;" + to_string(sizeX) + ";" + to_string(sizeY), "  ");
					col3flag = false;
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
					updateDir(&contentDir, &scg, currentPath, hidenFiles, sizeX, sizeY);
					print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
				} else {
					col3flag = true;
					string path = *next(contentDir.begin(), cursorRow);
					printFileContent(&scg, path, sizeX, sizeY);
				}
				break;
			case 58: // Command mode
				commandMode = true;
				break;
			case 113: // Quit
				quit = true;
				break;
			default:
				break;
		}
		
		cout << "\33[2K\r";

		if (commandMode)
		{
			string command;
			cout << ":";
			getline(cin, command);
			cout << "\033[A\33[2K\r";
			commandMode = false;
			makeCommand(command, currentPath);
			updateDir(&contentDir, &scg, currentPath, hidenFiles, sizeX, sizeY);
			print1Col(&contentDir, &scg, cursorRow, sizeX, sizeY);
			continue;
		}
	}

	return 0;
}

