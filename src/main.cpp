#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"

using namespace std;

int main (){

	cout << "Maze generator: Randomized depth-first search." << endl;

	int x=0;
	int y=0;
	const int size = 3;

	Maze maze(size);
	maze.build(x,y);
	Graphic graphic(size);
	graphic.openFile();

	graphic.closeFile();
	// for (int i=0;i<10;i++){
	// 	maze.generate(x,y);
	// }

	return 0;
}