#include <iostream>
#include <filesystem>
#include <list>
#include <iterator>
#include "libs/libSCG.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

int getch() {
	struct termios oldt,
	newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

void moveCur(int & chooseDir, int upORdown, libSCG & scg, list<string> & currentDir)
{
	scg.execute("dot;" + *next(currentDir.begin(), chooseDir) + ";0;" + to_string(chooseDir));
	if (chooseDir + upORdown < 0) {
		chooseDir = 0;
	} else if (chooseDir + upORdown > currentDir.size() - 1) {
		chooseDir = currentDir.size() - 1;
		cout << currentDir.size();
	} else {
		chooseDir += upORdown;
	}
	scg.execute("bgcolor;0;64;255");
	scg.execute("dot;" + *next(currentDir.begin(), chooseDir) + ";0;" + to_string(chooseDir));
	scg.execute("bgcolor;0;0;0");
}

void updateDir(list<string> & currentDir, string path, int & chooseDir, libSCG & scg, int sizeX, int sizeY)
{
	currentDir.erase(currentDir.begin(), currentDir.end());
	currentDir.push_front("..");
	scg.execute("clear");
	int counter = 0;
	cout << path << endl;
	for (auto& entry : filesystem::directory_iterator(path))
	{
		currentDir.push_back(string(entry.path()));
		scg.execute("dot;..;0;0");

		if (counter < sizeY - 1) scg.execute("dot;" + string(entry.path()) + ";0;" + to_string(counter + 1));
		counter++;
	}
	chooseDir = 0;
	scg.execute("bgcolor;0;64;255");
	scg.execute("dot;" + *next(currentDir.begin(), chooseDir) + ";0;0");
	scg.execute("bgcolor;0;0;0");
}

void selectDir(list<string> & currentDir, string & path, int & chooseDir)
{
	path = *next(currentDir.begin(), chooseDir);
}

int main()
{
	int sizeX = 83;
	int sizeY = 32;
	libSCG scg(sizeX, sizeY, false, true);

	int chooseDir = 0;

	list <string> currentDir;

	string path = "/";
	cin >> path;
	
	updateDir(currentDir, path, chooseDir, scg, sizeX, sizeY);

	getch();
	
	while(true)
	{
		int upORdown = getch();
		if (upORdown == 113 || upORdown == 106 || upORdown == 107 || upORdown == 10)
		{
			upORdown = upORdown == 107 ? -1 : upORdown;
			upORdown = upORdown == 106 ? 1 : upORdown;
			if (upORdown == 1 || upORdown == -1) {
				moveCur(chooseDir, upORdown, scg, currentDir);
			} else if (upORdown == 10) {
				if (*next(currentDir.begin(), chooseDir) == "..") {
					path.erase(path.find_last_of("/"), path.length());
				} else {
					path = *next(currentDir.begin(), chooseDir);
				}
				updateDir(currentDir, path, chooseDir, scg, sizeX, sizeY);
				cout << path << endl;
			} else if (upORdown == 113) {
				break;
			}

		}
	}
	return 0;
}
