#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"
#include "display.hpp"

using namespace std;

int main (){

	std::cout << "Maze generator: Randomized depth-first search." << endl;

	int x=0;
	int y=0;
	int size = 1;
	std::cout << "Size: "; 
	std::cin >> size;	
	
	// Construct render window
	Display display{800,600};
	// Start displaying 
	display.start();

	if(size>0){
		Maze maze(size);
		maze.build(x,y);
		maze.displayMaze();
		std::cin.get();
	}
	return 0;
}