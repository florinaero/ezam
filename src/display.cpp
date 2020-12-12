#include "display.hpp"
#include <iostream>

Display::Display(unsigned int width, unsigned int height):
m_width(width),
m_height(height),
m_grid(sf::Quads, 8)
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
    createGrid(0);
    
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
        
        MyDrawable myDraw;
        window.draw(myDraw);
        window.clear(sf::Color::Cyan);
        // Display title page
        for(auto& elem : m_frames){
            if(elem.second.first==true)
            window.draw(elem.second.second);
        }
        if(m_frames.at("title").first == false){
            window.draw(m_grid);
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

void Display::createGrid(int size){

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

