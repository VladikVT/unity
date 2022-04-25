#include "unity.h"

void print1Col(list<string> *contentDir, libSCG *scg, int cursorRow, int sizeY)
{
	int counter = 0;
	// Clear first column if you scroll column to up or down 
	if (sizeY - cursorRow <= 0) 
		scg->execute("rect;  ;true;0;0;11;" + to_string(sizeY));
	for (string i : *contentDir)
	{
		string file = i;
		file.erase(0, i.find_last_of("/"));
		if (is_directory(i)) { scg->execute("fgcolor;255;255;0"); }
		if (cursorRow == counter) { scg->execute("bgcolor;0;128;192"); }
		if (sizeY - cursorRow > 0)
		{
			scg->execute("text;" + file + ";20;0;" + to_string(counter));
		} else {
			scg->execute("text;" + file + ";20;0;" + to_string(counter + (sizeY - cursorRow)));
		}
		if (is_directory(i)) { scg->execute("fgcolor;255;255;255"); }
		if (cursorRow == counter) {scg->execute("bgcolor;0;0;0"); }

		counter++;
	}
}

void updateDir(list<string> *contentDir, libSCG *scg, string path, int sizeY)
{
	contentDir->clear();
	// Clear first column
	scg->execute("rect;  ;true;0;0;11;" + to_string(sizeY));
	// Add path to top dir
	contentDir->push_front("/..");
	// Add paths to internal dirs
	for (auto& entry : filesystem::directory_iterator(path))
	{
		contentDir->push_back(string(entry.path()));
	}
}

void moveCursorRow(int *cursorRow, int direction)
{
	*cursorRow += direction;
}

int getch()
{
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
