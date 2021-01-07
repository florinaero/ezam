#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"
#include "control.hpp"
#include "kruskal.hpp"

using namespace std;

int main (){

	std::cout << "Maze generator: Randomized depth-first search." << endl;
	int size = 10;
	int x=size/2;
	int y=size/2;
	
	// Kruskal krusk(size,size);
	// auto krusk_tree = krusk.getTree();

	// Construct render window
	Control product{1000, 1000, size};
	// // Start displaying 
	product.run();

	// if(size>0){
	// 	Maze maze(size);
	// 	maze.buildDFS(x,y);
	// 	maze.displayMaze();
	// 	// std::cin.get();
	// }
	
	return 0;
}