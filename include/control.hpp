#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <unordered_map>
#include <utility>
#include <iostream>
// User
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "maze.hpp"
#include "grid.hpp"

class Control {
public:    
    // ctor
    Control();
    // ctor for window's size
    Control(int width_win, int height_win, int size);
    // dtor
    ~Control();
    void setInputSize(int size);
    void run();
    static bool isMouseOnArea(const sf::RenderWindow& window, const sf::FloatRect& area_coord);

private: 
    int m_input_size;
    sf::Font m_font;
    std::unordered_map<std::string, std::pair<bool, sf::Text>> m_frames;
    sf::RenderWindow m_window;
    Grid m_grid;

    bool showMaze();
    // Set Title page 
    void setTitle();
    ////////////////////////////////////////////////////////////
    /// @brief Set the title of the next stage 
    ////////////////////////////////////////////////////////////
    std::pair<sf::Text, bool> setStageName(std::string name);
    // Set font 
    bool setFont();
    void initialize();
    bool displayKrusk(std::shared_ptr<Grid> sp_grid);
    bool displayDft(std::shared_ptr<Grid> sp_grid);
    bool displayRecDiv(std::shared_ptr<Grid> sp_grid);
};

#endif
