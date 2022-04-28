#include "unity.h"

void printFileContent(libSCG *scg, string path, int sizeX, int sizeY)
{
	string line;
	ifstream file(path);
	int counter = 0;
	
	if (!file.is_open())
	{
		scg->execute("text;" + to_string((int)(sizeX)) + ";" + to_string((int)(sizeX * 0.35 + 1)) + ";" + to_string(counter), "[CAN`T OPEN FILE]");
		return ;
	} else if (!access(path.c_str(), X_OK)) {
		scg->execute("text;" + to_string((int)(sizeX)) + ";" + to_string((int)(sizeX * 0.35 + 1)) + ";" + to_string(counter), "[EXECUTABLE FILE]");
		return ;
	}

	while (getline(file, line) && counter < sizeY)
	{
		scg->execute("text;" + to_string((int)(sizeX)) + ";" + to_string((int)(sizeX * 0.35 + 1)) + ";" + to_string(counter), line);
		counter++;
	}
}

void print1Col(list<string> *contentDir, libSCG *scg, int cursorRow, int sizeX, int sizeY)
{
	int counter = 0;
	int fstDir = 0;
	int lastDir = sizeY;
	// Clear first column if you scroll column to up or down
	if (sizeY > contentDir->size())
		lastDir = contentDir->size();
	if (sizeY - cursorRow <= 1)
	{
		// Clear first column
		scg->execute("rect;true;0;0;" + to_string((int)(sizeX * 0.15 + 1)) + ";" + to_string(sizeY), "  ");
		if (sizeY - cursorRow <= 0)
		{
			fstDir = cursorRow - sizeY;	
			lastDir = fstDir + sizeY + 1;
		}
	}
	for (auto i = next(contentDir->begin(), fstDir); i != next(contentDir->begin(), lastDir); ++i)
	{
		string fileCopy = *i;
		string file = *i;
		file.erase(0, fileCopy.find_last_of("/"));	
		if (is_directory(fileCopy)) { scg->execute("fgcolor;255;255;0"); }
		if (cursorRow - fstDir == counter) { scg->execute("bgcolor;0;128;192"); }
		if (sizeY - cursorRow > 0)
		{
			scg->execute("text;" + to_string((int)(sizeX * 0.35) - 1) + ";0;" + to_string(counter), file);
		} else {
			scg->execute("text;" + to_string((int)(sizeX * 0.35) - 1) + ";0;" + to_string(counter - 1), file);
		}
		if (is_directory(fileCopy)) { scg->execute("fgcolor;255;255;255"); }
		if (cursorRow - fstDir == counter) {scg->execute("bgcolor;0;0;0"); }

		counter++;
	}
}

void print2Col(string path, libSCG *scg, bool hidenFiles, int cursorRow, int sizeX, int sizeY)
{
	int counter = 0;
	if (access(path.c_str(), R_OK) == 0)
	{
		for (auto& entry : filesystem::directory_iterator(path))
		{
			if (counter > sizeY) break;
			string file = string(entry.path());
			string fileCopy = file;
			file.erase(0, file.find_last_of("/"));
			if (file.find_first_of(".") == 1 && !hidenFiles)
			{
				continue;
			}
			if (file.find_first_of(".") == 0 && !hidenFiles) continue;
			if (is_directory(fileCopy)) { scg->execute("fgcolor;255;255;0"); }
			scg->execute("text;" + to_string((int)(sizeX * 0.35) - 1) + ";" + to_string((int)(sizeX * 0.15) + 3)  + ";" + to_string(counter), file);	
			if (is_directory(fileCopy)) { scg->execute("fgcolor;255;255;255"); }
			counter++;
		}
	} else {
		scg->execute("bgcolor;255;0;0");
		scg->execute("text;" + to_string((int)(sizeX * 0.35)) + ";" + to_string((int)(sizeX * 0.15) + 3)  + ";0", "[ACCESS DENIED]");
		scg->execute("bgcolor;0;0;0");
	}
}

void updateDir(list<string> *contentDir, libSCG *scg, string path, bool hidenFiles, int sizeX, int sizeY)
{
	if (access(path.c_str(), R_OK) == 0)
	{
		contentDir->clear();
		// Clear 1 and 2 column
		scg->execute("rect;true;0;0;" + to_string((int)(sizeX * 0.15 + 1)) + ";" + to_string(sizeY), "  ");
		scg->execute("rect;true;" + to_string((int)(sizeX * 0.15) + 3) + ";0;" + to_string(sizeX / 3 + 1) + ";" + to_string(sizeY - 1), "  ");
		// Add path to top dir
		contentDir->push_front("/..");
		// Add paths to internal dirs
		for (auto& entry : filesystem::directory_iterator(path))
		{
			string file = entry.path();
			file.erase(0, file.find_last_of("/"));	
			if (file.find_first_of(".") == 1 && !hidenFiles)
			{
				continue;
			}
			contentDir->push_back(string(entry.path()));
		}
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
