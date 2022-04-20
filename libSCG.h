#ifndef LIBSCG_H
#define LIBSCG_H
#define clearScreen cout << "\033[H\033[J"
#define saveCursor cout << "\033[s"
#define loadCursor cout << "\033[u"

#include <iostream>
#include <cmath>

using namespace std;

class libSCG
{
	public:
		// VARIABLES //

		// FUNCTIONS //
		libSCG(int sX, int sY, bool debug, bool btflWindow);
		~libSCG();
		void execute(string command);
	protected:
		// VARIABLES //
		bool btflWindow = true;
		bool debug = true;
	private:
		// VARIABLES // 
		int sizeX;
		int sizeY;
		int FGcolor[3] = {255, 255, 255};
		int BGcolor[3] = {0, 0, 0};
		// FUNCTIONS //
		void drawWindow();
		void printLogMsg(string msg, int err);
		void writeSymbol(string symbol, int x, int y);
		void rectangle(string symbol, bool fill, int x1, int y1, int x2, int y2);
		void setColor(int Br, int Bg, int Bb, int Fr, int Fg, int Fb);	
		void circle(string symbol, int x, int y, int radius, bool fill);
		void line(string symbol, int x1, int y1, int x2, int y2);
};

#endif // LIBSCG_H
