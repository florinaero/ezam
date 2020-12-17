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
    void setCoord(const int x, const int y);
    static bool isMouseOnArea(const sf::RenderWindow& window, const sf::FloatRect& area_coord);
private: 
    unsigned int m_width;
    unsigned int m_height;
    int m_input_size;
    int m_x;
    int m_y;
    sf::Font m_font;
    std::unordered_map<std::string, std::pair<bool, sf::Text>> m_frames;

    // Set Title page 
    void setTitle();

    // Set font 
    bool setFont();

    void createGrid(int size);

};

#endif
