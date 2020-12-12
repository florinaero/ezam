#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <utility>
#include <iostream>

class Display {
public:    
    // ctor
    Display();
    // Construct with window size
    Display(unsigned int width, unsigned int height);

    void setInputSize(int size);

    void start();
    
    static bool isMouseOnArea(const sf::RenderWindow& window, const sf::FloatRect& area_coord);
private: 
    unsigned int m_width;
    unsigned int m_height;
    int m_input_size;
    sf::Font m_font;
    std::unordered_map<std::string, std::pair<bool, sf::Text>> m_frames;
    sf::VertexArray m_grid;

    // Set Title page 
    void setTitle();

    // Set font 
    bool setFont();

    void createGrid(int size);

};

class MyDrawable : public sf::Drawable
{
public:

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        std::cout << "Call my draw!" << std::endl;
        // You can draw other high-level objects
        target.draw(m_sprite, states);
        // ... or use the low-level API
        states.texture = &m_texture;
        target.draw(m_vertices, states);
        // ... or draw with OpenGL directly
    }
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::VertexArray m_vertices;
};
#endif
