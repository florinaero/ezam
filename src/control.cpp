#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include "control.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include "kruskal.hpp"

Control::Control(int width_win, int height_win, int size):
m_window(sf::VideoMode(width_win, height_win), "ezam", sf::Style::Default),
m_grid(size, m_window),
m_input_size(size)
{   
    //  NEVER use together setVerticalSyncEnabled() with setFramerateLimit()
    // m_window.setVerticalSyncEnabled(true); 
    // m_window.setFramerateLimit(60);
    // m_window.setPosition(sf::Vector2i(900, -1080));
    m_window.setPosition(sf::Vector2i(0, 0));   // top-left
    Control::setFont();
};

// dtor
Control::~Control(){
}
void Control::setInputSize(int size) {
    m_input_size = size;
}
void Control::initialize(){
    // empty
}

//************************************************************
//      Process and display
//************************************************************
void Control::run(){
    bool title_on = true;        
    static bool is_running = true;
    bool is_krusk_running = true;
    bool is_dft_running = true;
    
    std::shared_ptr<Grid> sp_grid = std::make_shared<Grid>(Grid(m_input_size, m_window));   
    Maze maze(m_input_size, sp_grid);
    Kruskal krusk(m_input_size,m_input_size,sp_grid); 

    setTitle();
    Control::initialize();

    while (m_window.isOpen()){
        sf::Event event;
        sf::Text size_text;            
        size_text.setCharacterSize(50);
        static sf::String size_input;     
        sf::Clock clock;
        
        // Display title page 
        for(auto& elem : m_frames){
            if(elem.second.first==true)
            m_window.draw(elem.second.second);
        }
        // Start Kruskal maze after title page
        if(m_frames.at("title").first == false){            
            if(is_krusk_running){               
                is_krusk_running = krusk.display(m_window);
            }
        }
        // Showing maze with DFS
        if(is_krusk_running==false&&is_dft_running==true){
            sp_grid->setColorAllQuads(sf::Color::Red);
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            is_dft_running = maze.display(m_window);            
        }

        if(is_dft_running==false){
            // m_window.close();
        }
        if(is_running&&is_krusk_running){            
            m_window.display(); 
            m_window.clear(sf::Color::Cyan);         
            sf::Time time = clock.getElapsedTime();
            // std::cout << "fps: " << 1.0f/time.asSeconds() << std::endl;
            clock.restart();  
        }
        else{
            // Avoid clearing
        }

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
    }
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



