#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <fstream>
#include <string>
#include <vector>

class Graphic
{
private:
	std::string m_dataFile; // Used to store content of file
public:
	int m_size;
	std::fstream m_out_file;
	std::vector<std::vector<int>> m_vert_walls;
	std::vector<std::vector<int>> m_horz_walls;
	std::vector<std::vector<int>> m_final_horz;
	std::vector<std::vector<int>> m_final_vert;
	std::pair<int, int> m_exit;
	// Ctor
	Graphic(int size);

	void openFile();
	void closeFile();

	// Get old and new coordinates to determine movement direction
	void getCoord(int old_x, int old_y, int new_x, int new_y);

	// Create graphic matrix from top to bottom by sequencing horizontal lines followed by vertical
	void buildDisplay();

	// Display final content from top to bottom
	// Horizontal followed by vertical walls
	void display();
};

#endif