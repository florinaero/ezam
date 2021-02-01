#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include "control.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include "kruskal.hpp"
#include "recursive_div.hpp"

Control::Control(int width_win, int height_win, int size):
m_sp_window(std::make_shared<sf::RenderWindow>
    (sf::VideoMode(width_win, height_win), "ezam", sf::Style::Default)),
m_input_size(size)
{   
    //  NEVER use together setVerticalSyncEnabled() with setFramerateLimit()
    // m_sp_window->setVerticalSyncEnabled(true); 
    // m_sp_window->setFramerateLimit(60);
    m_sp_window->setPosition(sf::Vector2i(0, 0));   // top-left
    // m_sp_window->setPosition(sf::Vector2i(1000, -1180));
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
    static bool is_running = true;
    bool title_on = true;        
    bool is_rec_running = true;
    bool is_krusk_running = true;
    bool is_dft_running = true;
    // bool is_krusk_running = false;
    // bool is_dft_running = false;
    
    std::shared_ptr<Grid> sp_grid = std::make_shared<Grid>
        (Grid(m_input_size, m_sp_window));   
    Control::initialize();
    setTitle();

    while (m_sp_window->isOpen()){
        sf::Event event;
        sf::Text size_text;            
        size_text.setCharacterSize(50);
        static sf::String size_input;     
        sf::Clock clock;                
        
        // Start Kruskal maze after title page
        if(m_frames.at("title").first == false){    
            if(is_krusk_running){       
                Kruskal krusk(m_input_size,m_input_size,sp_grid);     
                is_krusk_running = displayMaze("KRUSKAL", sp_grid, krusk);
            }
        }
        // Showing maze with DFS
        if(is_krusk_running==false && is_dft_running==true){
            sp_grid->setColorAllQuads(sf::Color::Red);
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            Maze dft(m_input_size, sp_grid);
            is_dft_running = displayMaze("DFT", sp_grid, dft);            
        }
        // Show maze with recursive division
        if(is_dft_running==false && is_rec_running==true){
            RecursiveDiv rec_div(sp_grid);
            is_rec_running = displayMaze("RECURSION DIVISION", sp_grid, rec_div);
        }

        if(is_rec_running==false){
            // m_sp_window->close();
        }

        // Display title page 
        if(m_frames.at("title").first==true){
            for(auto& elem : m_frames){
                if(elem.second.first==true)
                m_sp_window->draw(elem.second.second);
            }
        }
        
        if(m_frames.at("title").first==true){            
            m_sp_window->display(); 
            m_sp_window->clear(sf::Color::Cyan);         
            sf::Time time = clock.getElapsedTime();
            std::cout << "fps: " << 1.0f/time.asSeconds() << std::endl;
            clock.restart();  
        }
        else{
            // Avoid clearing
        }

        while (m_sp_window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                m_sp_window->close();
            }
            if (event.type == sf::Event::TextEntered){
                size_input += event.text.unicode;
                size_text.setString(size_input);
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if(Control::isMouseOnArea(m_sp_window, m_frames.at("start").second.getGlobalBounds())){
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
    auto size_win = m_sp_window->getSize();
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

// Set label for next stage
sf::Text Control::setStageName(std::string name){
    sf::Text text {"GO TO: "+name, m_font, 20};
    sf::Vector2u win_size = m_sp_window->getSize();
    sf::VertexArray banner(sf::Quads,4);
    int x_pos = 0;
    int y_pos = 0;

    text.setFillColor(sf::Color::Yellow);
    text.setOutlineThickness(0);
    text.setPosition(x_pos, y_pos);
    auto bounds = text.getGlobalBounds();

    // Set banner width and position based on text
    banner[0].position = sf::Vector2f(x_pos, y_pos);
    banner[1].position = sf::Vector2f(x_pos+bounds.width, y_pos);
    banner[2].position = sf::Vector2f(x_pos+bounds.width, y_pos
                                        +2*bounds.height); 
    banner[3].position = sf::Vector2f(x_pos, y_pos+2*bounds.height);
    for(int i=0;i<4;i++){
        banner[i].color = sf::Color::Black;
    }
    m_sp_window->draw(banner);
    m_sp_window->draw(text);
    m_sp_window->display();
    return text;
}

bool Control::setFont(){
    if(m_font.loadFromFile("C:\\_Personal\\repo\\ezam\\config\\fonts\\MAIAN.tff")!=true){
        std::cout << "Error in loading font" << std::endl;
        return false;
    }
    return true;
}

bool Control::isMouseOnArea(const std::shared_ptr<sf::RenderWindow> sp_window, const sf::FloatRect& area_coord){
    auto position = sf::Mouse::getPosition(*sp_window);
    if(area_coord.contains(static_cast<sf::Vector2f>(position))){
        return true;
    }
    else{
        return false;
    }
}

bool Control::displayMaze(std::string stage_name, std::shared_ptr<Grid> sp_grid, utils::ObjectMaze& obj_maze){    
    auto stage_text = setStageName(stage_name);
    sf::Event event;

    while (m_sp_window->pollEvent(event) || true){
        if (event.type == sf::Event::Closed){
            m_sp_window->close();
        }
        if (event.type == sf::Event::MouseButtonPressed){
            if(Control::isMouseOnArea(m_sp_window, stage_text.getGlobalBounds())){
                obj_maze.display(m_sp_window);
                return false;
            }
        }
    }
    return true;
}


