#ifndef _GRID_HPP
#define _GRID_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <deque>

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
    // Create walls based on widnow dimenssions
    void createWalls(); 

    void setSize(const int size);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {           
        // ... or use the low-level API
        states.texture = &m_texture;
        // Draw grid
        target.draw(m_quads, states);
        target.draw(m_v_walls, states);
        target.draw(m_h_walls, states);
        // Draw head
        target.draw(m_head_q, states);
        // ... or draw with OpenGL directly
    }
    
    sf::VertexArray m_quads;
    sf::VertexArray m_v_walls;
    sf::VertexArray m_h_walls;
    sf::VertexArray m_head_q;
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::VertexArray m_vertices;
    std::deque<sf::Vertex> m_seen_q; // Seen quads by head 
    const sf::RenderWindow& m_window;
    int m_size;
    int m_sqr_w; // square width
    int m_sqr_h; // square height
    int m_old_x; 
    int m_old_y; 
    static inline const int q_s = 4; // number of lines for one quad
};

#endif