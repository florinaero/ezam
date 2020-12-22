#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <utility>
#include <map>
#include <deque>
#include "graphic.hpp"
#include "maze.hpp"
#include "logging.hpp"

using namespace std;

// Ctor
Maze::Maze(int size):
m_size(size),
m_graph(size, vector<int>(size, 0)), 
m_graph_out(size)
{
};

// Dctor
Maze::~Maze(){
	if(logging::SHOW){
		cout << "Wall: \n";
		cout << "Vertical:\n";

		for(int y=m_size-1;y>=0;y--){
			for(int x=0;x<m_size-1;x++){
				cout << m_graph_out.m_vert_walls[x][y] << " ";	
			}
			cout << endl;
		}

		cout << "Horizontal:\n";
		for(int y=m_size-2;y>=0;y--){
			for(int x=0;x<m_size;x++){
				cout << m_graph_out.m_horz_walls[x][y] << " ";	
			}
			cout << endl;
		}
	}
}

// Build maze based on deppth first search using recursion
void Maze::buildDFS(int x, int y){
	bool go_back = false;
	int old_x = 0;
	int old_y = 0;

	// Check if node is visited
	if(m_graph[x][y]!=1){
		// Mark as visited
		m_graph[x][y] = 1;
		// Store visited nodes
		m_que_visit.emplace_back(x,y);
	}
	// Store old coordinates for graphic usage
	old_x = x;
	old_y = y;
	// Store coordinates of visited cells before generating others
	m_coord.push_back(std::make_pair(x,y));
	// Return new coordinates by reference
	go_back = generate(x,y);
	// Send old and new coord for graphic display
	m_graph_out.getCoord(old_x, old_y, x, y);

	// Pop last visited node
	if(go_back){
		// Last element is the one that has zero unvisted neighbors
		m_que_visit.pop_back();
		if(m_que_visit.size()>0){
			x = m_que_visit.back().first;
			y = m_que_visit.back().second;
			// cout << "GO_BACK:"  <<" x=" << x << " y=" << y << endl;		
		}
	}
	else{
		m_graph_out.m_exit.first = x;
		m_graph_out.m_exit.second = y;
	}
	// Keep recursion
	if(m_que_visit.size()>0){
		buildDFS(x,y);
	}
}

// Generate coord of next block
bool Maze::generate(int &x, int&y){
	// cout << "intial:"  <<" x=" << x << " y=" << y << endl;
	random_device rd;
	mt19937 rng(rd());		
	static int counter = 1;
	// Define available options based on current position and if node is already visited
	vector<pair<int,int>> neighbors;
	if(x>0){
		// Add west
		if(m_graph[x-1][y]!=1){
			neighbors.emplace_back(x-1,y);
		}
	}
	if(x<m_size-1){
		// Add est
		if(m_graph[x+1][y]!=1){
			neighbors.emplace_back(x+1,y);
		}
	}
	if(y>0){
		// Add north
		if(m_graph[x][y-1]!=1){
			neighbors.emplace_back(x,y-1);
		}			
	}
	if(y<m_size-1){
		// Add south		
		if(m_graph[x][y+1]!=1){
			neighbors.emplace_back(x,y+1);
		}	
	}

	if(neighbors.size()>0){
		// Randomly find coordinates of a node 
		uniform_int_distribution<mt19937::result_type> direction(0,neighbors.size()-1);
		auto coord = neighbors.at(direction(rng));

		x = coord.first;
		y = coord.second;
		if(logging::SHOW){
			cout << counter++  <<" x=" << x << " y=" << y << endl;
		}
		return false;
	}
	else{
		// Is true that it should go back on tree to find a node with unvisited neighbors
		return true;
	}
}

// Display Maze
void Maze::displayMaze(){
	m_graph_out.buildDisplay();
	m_graph_out.display();
}

void Maze::getCoord(std::vector<std::pair<int,int>>& coordinates) const{
	coordinates = m_coord;
}
