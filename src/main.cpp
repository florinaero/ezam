#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"
#include "control.hpp"

using namespace std;

int main (){

	std::cout << "Maze generator: Randomized depth-first search." << endl;
	int size = 40;
	int x=size/2;
	int y=size/2;
	std::cout << "Size: " << size << std::endl; 
	// std::cin >> size;	

	// Construct render window
	Control product{800, 800, size};
	product.setStart(x,y);
	// Start displaying 
	product.run();

	if(size>0){
		Maze maze(size);
		maze.buildDFS(x,y);
		maze.displayMaze();
		// std::cin.get();
	}
	return 0;
}