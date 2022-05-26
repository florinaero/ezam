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
#include "utils.hpp"

class Control
{
public:
    // ctor
    Control();
    // ctor for window's size
    Control(int width_win, int height_win, int size);
    // dtor
    ~Control();
    void setInputSize(int size);
    void run();
    static bool isMouseOnArea(const std::shared_ptr<sf::RenderWindow> sp_window, const sf::FloatRect &area_coord);

private:
    int m_input_size;
    sf::Font m_font;
    std::unordered_map<std::string, std::pair<bool, sf::Text>> m_frames;
    std::shared_ptr<sf::RenderWindow> m_sp_window;

    bool showMaze();
    // Set Title page
    void setTitle();
    ////////////////////////////////////////////////////////////
    /// @brief Set the title of the next stage
    ////////////////////////////////////////////////////////////
    sf::Text setStageName(std::string name);
    // Set font
    bool setFont();
    void initialize();
    bool displayMaze(std::string stage_name, std::shared_ptr<Grid> sp_grid, utils::ObjectMaze &obj_maze);
};

#endif
