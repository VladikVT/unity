#include "libSCG.h"

libSCG::libSCG(int sX, int sY, bool debug, bool btflWindow)
	: sizeX(sX), sizeY(sY), debug(debug), btflWindow(btflWindow)
{
	clearScreen;
	saveCursor;
	if ( btflWindow == true ) { cout << "\033[" << sizeY + 3 << ";" << 0 << "H"; }
	else { cout << "\033[" << sizeY + 1 << ";" << 0 << "H"; }
	printLogMsg("Start SCG", 0); 
	printLogMsg("Window size: " + to_string(sizeX) + "x" + to_string(sizeY), 0);
	loadCursor;
	cout << "\033[" << 0 << ";" << 0 << "H";
	drawWindow();

	if (debug == false)
	{
		if ( btflWindow == true ) { cout << "\033[" << sizeY + 3 << ";" << 0 << "H"; }
		else { cout << "\033[" << sizeY + 1 << ";" << 0 << "H"; }

		saveCursor;
	}

	loadCursor;
}

libSCG::~libSCG()
{
	printLogMsg("Stop SCG", 0);
}

// PUBLIC //

void libSCG::execute(string command, string symbol)
{
	string dlmtr = ";";
	size_t pos = 0;
	string token;
	string cmdCopy = command;
	
	int atrCount = 10;
	string cmdAttributes[atrCount];
	int counter = 0;
	
	while ((pos = command.find(dlmtr)) != string::npos)
	{
		token = command.substr(0, pos);
		if (counter < atrCount) 
		{ 
			cmdAttributes[counter] = token; 
		} else {
			printLogMsg("Allowed count of attributes in command '" + cmdCopy + "' has been exceeded", 2);
			return ;
		}
		command.erase(0, pos + dlmtr.length());
		counter++;
	}
	cmdAttributes[counter++] = command;
	// RECTANGLE //
	if (cmdAttributes[0] == "rect")
	{
		try {
			rectangle(symbol,
				cmdAttributes[1] == "true",
				stoi(cmdAttributes[2]), 
				stoi(cmdAttributes[3]),
				stoi(cmdAttributes[4]),
				stoi(cmdAttributes[5]));
		} catch (...) {
			printLogMsg("Command '" + cmdCopy + "' got not correct data!", 2);
			return ;
		}
	}
	// CHANGE FOREGRAUND COLOR //
	else if (cmdAttributes[0] == "fgcolor")
	{
		try {
			setColor(BGcolor[0], BGcolor[1], BGcolor[2], 
				stoi(cmdAttributes[1]),
				stoi(cmdAttributes[2]),
				stoi(cmdAttributes[3]));
		} catch (...) {
			printLogMsg("Command '" + cmdCopy + "' got not correct data!", 2);
			return ;
		}
	} 
	// CHANGE BACKGROUND COLOR //
	else if (cmdAttributes[0] == "bgcolor")
	{
		try {
			setColor(stoi(cmdAttributes[1]),
				stoi(cmdAttributes[2]),
				stoi(cmdAttributes[3]),
				FGcolor[0], FGcolor[1], FGcolor[2]);
		} catch (...) {
			printLogMsg("Command '" + cmdCopy + "' got not correct data!", 2);
			return ;
		}
	} 
	// DOT //
	else if (cmdAttributes[0] == "dot")
	{
		try {
			rectangle(symbol,
				true,
				stoi(cmdAttributes[1]), 
				stoi(cmdAttributes[2]),
				stoi(cmdAttributes[1]),
				stoi(cmdAttributes[2]));
		} catch (...) {
			printLogMsg("Command '" + cmdCopy + "' got not correct data!", 2);
			return ;
		}
	}
	// CIRCLE //
	else if (cmdAttributes[0] == "circle")
	{
		try {
			circle(symbol,
				stoi(cmdAttributes[1]), 
				stoi(cmdAttributes[2]),
				stoi(cmdAttributes[3]),
				cmdAttributes[4] == "true");
		} catch (...) {
			printLogMsg("Command '" + cmdCopy + "' got not correct data!", 2);
			return ;
		}
	}
	// LINE //
	else if (cmdAttributes[0] == "line")
	{
		try {
			line(symbol,
				stoi(cmdAttributes[1]), 
				stoi(cmdAttributes[2]),
				stoi(cmdAttributes[3]),
				stoi(cmdAttributes[4]));
		} catch (...) {
			printLogMsg("Command '" + cmdCopy + "' got not correct data!", 2);
			return ;
		}
	}
	// CLEAR //
	else if (cmdAttributes[0] == "clear")
	{
		clearScreen;
		drawWindow();
	}
	// TEXT //
	else if (cmdAttributes[0] == "text")
	{
		text(symbol,
				stoi(cmdAttributes[1]),
				stoi(cmdAttributes[2]),
				stoi(cmdAttributes[3]));
	}
}

// PRIVATE //

void libSCG::drawWindow()
{
	if (btflWindow == true)
	{
		for (int y = 0; y < sizeY + 2; y++)
		{
			for (int x = 0; x < sizeX + 2; x++)
			{
				if (y == 0 || y == sizeY + 1)
				{
					cout << "==";
				} else if (x == 0 || x == sizeX + 1) {
					cout << "||";
				} else {
					cout << "  ";
				}
			}
			cout << endl;
		}
	} else {
		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				cout << "[]";
			}
			cout << endl;
		}
	}	
}

void libSCG::printLogMsg(string msg, int err)
{
	if (debug == true || err == 2)
	{
		cout << "SCG[" + to_string(err) + "] >>> " + msg + "\n";
		saveCursor;
	}
}

void libSCG::writeSymbol(string symbol, int x, int y)
{
	if (0 <= x && x < sizeX && 0 <= y && y < sizeY)
	{
		if (btflWindow == true) { cout << "\033[" + to_string(y + 2) + ";" + to_string(x * 2 + 3) + "H"; }
		else { cout << "\033[" + to_string(y + 1) + ";" + to_string(x * 2 + 1) + "H"; }
		cout << "\033[38;2;" + to_string(FGcolor[0]) + ";" + to_string(FGcolor[1]) + ";" + to_string(FGcolor[2]) + "m"
			+ "\033[48;2;" + to_string(BGcolor[0]) + ";" + to_string(BGcolor[1]) + ";" + to_string(BGcolor[2]) + "m"
			+ symbol + "\033[0;0m";
		loadCursor;
	}
}

void libSCG::rectangle(string symbol, bool fill, int x1, int y1, int x2, int y2)
{
	if (x1 < 0 || y1 < 0 || x2 >= sizeX || y2 >= sizeY)
	{
		printLogMsg("Rectangle out of window", 1);
	}
	if (x1 > x2 || y1 > y2)
	{
		printLogMsg("Not correct rectangle coordinates", 2);
		return ;
	}
	
	if (fill == true)
	{
		for (int y = y1; y <= y2; y++)
		{
			for (int x = x1; x <= x2; x++)
			{
				writeSymbol(symbol, x, y);
			}
		}
	} else {
		for (int y = y1; y <= y2; y++)
		{
			for (int x = x1; x <= x2; x++)
			{
				if (y == 0 || x == 0 || y == y2 || x == x2)
				{
					writeSymbol(symbol, x, y);
				}
			}
		}
	}
	printLogMsg("Draw rectangle", 0);
}

void libSCG::setColor(int Br, int Bg, int Bb, int Fr, int Fg, int Fb)
{
	BGcolor[0] = Br;
	BGcolor[1] = Bg;
	BGcolor[2] = Bb;
	FGcolor[0] = Fr;
	FGcolor[1] = Fg;
	FGcolor[2] = Fb;
	printLogMsg("Color set", 0);
}

void libSCG::circle(string symbol, bool fill, int radius, int x, int y)
{
	if (x - radius < 0 || y - radius < 0 || x + radius >= sizeX || y + radius >= sizeY)
	{
		printLogMsg("Circle out of window", 1);
	}
	double thinknessIn = 0.2;
	if (fill == true) { thinknessIn = radius; }
	double thinknessOut = radius / 10.0 + 0.1;
	double rIn = radius - thinknessIn;
	double rOut = radius + thinknessOut;

	double x1, y1, z;

	for (int i = 0; i < radius * 2 + 1; i++)
	{
		for (int j = 0; j < radius * 2 + 1; j++)
		{
			x1 = j - radius;
			y1 = i - radius;
			z = sqrt(pow(x1, 2) + pow(y1, 2));
			if (rOut >= z && z >= rIn){
				if (x1 <= 0 && y1 >= 0)
				{
					writeSymbol(symbol, j + 1 + x - radius, i + y - radius);
				} else if (x1 > 0 && y1 > 0) {
					writeSymbol(symbol, j + x - radius, i + y - radius);
				} else if (x1 < 0 && y1 < 0) {
					writeSymbol(symbol, j + 1 + x - radius, i + 1 + y - radius);
				} else {
					writeSymbol(symbol, j + x - radius, i + 1 + y - radius);
				}
			}
		}
	}
}
// The Bresenham`s line algorithm is used
void libSCG::line(string symbol, int x1, int y1, int x2, int y2)
{
	if (x1 < 0 || y1 < 0 || x2 >= sizeX || y2 >= sizeY)
	{
		printLogMsg("Line out of window", 1);
	}
	int dx = (x2 - x1) >= 0 ? 1 : -1;
	int dy = (y2 - y1) >= 0 ? 1 : -1;
	int lengthX = abs(x2 - x1);
	int lengthY = abs(y2 - y1);
	int length = max(lengthX, lengthY);	

	if (length == 0) { writeSymbol(symbol, x2, y2); }
	if (lengthY <= lengthX)
	{
		int x = x1;
		int y = y1;
		int d = -lengthX;
		length++;
		while(length--)
		{
			writeSymbol(symbol, x, y);
			x += dx;
			d += 2 * lengthY;
			if (d > 0)
			{
				d -= 2 * lengthX;
				y += dy;
			}
		}
	} else {
		int x = x1;
		int y = y1;
		int d = -lengthY;
		length++;
		while(length--)
		{
			writeSymbol(symbol, x, y);
			y += dy;
			d += 2 * lengthX;
			if (d > 0)
			{
				d -= 2 * lengthY;
				x += dx;
			}
		}
	}
}

void libSCG::text(string text, int length, int x, int y)
{
	if (text.length() > length)
	{
		text.resize(length);
		text[length - 1] = '.';
		text[length - 2] = '.';
	}
	writeSymbol(text, x, y);
}
