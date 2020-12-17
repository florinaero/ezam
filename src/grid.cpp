#include "grid.hpp"
#include <iostream>


Grid::Grid(int size, const sf::RenderWindow& window):
m_size(size),
m_window(window),
m_grid(sf::Quads, 8),
m_quad(sf::Quads, 4)
{
    setSize(size);
    // createGrid();
};

Grid::~Grid(){
    std::cout << "Destroy" << std::endl;
};

void Grid::createGrid(){

    sf::Vector2u dimens = m_window.getSize();
    
    m_grid[0].position = sf::Vector2f(10.f, 10.f);
    m_grid[1].position = sf::Vector2f(110.f, 10.f);
    m_grid[2].position = sf::Vector2f(110.f, 110.f);
    m_grid[3].position = sf::Vector2f(10.f, 110.f);

    
    m_grid[4].position = sf::Vector2f(110.f, 10.f);
    m_grid[5].position = sf::Vector2f(220.f, 10.f);
    m_grid[6].position = sf::Vector2f(220.f, 110.f);
    m_grid[7].position = sf::Vector2f(110.f, 110.f);

    for(int i=0;i<8;i++){
        m_grid[i].color = sf::Color::Red;
    }
}

void Grid::setSize(const int size){
    m_sqr_w = static_cast<int>(m_window.getSize().x / size);
    m_sqr_h = static_cast<int>(m_window.getSize().y / size);
    std::cout << "m_sqr_w = " << m_sqr_w << " m_sqr_h= " << m_sqr_h << std::endl;
}

void Grid::moveHead(const int x, const int y){
    // Map coordinates with window's size 
    m_quad[0].position = sf::Vector2f(m_sqr_w*x, m_sqr_h*y);
    m_quad[1].position = sf::Vector2f(m_sqr_w*x+m_sqr_w, m_sqr_h*y);
    m_quad[2].position = sf::Vector2f(m_sqr_w*x+m_sqr_w, m_sqr_h*y+m_sqr_h);
    m_quad[3].position = sf::Vector2f(m_sqr_w*x, m_sqr_h*y+m_sqr_h);

    for(int i=0;i<m_quad.getVertexCount();i++){
        m_quad[i].color = sf::Color::Green;
    }
}