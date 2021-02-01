#include "kruskal.hpp"
#include "logging.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

// Ctor
Kruskal::Kruskal(int lines, int cols, std::shared_ptr<Grid> sp_grid)
    : m_grid(sp_grid),
      m_lines(lines),
      m_cols(cols),
      m_walls((cols-1)*lines + cols*(lines-1)),
      m_cells(lines*cols) 
{
    if(sp_grid==nullptr){
        throw std::exception("null dependency!");
    }
    init();
    storeWalls();
    randWall();
    showWalls();
}

// dtor
Kruskal::~Kruskal(){
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
            // std::cout << "idx_1: " << idx_1 << " idx_2: " << idx_2 << std::endl;
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

bool Kruskal::display(std::shared_ptr<sf::RenderWindow> window) {
    std::vector<std::pair<int,int>>::const_iterator it;
    sf::Event event;
    long wait_time = 10;
    for(it=m_tree.begin();it!=m_tree.end();++it){
        // Watch for events
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window->close();
            }
        }
        m_grid->removeWall(it->first, it->second, sf::Color::Green);
        markCells(it->first, it->second, sf::Color::Red, m_grid);
        window->draw(*this); 
        
        window->display();
        std::this_thread::sleep_for(std::chrono::milliseconds{wait_time}); 
          
        markCells(it->first, it->second, sf::Color::Green, m_grid);
        window->draw(*this); 
        window->display();
        std::this_thread::sleep_for(std::chrono::milliseconds{wait_time}); 
    }
    return false;
}

void Kruskal::markCells(int cell_1, int cell_2,sf::Color color, std::shared_ptr<Grid> grid){
    int x = cell_1 % m_lines; // line's index
    int y = cell_1 / m_lines; // column's index
    grid->setColorQuad(x,y,color);
    
    x = cell_2 % m_lines; // line's index
    y = cell_2 / m_lines; // column's index
    grid->setColorQuad(x,y,color);
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

    for(auto& i : m_tree){
        if(logging::SHOW){
            std::cout << "idx = " << j++ << "  ";
            std::cout << "cell1: " << i.first << " cell2: " << i.second << std::endl;
        }
    }
}

void Kruskal::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // Draw grid        
    target.draw(m_grid->m_quads, states);                        
    target.draw(m_grid->m_row_walls, states);
    target.draw(m_grid->m_col_walls, states);
    target.draw(m_grid->m_removed_walls, states);
    target.draw(m_grid->m_outline, states);
    // ... or draw with OpenGL directly
}

void Kruskal::setGrid(std::shared_ptr<Grid> grid){
    if(grid==nullptr){
        throw std::exception("null dependency!");
    }
    m_grid = grid;
}

