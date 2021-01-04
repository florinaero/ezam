#include "kruskal.hpp"
#include "logging.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

Kruskal::Kruskal(int lines, int cols):
m_lines(lines),
m_cols(cols),
m_walls((cols-1)*lines + cols*(lines-1)),
m_cells(lines*cols)
{
    init();
    storeWalls();
    randWall();
    showWalls();
}

void Kruskal::init(){
    for(int i=0;i<m_lines*m_cols;i++){
        m_cells.at(i) = i;
    }
}

void Kruskal::storeWalls(){
    /* 0|1|2 // vert walls
    *  - - - // horz walls
    *  3|4|5
    *  - - -
    *  6|7|8 
    */
    // Iterate line by line over matrix of cells
    for(int i=0;i<m_lines;i++){
        for(int j=0;j<m_cols;j++){
            // Number of vertical walls per line is cols - 1 
            if(j<m_cols-1){
                m_vert_walls.push_back(std::make_pair(i*m_lines+j, i*m_lines+j+1)); // Adjacent cells on line    
            } 
            // Number of horizontal walls per column is lines-1
            if(i<m_lines-1){                                                                                            
                m_horz_walls.push_back(std::make_pair(i*m_lines+j, (i+1)*m_lines+j)); // Adjacent cells on columns           
            }    
        }
    }    
}

// Randomly select walls until all cells are visited
void Kruskal::randWall() noexcept{
    int cnt = 0;
    int idx = 0;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::vector<int> unvisited(m_walls);

    // Fill vector with tag for each wall
    for(int i=0;i<m_walls;i++){
        unvisited.at(i) = i;
    }
    
    // Run until all cells are visited
    while(!unvisited.empty()){
        cnt++;
        // Uniform distribution of remained unvisited walls
        std::uniform_int_distribution<std::mt19937::result_type> distrib(0,unvisited.size()-1);
        int select = distrib(rng);
        idx = unvisited.at(select); // Get index of remaining unvisted wall
        // std::cout << select << " " << idx << std::endl; 
        unvisited.erase(unvisited.begin()+select); // Remove visited element 
        // Consider vertical walls first and then horizontal walls and store wall
        if(idx>m_vert_walls.size()-1){
            m_seq_walls.push_back(m_horz_walls.at(idx-m_vert_walls.size()));
        }
        else{
            m_seq_walls.push_back(m_vert_walls.at(idx));
        }
        // Get last pair of cells for an wall
        auto pair_cells = m_seq_walls.back();
        int idx_1 = findSet(pair_cells.first);
        int idx_2 = findSet(pair_cells.second);
        if(idx_1!=idx_2){
            // Store pair of cells where walls are removed(spanning tree for Kruskal alg.)
            m_tree.push_back(pair_cells);
            
            unionSet(idx_1,idx_2);
            std::cout << "idx_1: " << idx_1 << " idx_2: " << idx_2 << std::endl;
            std::cout << "";
        }        
    }
}

// Joint visited cells 
void Kruskal::unionSet(int u, int v){
		m_cells[u] = m_cells[v];
	}

// Find cell's parent, in first case is indicated a cycle 
int Kruskal::findSet(int idx){
    if(idx==m_cells[idx]){
        return idx;
    }
    else{
        return findSet(m_cells[idx]);
    }
} 

std::vector<std::pair<int,int>> Kruskal::getTree() const {
    return m_tree;
}

bool Kruskal::display(sf::RenderWindow& window){
    Grid grid(m_lines, window);
    std::vector<std::pair<int,int>>::const_iterator it;
    sf::Event event;

    for( it=m_tree.begin();it!=m_tree.end();++it){
        // Watch for events
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }
        // markCells(it->first, it->second, sf::Color::Red, grid);
        // // grid.removeWall(it->first, it->second, sf::Color::Red);
        // markWall(it->first, it->second, sf::Color::Blue, grid);
        // window.draw(grid); 
        // window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds{200}); 
          
        markCells(it->first, it->second, sf::Color::Green, grid);
        markWall(it->first, it->second, sf::Color::Green, grid);
        window.draw(grid); 
        window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds{200}); 
    }
    return false;
}

void Kruskal::markCells(int cell_1, int cell_2,sf::Color color, Grid& grid){
    int x = cell_1 % m_lines; // line's index
    int y = cell_1 / m_lines; // column's index
    grid.setColorQuad(x,y,color);
    
    x = cell_2 % m_lines; // line's index
    y = cell_2 / m_lines; // column's index
    grid.setColorQuad(x,y,color);
}

void Kruskal::markWall(int cell_1, int cell_2,sf::Color color, Grid& grid){
    // Find if is horz or vertical wall between cells by checking
    // delta of 1 for horizontal cells(vert wall) and 
    // larger than 1 for vertical cells(horz wall) 
    if(cell_2-cell_1==1){
        grid.setColorWallVert(cell_1,cell_2,color);
    }
    if(cell_2-cell_1>1){
        grid.setColorWallHorz(cell_1,cell_2,color);
    }
}

void Kruskal::setQuad(int idx, int width, int height){    
    int x = idx % m_lines; // line's index
    int y = idx / m_lines; // column's index
    int cnt = y*m_lines*Grid::q_s + x*Grid::q_s; // index based on coordinates
    
    sf::VertexArray cell(sf::Quads, Grid::q_s); 
    cell[0] = sf::Vector2f(x*width, y*height);
    cell[1] = sf::Vector2f(x*width + width, y*height);
    cell[2] = sf::Vector2f(x*width + width, y*height + height);
    cell[3] = sf::Vector2f(x*width, y*height + height);
    
    for(int i=0;i<cell.getVertexCount();i++){
        cell[i].color = sf::Color::Magenta;    
    }
}

// DEBUG content
// Show visited wall and cells
void Kruskal::showWalls(){
    int j = 0; 
    for(auto& i : m_seq_walls){
        if(logging::SHOW){        
            std::cout << "idx = " << j++ << "  ";
            std::cout << "cell1=" << i.first << ", cell2= " << i.second << std::endl;
        }
    }
    j=0;
    std::cout << "spanning tree\n";
    for(auto& i : m_tree){
        if(logging::SHOW){
            std::cout << "idx = " << j++ << "  ";
            std::cout << "cell1: " << i.first << " cell2: " << i.second << std::endl;
        }
    }
}

