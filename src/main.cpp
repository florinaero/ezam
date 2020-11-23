#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"

using namespace std;

int main (){

	cout << "Maze generator: Randomized depth-first search." << endl;

	int x=0;
	int y=0;
	int size = 0;
	cout << "Size: "; 
	cin >> size;
	
	Maze maze(size);
	maze.build(x,y);
	maze.displayMaze();
	// for (int i=0;i<10;i++){
	// 	maze.generate(x,y);
	// }

	return 0;
}