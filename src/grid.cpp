#include "grid.hpp"
#include <iostream>


Grid::Grid(int size, const sf::RenderWindow& window):
m_size(size),
m_window(window),
m_quads(sf::Quads, size*size*q_s),
m_v_walls(sf::Quads, (size+1)*size*q_s),
m_h_walls(sf::Quads, (size+1)*size*q_s),
m_head_q(sf::Quads, q_s), 
m_seen_q(size*size*q_s),
m_old_x(0), 
m_old_y(0)
{
    setSize(size);
    createGrid();
    createWalls();
};

Grid::~Grid(){
    std::cout << "Destroy" << std::endl;
};

void Grid::createGrid(){
    int cnt = 0;
    for(int i=0;i<m_size*m_size;i++){    
        int x = i%m_size;
        int y = i/m_size;
        m_quads[cnt].position   = sf::Vector2f(m_sqr_w*x, m_sqr_h*y);                 // top-left
        m_quads[cnt+1].position = sf::Vector2f(m_sqr_w*x+m_sqr_w, m_sqr_h*y);         // top-right
        m_quads[cnt+2].position = sf::Vector2f(m_sqr_w*x+m_sqr_w, m_sqr_h*y+m_sqr_h); // bottom-right
        m_quads[cnt+3].position = sf::Vector2f(m_sqr_w*x, m_sqr_h*y+m_sqr_h);         // bottom-left
        m_quads[cnt].color   = sf::Color::Yellow;
        m_quads[cnt+1].color = sf::Color::Yellow;
        m_quads[cnt+2].color = sf::Color::Black;
        m_quads[cnt+3].color = sf::Color::Yellow;
        cnt = cnt + 4;        
    }
}

void Grid::createWalls(){
    sf::Color color_wall = sf::Color::Black;    
    int space = m_sqr_w - m_thick;
    int cnt = 0;
    for(int i=0;i<(m_size+1)*m_size;i++){
        int x = i%m_size;   // count on line
        int y = i/m_size;   // count on column
        // Vertical walls
        m_v_walls[cnt].position   = sf::Vector2f(m_sqr_w*x, m_sqr_h*y);               // top-left
        m_v_walls[cnt+1].position = sf::Vector2f(m_sqr_w*x+m_thick, m_sqr_h*y);         // top-right
        m_v_walls[cnt+2].position = sf::Vector2f(m_sqr_w*x+m_thick, m_sqr_h*y+m_sqr_h+m_thick); // bottom-right
        m_v_walls[cnt+3].position = sf::Vector2f(m_sqr_w*x, m_sqr_h*y+m_sqr_h+m_thick);       // bottom-left
        m_v_walls[cnt].color   = color_wall;
        m_v_walls[cnt+1].color = color_wall;
        m_v_walls[cnt+2].color = color_wall;
        m_v_walls[cnt+3].color = color_wall;
        // Horizontal walls
        m_h_walls[cnt].position   = sf::Vector2f(m_sqr_w*x, m_sqr_h*y);               // top-left
        m_h_walls[cnt+1].position = sf::Vector2f(m_sqr_w*x+m_sqr_w+m_thick, m_sqr_h*y);       // top-right
        m_h_walls[cnt+2].position = sf::Vector2f(m_sqr_w*x+m_sqr_w+m_thick, m_sqr_h*y+m_thick); // bottom-right
        m_h_walls[cnt+3].position = sf::Vector2f(m_sqr_w*x, m_sqr_h*y+m_thick);         // bottom-left
        m_h_walls[cnt].color   = color_wall;
        m_h_walls[cnt+1].color = color_wall;
        m_h_walls[cnt+2].color = color_wall;
        m_h_walls[cnt+3].color = color_wall;
        cnt = cnt + 4;
    }
}

void Grid::setSize(const int size){
    m_sqr_w = static_cast<int>(m_window.getSize().x / size);
    m_sqr_h = static_cast<int>(m_window.getSize().y / size);
    m_thick = 0.5 * m_sqr_w;
    std::cout << "m_sqr_w = " << m_sqr_w << " m_sqr_h= " << m_sqr_h << std::endl;
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
