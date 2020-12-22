#include "control.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include <thread>
#include <chrono>
#include <iostream>

Control::Control(int width_win, int height_win, int size):
m_window(sf::VideoMode(width_win, height_win), "ezam", sf::Style::Default),
m_maze(size),
m_grid(size, m_window),
m_input_size(0),
m_start_x(0),
m_start_y(0)
{   
    //  NEVER use together setVerticalSyncEnabled() with setFramerateLimit()
    // m_window.setVerticalSyncEnabled(true); 
    // m_window.setFramerateLimit(60);
    Control::setFont();
};

void Control::setInputSize(int size) {
    m_input_size = size;
}
void Control::initialize(){
    // Build maze
    m_maze.buildDFS(m_start_x, m_start_y);
}

void Control::setStart(int x, int y){
    m_start_x = x;
    m_start_y = y;
}

void Control::run(){
    setTitle();
    Control::initialize();
    bool title_on = true;        
    static bool is_running = true;

    while (m_window.isOpen()){
        sf::Event event;
        sf::Text size_text;            
        size_text.setCharacterSize(50);
        static sf::String size_input;     
        sf::Clock clock;

        while (m_window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                m_window.close();
            }
                 
            if (event.type == sf::Event::TextEntered){
                size_input += event.text.unicode;
                size_text.setString(size_input);
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if(Control::isMouseOnArea(m_window, m_frames.at("start").second.getGlobalBounds())){
                    m_frames.at("title").first = false;
                    m_frames.at("start").first = false;
                } 
            }
        }
        
        // Display title page 
        for(auto& elem : m_frames){
            if(elem.second.first==true)
            m_window.draw(elem.second.second);
        }
        // Start drawing after displaying title page
        if(m_frames.at("title").first == false){
            is_running = Control::showMaze();                
        }
        
        if(is_running){
            m_window.display(); 
            m_window.clear(sf::Color::Cyan);         
            sf::Time time = clock.getElapsedTime();
            std::cout << "fps: " << 1.0f/time.asSeconds() << std::endl;
            clock.restart();  
        }
        else{
            // Avoid clearing
        }
    }
}

// Create display content for maze coordinates
bool Control::showMaze(){
    std::vector<std::pair<int,int>> coord;
    m_maze.getCoord(coord);
    static int counter = 0;
    bool is_running = false;

    if(counter<coord.size()){                                
        auto coord_pair = coord.at(counter);
        counter++;         
        // std::cout << "x=" << coord_pair.first << " y=" << coord_pair.second << std::endl;
        m_grid.moveHead(coord_pair.first,coord_pair.second);
        std::this_thread::sleep_for(std::chrono::milliseconds{10});        
        m_window.draw(m_grid);
        is_running = true;
    }
    else{
        is_running = false;
    }
    return is_running;
}

void Control::setTitle() {
    const int font_size = 60;
    std::string text {"EZAM"};

    sf::Text title{text, m_font, font_size};
    auto bounds = title.getGlobalBounds();
    auto size_win = m_window.getSize();
    title.setPosition(size_win.x/2 - bounds.width/2 , size_win.y/2 - bounds.height/2);
    title.setFillColor(sf::Color::Magenta);
    title.setOutlineColor(sf::Color::Black); 
    title.setOutlineThickness(2.f);
    m_frames.emplace(std::make_pair("title", std::make_pair(true,title)));
    
    sf::Text start {"start", m_font, 40};
    start.setFillColor(sf::Color::Black);
    start.setPosition(title.getPosition().x, title.getPosition().y + bounds.height + 10);

    m_frames.emplace(std::make_pair("start", std::make_pair(true,start)));
}

bool Control::setFont(){
    if(m_font.loadFromFile("C:\\_Personal\\repo\\ezam\\config\\fonts\\MAIAN.tff")!=true){
        std::cout << "Error in loading font" << std::endl;
        return false;
    }
    return true;
}

bool Control::isMouseOnArea(const sf::RenderWindow& window, const sf::FloatRect& area_coord){
    auto position = sf::Mouse::getPosition(window);
    if(area_coord.contains(static_cast<sf::Vector2f>(position))){
        return true;
    }
    else{
        return false;
    }
}




