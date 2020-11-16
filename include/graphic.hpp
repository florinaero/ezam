#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <fstream>
#include <string>
#include <vector>

class Graphic{
public:
	int m_size;
	std::fstream m_out_file;
	std::vector<std::vector<int>> m_vert_walls;
	std::vector<std::vector<int>> m_horz_walls;
	// Ctor
	Graphic(int size);

	void openFile();
	void closeFile();

	// Get old and new coordinates to determine movement direction
	void getCoord(int old_x, int old_y, int new_x, int new_y);
private:
	std::string m_dataFile;	// Used to store content of file

};

#endif