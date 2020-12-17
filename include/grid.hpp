#ifndef _GRID_HPP
#define _GRID_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Grid : public sf::Drawable
{
public:
    Grid();
    Grid(int size, const sf::RenderWindow& window);
    // Map coordinates to window size 
    void moveHead(const int x, const int y);
    ~Grid();
private:
    // Create grid based on window dimenssions
    void createGrid();
    void setSize(const int size);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {           
        // You can draw other high-level objects
        target.draw(m_quad, states);
        // ... or use the low-level API
        states.texture = &m_texture;
        target.draw(m_vertices, states);
        // ... or draw with OpenGL directly
    }
    
    sf::VertexArray m_grid;
    sf::VertexArray m_quad;
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::VertexArray m_vertices;
    const sf::RenderWindow& m_window;
    int m_size;
    int m_sqr_w; // square width
    int m_sqr_h; // square height
};

#endif