#include "display.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include <thread>
#include <chrono>
#include <iostream>

Display::Display(unsigned int width, unsigned int height):
m_width(width),
m_height(height),
m_input_size(0),
m_x(0),
m_y(0)
{    
    Display::setFont();
};

void Display::setInputSize(int size) {
    m_input_size = size;
}

void Display::start(){
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(m_width, m_height), "ezam", sf::Style::Default, settings);
    Display::setTitle();
    bool title_on = true;    
    Grid grid(10, window);
    Maze maze(1);
    maze.build(0,0);
    std::vector<std::pair<int,int>> coord;
    maze.getCoord(coord);

    while (window.isOpen()){
        sf::Event event;
        sf::Text size_text;            
        size_text.setCharacterSize(50);
        static sf::String size_input;     

        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
                 
            if (event.type == sf::Event::TextEntered){
                size_input += event.text.unicode;
                size_text.setString(size_input);
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if(Display::isMouseOnArea(window, m_frames.at("start").second.getGlobalBounds())){
                    m_frames.at("title").first = false;
                    m_frames.at("start").first = false;
                } 
            }
        }
 
        window.clear(sf::Color::Cyan);
        // Display title page 
        for(auto& elem : m_frames){
            if(elem.second.first==true)
            window.draw(elem.second.second);
        }
        // Start drawing after displaying title page
        if(m_frames.at("title").first == false){    
            static int counter = 0;
            std::cout << "size = " << coord.size() << std::endl;
            if(counter<coord.size()){                                
                auto coord_pair = coord.at(counter);
                counter++;         
                std::cout << "x=" << coord_pair.first << " y=" << coord_pair.second << std::endl;
                grid.moveHead(coord_pair.first,coord_pair.second);
                std::this_thread::sleep_for(std::chrono::milliseconds{100});        
                window.draw(grid);
            }
        }
        window.display();
    }
}

void Display::setTitle() {
    const int font_size = 60;
    std::string text {"EZAM"};

    sf::Text title{text, m_font, font_size};
    auto bounds = title.getGlobalBounds();

    title.setPosition(m_width/2 - bounds.width/2 , m_height/2 - bounds.height/2);
    title.setFillColor(sf::Color::Magenta);
    title.setOutlineColor(sf::Color::Black); 
    title.setOutlineThickness(2.f);
    m_frames.emplace(std::make_pair("title", std::make_pair(true,title)));
    
    sf::Text start {"start", m_font, 40};
    start.setFillColor(sf::Color::Black);
    start.setPosition(title.getPosition().x, title.getPosition().y + bounds.height + 10);

    m_frames.emplace(std::make_pair("start", std::make_pair(true,start)));
}

bool Display::setFont(){
    if(m_font.loadFromFile("config\\fonts\\MAIAN.tff")!=true){
        std::cout << "Error in loading font" << std::endl;
        return false;
    }
    return true;
}

bool Display::isMouseOnArea(const sf::RenderWindow& window, const sf::FloatRect& area_coord){
    auto position = sf::Mouse::getPosition(window);
    if(area_coord.contains(static_cast<sf::Vector2f>(position))){
        return true;
    }
    else{
        return false;
    }
}

void Display::setCoord(const int x, const int y){
    m_x = x;
    m_y = y; 
}



