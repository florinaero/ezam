#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>
#include <deque>
#include "graphic.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "grid.hpp"

class Maze : sf::Drawable 
{
private:
	int m_size;
	std::vector<std::vector<int>> m_graph;
	std::deque<std::pair<int,int>> m_que_visit;
	std::vector<std::pair<int,int>> m_coord;
	Graphic m_graph_out;
	Grid *m_grid;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
public:
	// Ctor
	Maze(int size);
	// Dctor
	~Maze();
	// Build maze based on depth first search using recursion
	void buildDFS(int x, int y);
	// Build maze using randomized Kruskal's algorithm
	void buildKruskal(int x, int y);
	// Generate coord of next block
	bool generate(int &x, int&y);
	// Display maze 
	void displayMaze();
	// Get visited coordinates
	void getCoord(std::vector<std::pair<int,int>>& coordinates) const;
	// Display content 
	bool display(sf::RenderWindow& window) const;
	void setGrid(Grid* grid);
};

#endif 