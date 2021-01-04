#include "grid.hpp"
#include <iostream>
#include <random>
#include <logging.hpp>

Grid::Grid(int size, const sf::RenderWindow& window):
m_size(size),
m_window(window),
m_outline(sf::Quads, 4*q_s),
m_quads(sf::Quads, size*size*q_s),
m_no_walls(size*(size-1)),  // Total walls on vertical/horizontal
m_v_walls(sf::Quads, 0),
m_h_walls(sf::Quads, 0),    
m_head_q(sf::Quads, q_s), 
m_seen_q(size*size*q_s),
m_old_x(0), 
m_old_y(0)
{
    setSize(size);
    createOutline();
    createGrid();
    createWalls();
};

Grid::~Grid(){
};

// Create window's outline from 4 vertices
void Grid::createOutline(){
    int win_w = m_window.getSize().x;
    int win_h = m_window.getSize().y;
    // Top line
    m_outline[0] = sf::Vector2f(0,0); //top-left
    m_outline[1] = sf::Vector2f(win_w,0); // top-right
    m_outline[2] = sf::Vector2f(win_w,m_outline_w); // bottom-right
    m_outline[3] = sf::Vector2f(0,m_outline_w); // bottom-left
    // Right line
    m_outline[4] = sf::Vector2f(win_w-m_outline_w,0);
    m_outline[5] = sf::Vector2f(win_w,0);
    m_outline[6] = sf::Vector2f(win_w,win_h);
    m_outline[7] = sf::Vector2f(win_w-m_outline_w,win_h);
    // Bottom line
    m_outline[8]  = sf::Vector2f(0,win_h-m_outline_w);
    m_outline[9]  = sf::Vector2f(win_w,win_h-m_outline_w);
    m_outline[10] = sf::Vector2f(win_w,win_h);
    m_outline[11] = sf::Vector2f(0,win_h);
    // Left line
    m_outline[12] = sf::Vector2f(0,0);
    m_outline[13] = sf::Vector2f(m_outline_w,0);
    m_outline[14] = sf::Vector2f(m_outline_w,win_h);
    m_outline[15] = sf::Vector2f(0,win_h);

    for(int i=0;i<m_outline.getVertexCount();i++){
        m_outline[i].color = sf::Color::Black;
    }    
}

void Grid::createGrid(){
    int cnt = 0;
    for(int i=0;i<m_size*m_size;i++){    
        int x = i%m_size;
        int y = i/m_size;
        m_quads[cnt].position   = sf::Vector2f(m_sqr_w*x+m_outline_w,         m_sqr_h*y+m_outline_w);         // top-left
        m_quads[cnt+1].position = sf::Vector2f(m_sqr_w*x+m_outline_w+m_sqr_w, m_sqr_h*y+m_outline_w);         // top-right
        m_quads[cnt+2].position = sf::Vector2f(m_sqr_w*x+m_outline_w+m_sqr_w, m_sqr_h*y+m_outline_w+m_sqr_h); // bottom-right
        m_quads[cnt+3].position = sf::Vector2f(m_sqr_w*x+m_outline_w,         m_sqr_h*y+m_outline_w+m_sqr_h); // bottom-left
        m_quads[cnt].color   = sf::Color::Yellow;
        m_quads[cnt+1].color = sf::Color::Yellow;
        m_quads[cnt+2].color = sf::Color::Black;
        m_quads[cnt+3].color = sf::Color::Yellow;
        cnt = cnt + 4;        
    }
}

void Grid::createWalls(){
    sf::Color color_wall = sf::Color::Black;    
    int cnt = 0;

    // Loop line by line and create vertical walls
    for(int i=0;i<m_no_walls;i++){
        // Window's coordinates, x changes on each coloumn, y changes on each line
        int x = i%(m_size-1); // n-1 columns of vertical walls  
        int y = i/(m_size-1); //      
        int x_coord = m_sqr_w*(x+1) + m_outline_w - m_thick/2; // Start from second column
        int y_coord = m_sqr_h*y + m_outline_w;

        // Vertical walls 
        m_v_walls[cnt].position   = sf::Vector2f(x_coord,         y_coord);                 // top-left
        m_v_walls[cnt+1].position = sf::Vector2f(x_coord+m_thick, y_coord);                 // top-right
        m_v_walls[cnt+2].position = sf::Vector2f(x_coord+m_thick, y_coord+m_sqr_h+m_thick); // bottom-right
        m_v_walls[cnt+3].position = sf::Vector2f(x_coord,         y_coord+m_sqr_h+m_thick); // bottom-left
        m_v_walls[cnt].color   = color_wall;
        m_v_walls[cnt+1].color = color_wall;
        m_v_walls[cnt+2].color = color_wall;
        m_v_walls[cnt+3].color = color_wall;
        cnt = cnt + 4;
    }
    cnt = 0;
    // Horizontal walls
    for(int i=0;i<m_no_walls;i++){
        int x = i%m_size;
        int y = i/m_size;
        int x_coord = m_sqr_w*x + m_outline_w;// - m_thick/2;
        int y_coord = m_sqr_h*(y+1) + m_outline_w; // Start from second line
        m_h_walls[cnt].position   = sf::Vector2f(x_coord,                 y_coord);         // top-left
        m_h_walls[cnt+1].position = sf::Vector2f(x_coord+m_sqr_w+m_thick, y_coord);         // top-right
        m_h_walls[cnt+2].position = sf::Vector2f(x_coord+m_sqr_w+m_thick, y_coord+m_thick); // bottom-right
        m_h_walls[cnt+3].position = sf::Vector2f(x_coord,                 y_coord+m_thick); // bottom left
        m_h_walls[cnt].color   = color_wall;
        m_h_walls[cnt+1].color = color_wall;
        m_h_walls[cnt+2].color = color_wall;
        m_h_walls[cnt+3].color = color_wall;
        cnt = cnt + 4;
    }
}

void Grid::setColorWallVert(const int cell_1, const int cell_2, const sf::Color& color_code){
    // Index based on cell index
    int cnt = cell_1*q_s - (cell_2/m_size)*q_s; // For each line last column is not counted so should be subtracted
    
    for(int i=0;i<q_s;i++){     
        m_v_walls[cnt+i].color = color_code;
    }
}

void Grid::setColorWallHorz(const int  cell_1, const int cell_2, const sf::Color& color_code){
    int cnt = cell_1*q_s; // index based on coordinates
    
    for(int i=0;i<q_s;i++){     
        m_h_walls[cnt+i].color = color_code;
    }
}

void Grid::removeWall(const int cell_1, const int cell_2, sf::Color color){
    // int cnt = ((cell_1%m_size)+(cell_1/m_size)*m_size) * q_s; // index based on coordinates
    int cnt = cell_1 * q_s;
    int x = m_outline_w + (cell_1%m_size)*m_sqr_w + (cell_1%m_size)*m_thick; // number of squares and walls per line
    int y = m_outline_w + (cell_1%m_size)*m_sqr_h + (cell_1%m_size)*m_thick; // number of squares and walls per column
    // Adjacent horizontal cells
    if(std::abs(cell_1-cell_2)==1){
        m_quads[cnt] = sf::Vector2f(x,y);                             // top-left
        m_quads[cnt+1] = sf::Vector2f(x+2*m_sqr_w+m_thick,y);         // top-right
        m_quads[cnt+2] = sf::Vector2f(x+2*m_sqr_w+m_thick,y+m_sqr_h); // bottom-right
        m_quads[cnt+3] = sf::Vector2f(x,y+m_sqr_h);                   // bottom-left
    }
    // Possibile adjacent vertical cells
    if(std::abs(cell_1-cell_2)>1){
        m_quads[cnt] = sf::Vector2f(x,y);                             // top-left
        m_quads[cnt+1] = sf::Vector2f(x+m_sqr_w,y);                   // top-right
        m_quads[cnt+2] = sf::Vector2f(x+m_sqr_w,y+2*m_sqr_h+m_thick); // bottom-right
        m_quads[cnt+3] = sf::Vector2f(x,y+2*m_sqr_h+m_thick);         // bottom-left
    }

    for(int i=0;i<q_s;i++){     
        m_quads[cnt+i].color = color;
    }
}

void Grid::setSize(const int size){
    m_h_walls.resize(m_no_walls*q_s);
    m_v_walls.resize(m_no_walls*q_s);
    m_outline_w = 2; // pixels 
    int win_size_x = m_window .getSize().x - 2*m_outline_w;
    int win_size_y = m_window .getSize().y - 2*m_outline_w;
    m_sqr_w = static_cast<int>(win_size_x / size);
    m_sqr_h = static_cast<int>(win_size_y / size);
    // Thick of interior walls
    m_thick = static_cast<int>(0.2 * m_sqr_w);
    // Modify outline in order to fill resulted gap from cast to int
    int delta_win = win_size_x - m_sqr_w*m_size; 
    if(delta_win!=0){
      m_outline_w += (delta_win+0.5)/2;   
    }
    if(logging::SHOW){
        std::cout << "WINDOW DIMENSSIONS: \n";
        std::cout << "m_sqr_w = " << m_sqr_w << " m_sqr_h= " << m_sqr_h << std::endl;
        std::cout << "win_size_x = " << win_size_x << " win_size_y= " << win_size_y << std::endl;
        std::cout << "m_window.getSize().x = " << m_window.getSize().x 
                    << " m_outline_w= " << m_outline_w << std::endl;
    }
}

void Grid::moveHead(const int x, const int y){
    // @todo: Check why this color is not working and lines are not dashed 
    sf::Color color_bgr = sf::Color(1,1,1,1);//sf::Color::Green;
    // Map coordinates with window's size 
    m_head_q[0].position = sf::Vector2f(m_sqr_w*x+m_thick, m_sqr_h*y+m_thick);
    m_head_q[1].position = sf::Vector2f(m_sqr_w*x+m_sqr_w, m_sqr_h*y+m_thick);
    m_head_q[2].position = sf::Vector2f(m_sqr_w*x+m_sqr_w, m_sqr_h*y+m_sqr_h);
    m_head_q[3].position = sf::Vector2f(m_sqr_w*x+m_thick, m_sqr_h*y+m_sqr_h);            
    
    int cnt = y*m_size*q_s + x*q_s; // index based on coordinates
    for(int i=0;i<q_s;i++){
        m_head_q[i].color = sf::Color::Magenta;
        // Color after cell is touched
        m_quads[cnt+i].color = sf::Color::Yellow;
    }

    cnt = m_old_y*m_size*q_s + x*q_s;
    // North
    if(m_old_y-y==1){
        cnt = m_old_y*m_size*q_s + x*q_s;
        for(int i=0;i<q_s;i++){     
            m_h_walls[cnt+i].color = color_bgr;
        }
    }
    // Est
    if(m_old_x-x==-1){
        cnt = y*m_size*q_s + x*q_s;
        for(int i=0;i<q_s;i++){     
            m_v_walls[cnt+i].color = color_bgr;
        }
    }
    // South
    if(m_old_y-y==-1){
        cnt = y*m_size*q_s + x*q_s;
        for(int i=0;i<q_s;i++){     
            m_h_walls[cnt+i].color = color_bgr;
        }
    }
    // West
    if(m_old_x-x==1){
        cnt = y*m_size*q_s + m_old_x*q_s;
        for(int i=0;i<q_s;i++){     
            m_v_walls[cnt+i].color = color_bgr;
        }
    }
    m_old_x = x;
    m_old_y = y;
}

void Grid::setColorQuad(const int x, const int y, const sf::Color& color_code){
    int cnt = y*m_size*q_s + x*q_s; // index based on coordinates
    
    for(int i=0;i<q_s;i++){     
        m_quads[cnt+i].color = color_code;
    }
}