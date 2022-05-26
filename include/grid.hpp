#ifndef _GRID_HPP
#define _GRID_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <deque>
#include <set>

class Grid : public sf::Drawable
{
public:
    Grid();
    Grid(int size, const std::shared_ptr<sf::RenderWindow> sp_window);
    // Set quads color based on grid coordinates
    void setColorQuad(const int x, const int y, const sf::Color color_code);

    //@brief: Get color of all quads as a vector with exact size of quads
    //@return False if size of vector is smaller than number of quads
    bool getColorQuad(const int x, const int y, std::vector<sf::Color> &color_codes);
    // Set color for all quads
    void setColorAllQuads(const sf::Color color_code);
    // Set color for vertical wall(column) based on index from left to right
    void setColorWallVert(const int index, const sf::Color color_code);
    // Set color for horizontal wall(row) based on index from up to bottom
    void setColorWallHorz(const int index, const sf::Color color_code);
    // Set color of all horizontal walls
    void setColorAllWallsHorz(const sf::Color color_code);
    // Set color of all vertical walls
    void setColorAllWallsVert(const sf::Color color_code);
    // Remove wall between 2 adjacent cells on horizontal or vertical
    void removeWall(const int cell_1, const int cell_2, sf::Color color);
    // ~Grid();

    static inline const int q_s = 4; // number of lines for one quad
    sf::VertexArray m_outline;
    sf::VertexArray m_quads;
    sf::VertexArray m_v_walls;
    sf::VertexArray m_h_walls;
    sf::VertexArray m_row_walls;
    sf::VertexArray m_col_walls;
    sf::VertexArray m_removed_walls;
    sf::VertexArray m_head_q;

    int getSqrW() const;
    int getSqrH() const;
    int getWallThick() const;
    int getOutlineW() const;
    int getSize() const;

private:
    // Create window's outlien based on window dimenssions
    void createOutline();
    // Create grid based on window dimenssions
    void createGrid();
    // Create walls based on widnow dimenssions
    void createWalls();

    void setSize(const int size);

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // ... or use the low-level API
        states.texture = &m_texture;
        // Draw grid
        target.draw(m_quads, states);
        target.draw(m_row_walls, states);
        target.draw(m_col_walls, states);
        target.draw(m_removed_walls, states);
        target.draw(m_outline, states);
        // Draw head
        target.draw(m_head_q, states);
        // ... or draw with OpenGL directly
    }

    sf::Sprite m_sprite;
    sf::Texture m_texture;
    const std::shared_ptr<sf::RenderWindow> m_sp_window;
    int m_size;
    int m_thick;
    int m_old_x;
    int m_old_y;
    int m_no_walls;  // number of inside walls for m*m grid
    int m_sqr_w;     // square width
    int m_sqr_h;     // square height
    int m_outline_w; // outline width
};

#endif