#include "graphic.hpp"
#include <iostream>

Graphic::Graphic(int size):
m_size(size), 
m_out_file("log/out1.txt", std::ios_base::out), 
m_vert_walls(size, std::vector<int>(size-1, 1)), // vertical inside walls 
m_horz_walls(size-1, std::vector<int>(size, 1))	 // horizontal inside walls
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

	// Case of horizontal movement and vertical wall is removed
	// Delta y is zero for this case
	if(delta_x!=0){
		// Always select minimum value which corresponds to matrix's index
		del_x = std::min(old_x, new_x);
		// if(delta_x>0){
		// 	// For movement from left to right the corespondent wall has smallest index in matrix
		// 	del_x = old_x;
		// }
		// else{
		// 	// For movement from right to left the correspondent wall has 
		// 	del_x = new_x;
		// }
		if(new_y>=m_size-1){
			std::cout << "new_y: " << new_y << std::endl;
			new_y--;
			std::cout << "new_y: " << new_y << std::endl;
		}
		del_y = new_y; // same with old_y
		// This wall will be removed
		m_vert_walls[del_x][del_y] = 0;
	}

	if(delta_y!=0){
		// Always select minimum value which corresponds to matrix's index
		del_y = std::min(old_y, new_y);
		// if(delta_y>0){
		// 	del_y = old_y;
		// }
		// else{
		// 	del_y = new_y;
		// }
		if(new_x>=m_size-1){
			new_x--;
		}
		del_x = new_x;
		m_horz_walls[del_x][del_y] = 0;
	}
}