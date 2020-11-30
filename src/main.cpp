#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"

using namespace std;

int main (){

	std::cout << "Maze generator: Randomized depth-first search." << endl;

	int x=0;
	int y=0;
	int size = 0;
	
	std::cout << "Size: "; 
	std::cin >> size;
	
	Maze maze(size);
	maze.build(x,y);
	maze.displayMaze();

	std::cin.get();
	return 0;
}