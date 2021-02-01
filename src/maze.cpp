#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <utility>
#include <map>
#include <deque>
#include <thread>
#include "graphic.hpp"
#include "maze.hpp"
#include "logging.hpp"

using namespace std;

// Ctor
Maze::Maze(int size, std::shared_ptr<Grid> sp_grid):
m_grid(sp_grid),
m_size(size),
m_graph(size, vector<int>(size, 0)), 
m_graph_out(size), 
m_head_qs(sf::Quads, Grid::q_s),
m_removed_walls(sf::Quads, 2*size*(size-1)*Grid::q_s), // number of individual walls
m_wall_idx(0),
m_old_x(0),
m_old_y(0)
{
	if(sp_grid==nullptr){
		throw std::exception("null dependency!");
	}
	this->buildDFS(0,0);
	for(int i=0;i<Grid::q_s;i++){
        m_head_qs[i].color = sf::Color::Magenta;
    }
};

// dtor
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

void Maze::setGrid(std::shared_ptr<Grid> grid){
	if(grid==nullptr){
		throw std::exception("null dependency!");
	}
    m_grid = grid;
}

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // Draw grid        
    target.draw(m_grid->m_quads, states);
	target.draw(this->m_head_qs, states);                        
    target.draw(m_grid->m_row_walls, states);
    target.draw(m_grid->m_col_walls, states);
	target.draw(this->m_removed_walls, states);
    target.draw(m_grid->m_outline, states);
    // ... or draw with OpenGL directly
}

bool Maze::display(std::shared_ptr<sf::RenderWindow> window) {
	sf::Event event;
	long wait_time = 10;

    for(int i=0;i<m_coord.size();i++){
		// Watch for events
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window->close();
            }
        }  
                          
        auto coord_pair = m_coord.at(i);     
        // std::cout << "x=" << coord_pair.first << " y=" << coord_pair.second << std::endl;
        this->moveHead(coord_pair.first, coord_pair.second);
        
		std::this_thread::sleep_for(std::chrono::milliseconds{wait_time});        
        window->draw(*this);
		window->display();        
    }
    return false;
}

void Maze::moveHead(const int x, const int y){
    // @todo: Check why this color is not working and lines are not dashed 
    sf::Color color_bgr = sf::Color::Green;
	int wid = m_grid->getSqrW();
	int hgt = m_grid->getSqrH();
	int tck = m_grid->getWallThick();
	int otl = m_grid->getOutlineW();
	int gap = tck/2;//tck%2==0 ? tck/2 : tck/2 + 1;
	
	// Mark stepped cells
	m_grid->setColorQuad(x,y,color_bgr);
    // Map coordinates with window's size 
    m_head_qs[0].position = sf::Vector2f(otl+wid*x, 	otl+hgt*y);
    m_head_qs[1].position = sf::Vector2f(otl+wid*x+wid, otl+hgt*y);
    m_head_qs[2].position = sf::Vector2f(otl+wid*x+wid, otl+hgt*y+hgt);
    m_head_qs[3].position = sf::Vector2f(otl+wid*x, 	otl+hgt*y+hgt);                    

	int key = x*1000+y;	
	auto search = m_visited_cells.find(key);
	// Avoid access violation when number of head moves is larger than no. of walls
	if(search==m_visited_cells.end()){
		m_visited_cells.insert({key,1});	
		// North
		if(m_old_y-y==1){
			int left = gap;
			int right = gap;
			// First column: extend wall to left  
			if(x==0){
				left = 0;
			}
			else{
				left = gap;
			}
			// Last column: extend wall to right
			if(x==m_size-1){	
				right = 0;
			}
			else{
				right = gap;
			}       
			// Create quad that will cover grid's horizontal wall
			m_removed_walls[m_wall_idx].position   = sf::Vector2f(otl+x*wid+left,  otl+(y+1)*hgt-gap);    
			m_removed_walls[m_wall_idx+1].position = sf::Vector2f(otl+x*wid+wid-right, otl+(y+1)*hgt-gap);    
			m_removed_walls[m_wall_idx+2].position = sf::Vector2f(otl+x*wid+wid-right, otl+(y+1)*hgt+gap);    
			m_removed_walls[m_wall_idx+3].position = sf::Vector2f(otl+x*wid+left,  otl+(y+1)*hgt+gap);  
			for(int i=0;i<Grid::q_s;i++){   
				m_removed_walls[m_wall_idx+i].color = color_bgr;
			}
		}
		// Est
		if(m_old_x-x==-1){
			int up = gap;		
			int down = gap;		
			// First line
			if(y==0){
				up = 0; 
			}
			else{
				up = gap;
			}
			// Last line
			if(y==m_size-1){
				down = 0; 
			}
			else{
				down = gap;
			}
			m_removed_walls[m_wall_idx].position   = sf::Vector2f(otl+x*wid-gap, otl+y*hgt+up);    
			m_removed_walls[m_wall_idx+1].position = sf::Vector2f(otl+x*wid+gap, otl+y*hgt+up);    
			m_removed_walls[m_wall_idx+2].position = sf::Vector2f(otl+x*wid+gap, otl+y*hgt+hgt-down);    
			m_removed_walls[m_wall_idx+3].position = sf::Vector2f(otl+x*wid-gap, otl+y*hgt+hgt-down);
			for(int i=0;i<Grid::q_s;i++){     
				m_removed_walls[m_wall_idx+i].color = color_bgr;
			}
		}
		// South
		if(m_old_y-y==-1){
			int left = gap;
			int right = gap;
			// First column: extend wall to left  
			if(x==0){
				left = 0;
			}
			else{
				left = gap;
			}
			// Last column: extend wall to right
			if(x==m_size-1){	
				right = 0;
			}
			else{
				right = gap;
			} 
			m_removed_walls[m_wall_idx].position   = sf::Vector2f(otl+x*wid+left, 	 otl+(m_old_y+1)*hgt-gap);    
			m_removed_walls[m_wall_idx+1].position = sf::Vector2f(otl+x*wid+wid-right, otl+(m_old_y+1)*hgt-gap);    
			m_removed_walls[m_wall_idx+2].position = sf::Vector2f(otl+x*wid+wid-right, otl+(m_old_y+1)*hgt+gap);    
			m_removed_walls[m_wall_idx+3].position = sf::Vector2f(otl+x*wid+left, 	 otl+(m_old_y+1)*hgt+gap);
			for(int i=0;i<Grid::q_s;i++){     
				m_removed_walls[m_wall_idx+i].color = color_bgr;
			}
		}
		// West
		if(m_old_x-x==1){
			int up = gap;		
			int down = gap;		
			// First line
			if(y==0){
				up = 0; 
			}
			else{
				up = gap;
			}
			// Last line
			if(y==m_size-1){
				down = 0; 
			}
			else{
				down = gap;
			}
			m_removed_walls[m_wall_idx].position   = sf::Vector2f(otl+m_old_x*wid-gap, otl+y*hgt+up);    
			m_removed_walls[m_wall_idx+1].position = sf::Vector2f(otl+m_old_x*wid+gap, otl+y*hgt+up);    
			m_removed_walls[m_wall_idx+2].position = sf::Vector2f(otl+m_old_x*wid+gap, otl+y*hgt+hgt-down);    
			m_removed_walls[m_wall_idx+3].position = sf::Vector2f(otl+m_old_x*wid-gap, otl+y*hgt+hgt-down);
			for(int i=0;i<Grid::q_s;i++){     
				m_removed_walls[m_wall_idx+i].color = color_bgr;
			}
		}		
		m_wall_idx += 4;
	}
	m_old_x = x;
	m_old_y = y;
}