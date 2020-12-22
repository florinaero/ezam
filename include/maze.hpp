#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>
#include <deque>
#include "graphic.hpp"

class Maze{
private:
	int m_size;
	std::vector<std::vector<int>> m_graph;
	std::deque<std::pair<int,int>> m_que_visit;
	std::vector<std::pair<int,int>> m_coord;
	Graphic m_graph_out;
	
public:
	// Ctor
	Maze(int size);
	// Dctor
	~Maze();
	// Build maze based on depth first search using recursion
	void buildDFS(int x, int y);
	// Generate coord of next block
	bool generate(int &x, int&y);
	// Display maze 
	void displayMaze();
	// Get visited coordinates
	void getCoord(std::vector<std::pair<int,int>>& coordinates) const;
};

#endif 