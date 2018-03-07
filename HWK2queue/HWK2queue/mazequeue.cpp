#include <queue>
#include <string>
#include<iostream>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	Coord curr(sr, sc);
	maze[curr.r()][curr.c()] = '#';
	Coord end(er, ec);
	coordQueue.push(curr);

	while (!coordQueue.empty())
	{
		curr = coordQueue.front();
		
		coordQueue.pop();
		if (curr.c() == end.c() && curr.r() == end.r())
			return true;
		if (maze[curr.r()][curr.c() + 1] != 'X' && maze[curr.r()][curr.c() + 1] != '#') //EAST
		{
			maze[curr.r()][curr.c() + 1] = '#';
			Coord a(curr.r(), curr.c() + 1);
			coordQueue.push(a);
		}
		if (maze[curr.r()][curr.c() - 1] != 'X' && maze[curr.r()][curr.c() - 1] != '#') //WEST
		{
			maze[curr.r()][curr.c() - 1] = '#';
			Coord a(curr.r(), curr.c() - 1);
			coordQueue.push(a);
		}
		if (maze[curr.r() + 1][curr.c()] != 'X' && maze[curr.r() + 1][curr.c()] != '#') //SOUTH
		{
			maze[curr.r() + 1][curr.c()] = '#';
			Coord a(curr.r() + 1, curr.c());
			coordQueue.push(a);
		}
		if (maze[curr.r() - 1][curr.c()] != 'X' && maze[curr.r() - 1][curr.c()] != '#') //NORTH
		{
			maze[curr.r() - 1][curr.c()] = '#';
			Coord a(curr.r() - 1, curr.c());
			coordQueue.push(a);
		}
	}

	return false;


}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}