#include <iostream>
#include "graphic.hpp"
#include "maze.hpp"
#include "control.hpp"
#include "kruskal.hpp"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

int main (){
	int size = 20;

	// Construct render window
	Control product{1000, 1000, size};
	// Start displaying 
	product.run();

	// std::cin.get();
	
	return 0;
}