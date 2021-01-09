#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>
#include <deque>
#include <memory>
#include <unordered_map>
#include "graphic.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "grid.hpp"

class Maze : sf::Drawable 
{
private:
	int m_size;
	int m_wall_idx;
	int m_old_x;
	int m_old_y;
	std::vector<std::vector<int>> m_graph;
	std::deque<std::pair<int,int>> m_que_visit;
	std::vector<std::pair<int,int>> m_coord;
	std::unordered_map<int,int> m_visited_cells;
	Graphic m_graph_out;
	std::shared_ptr<Grid> m_grid;
	sf::VertexArray m_head_qs;
	sf::VertexArray m_removed_walls;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void moveHead(const int x, const int y);

public:
	// Ctor
	Maze(int size, std::shared_ptr<Grid> sp_grid);
	// dtor
	~Maze();
	// Build maze based on depth first search using recursion
	void buildDFS(int x, int y);
	// Generate coord of next block
	bool generate(int &x, int&y);
	// Display maze 
	void displayMaze();
	// Get visited coordinates
	void getCoord(std::vector<std::pair<int,int>>& coordinates) const;
	// Display content 
	bool display(sf::RenderWindow& window);
	void setGrid(std::shared_ptr<Grid> grid);
};

#endif 