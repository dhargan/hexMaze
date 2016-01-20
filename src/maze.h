#ifndef MAZE_H
#define MAZE_H

class Cell;
class Maze;

class Cell{
	private:
		int id;
		Cell* neighbors[6];
		bool walls[6];
		bool visited;
		int NVN; //non visited neighbor count
	public:
		Cell(int id);
		bool isVisited();
		bool isAllNeighborsVisited();
		void setNeighbor(int i, int nb, Maze* maze);
		Cell* getNeighbor(int i);
		void incNVN(); 
		void decNVN();
		int getNVN(); 
		bool isWallExist(int i);
		void visit();
		int getID();
		bool isValidNB(int i,int nb,int w);
		void removeWall(int dir);
};

class Maze{
	private:
		Cell** cells;
		int height;
		int width;
		int size;
	public:
		Maze(int h, int w);
		void draw();
		void generateMaze();
		int getSize();
		int getWidth();
		Cell* getCell(int i);
		void printCells();
		void removeWall(int from, int dir);		
};

#endif

