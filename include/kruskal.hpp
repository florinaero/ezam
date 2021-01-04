#ifndef _KRUSKAL_HPP
#define _KRUSKAL_HPP

#include "grid.hpp"
#include <vector>
#include <utility>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>



class Kruskal{
public: 
    // ctor
    Kruskal(int lines, int cols);    
    // Return connected cells(spanning tree)
    std::vector<std::pair<int,int>> getTree() const;   
    // Display maze based on Kruskal alg. and return false when finished
    bool display(sf::RenderWindow& window);

private:    
    int m_lines;
    int m_cols;
    int m_walls;
    std::vector<std::pair<int,int>>m_vert_walls;
    std::vector<std::pair<int,int>>m_horz_walls;
    std::vector<std::pair<int,int>>m_seq_walls; // Sequence of randomly selected walls
    std::vector<std::pair<int,int>>m_tree; // Spanning tree 
    std::vector<int> m_cells;

    void init();
    // Create vertical and horizontal walls between cells
    void storeWalls();
    // Randomly select one wall from collection of vertical and horiz.
    void randWall() noexcept;
    void markCells(int cell_1, int cell_2,sf::Color color, Grid& grid);
    void markWall(int cell_1, int cell_2,sf::Color color, Grid& grid);
    void setQuad(int idx, int width, int height);
    void showWalls();
    void unionSet(int cell_1, int cell_2);
    int findSet(int cell);   
};


#endif