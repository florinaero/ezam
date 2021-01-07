#ifndef _KRUSKAL_HPP
#define _KRUSKAL_HPP

#include <vector>
#include <utility>
#include "grid.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>



class Kruskal : public sf::Drawable
{
public: 
    // ctor
    Kruskal(int lines, int cols);    
    // Return connected cells(spanning tree)
    std::vector<std::pair<int,int>> getTree() const;   
    // Display maze based on Kruskal alg. and return false when finished
    bool display(sf::RenderWindow& window);
    void setGrid(Grid* grid);

private:    
    int m_lines;
    int m_cols;
    int m_walls;
    Grid *m_grid;
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
    // Kruskal alg
    void unionSet(int cell_1, int cell_2);
    int findSet(int cell);   
    void markCells(int cell_1, int cell_2,sf::Color color, Grid* grid);
    void showWalls();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif