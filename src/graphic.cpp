#include "graphic.hpp"
#include <iostream>
#include <string>

Graphic::Graphic(int size):
m_size(size), 
m_out_file("log/out1.txt", std::ios_base::out), 
m_vert_walls(size-1, std::vector<int>(size, 1)), // vertical inside walls, col=size-1, lines=size  
m_horz_walls(size, std::vector<int>(size-1, 1)), // horizontal inside walls, col=size, lines=size-1
m_final_horz(size,std::vector<int>(size+1,0)),	 // Matrix of horizontal walls
m_final_vert(size+1,std::vector<int>(size,0))   // Matrix of vertical walls
{};

void Graphic::openFile(){
	if(m_out_file.fail()){
		std::cout << "ERROR: File didn't open!" << std::endl;
		m_out_file.close();
	}
}

void Graphic::closeFile(){
	m_out_file.close();
}

void Graphic::getCoord(int old_x, int old_y, int new_x, int new_y){
	// Consider origin point at left bottom corner which will create 
	//	positive values for movement to maximum
	int delta_x = new_x - old_x;
	int delta_y = new_y - old_y;
	int del_x = 0;
	int del_y = 0;

	// Case of horizontal movement where vertical wall is removed
	// Delta y is zero for this case
	if(delta_x!=0){
		// Always select minimum value which corresponds to matrix's index
		del_x = std::min(old_x, new_x);
		// Same with old_y
		del_y = new_y; 
		// This wall will be removed
		m_vert_walls[del_x][del_y] = 0;
	}

	if(delta_y!=0){
		// Always select minimum value which corresponds to matrix's index
		del_y = std::min(old_y, new_y);
		del_x = new_x;
		m_horz_walls[del_x][del_y] = 0;
	}
}

// Build matrices for horizontal and vertical walls
void Graphic::buildDisplay(){
	// Horizontal
	for(int x=0;x<m_size;x++){
		for(int y=0;y<m_size+1;y++){
			if(y==0 || y==m_size){
				m_final_horz[x][y] = 1;
			}
			else{
				m_final_horz[x][y] += m_horz_walls[x][y-1]; 
			}
		}
	}

	// Vertical 
	for(int x=0;x<m_size+1;x++){
		for(int y=0;y<m_size;y++){
			if(x==0 || x==m_size){
				m_final_vert[x][y] = 1;
			}
			else{
				m_final_vert[x][y] += m_vert_walls[x-1][y]; 
			}
		}
	}	
}

void Graphic::display(){
	std::string content;
	char elem = ' ';
	int y_v = 0;
	const char SYMBOL = '#';
	// Go from left-top to right-bottom 
	for(int y=m_size;y>=0;y--){
		// Complete vertical wall on left
		content.push_back(SYMBOL);
		for(int x=0;x<m_size;x++){
			// Mark entrance
			if(x==0 && y==0){
				content.push_back('^');
			}
			if(m_final_horz[x][y]==1){
				elem = SYMBOL;
				content.push_back(elem);
			}
			else{
				elem = ' ';
				content.push_back(elem);
			}
			content.push_back(elem);
			// Remove one elem to compensate for entrance symbol
			if(x==0 && y==0){
				content.pop_back();
			}
		}
		// Complete vertical wall on right only for horzintal internal walls
		if(y>0 && y<m_size){
			content.pop_back();
			content.push_back('#');
		}
		content.push_back('\n');

		// Internal vertical walls height is less by 1 than size
		if(y>0){
			y_v = y - 1;
			// Vertical walls with space between them
			for(int x=0;x<m_size+1;x++){
				if(m_final_vert[x][y_v]==1){
					elem = SYMBOL;
				}
				else{
					elem = ' ';
				}
				content.push_back(elem);
				content.push_back(' ');
			}
			content.push_back('\n');
		}
	}

	std::cout << "##Maze##\n";
	std::cout << m_exit.first << " " << m_exit.second << std::endl;
	std::cout << content << std::endl;
}
