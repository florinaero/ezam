#include <thread>
#include <chrono>
#include <iostream>
#include "recursive_div.hpp"
#include "logging.hpp"

RecursiveDiv::RecursiveDiv(std::shared_ptr<Grid> sp_grid)
    : m_walls(sf::Quads),
      m_sp_grid(sp_grid)
{}

RecursiveDiv::~RecursiveDiv(){}

void RecursiveDiv::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sp_grid->m_outline, states);
    target.draw(m_sp_grid->m_quads, states);
    target.draw(m_vect_walls.data(), m_vect_walls.size(),sf::Quads, states);
}

bool RecursiveDiv::display(std::shared_ptr<sf::RenderWindow> window){
    m_window = window;
    m_sp_grid->setColorAllQuads(sf::Color::Yellow);
    // Start displying
    split(0, 0, m_sp_grid->getSize(), m_sp_grid->getSize()); 
    return false;
}

// @details Origin is top-left
// @param[in] x   Origin's coordinate X of chosen quadrilateral
// @param[in] y   Origin's coordinate Y of chosen quadrilateral
// @param[in] wid Width of chosen quadrilateral
// @param[in] hgt Height of chosen quadrilateral
void RecursiveDiv::split(int x, int y, int wid, int hgt){
    int col = 0;
    int row = 0;
    int new_wid = 0;
    int new_hgt = 0;
    int new_x = 0;
    int new_y = 0;
    int rest_x = 0; // coord of the other half
    int rest_y = 0; // coord of the other half
    static int counter = 0;           
    counter++;

    if(wid<2 || hgt<2){
        return;
    }

    // Select splitting orientation on short edge 
    int orient = chooseOrientation(wid, hgt);

    // Split on vertical
    if(orient==m_VERT){
        // Select column between X origin and width
        col = utils::randRange(x+1, x+wid-1);

        // Select wall between Y origin and height, select from vertical line
        int rowEmpty = hgt!=1 ? (utils::randRange(y+1, y+hgt-1)) : y; // Start from next coord and stop one before because walls index is considered from cell's top      
        m_walls_coord.push_back(Coord(col, y, 0, hgt));
        m_emptyWall_vert.push_back(std::make_pair(col, rowEmpty)); // <X,Y>            
    }
    
    // Split on horizontal
    if(orient==m_HORZ){
        // Select range on y axis
        row = utils::randRange(y+1, y+hgt-1);
        // In case that wid is 1 select column        
        int colEmpty = wid!=1 ? (utils::randRange(x+1, x+wid-1)) : x; // Same as vertical
        m_walls_coord.push_back(Coord(x, row, wid, 0));
        m_emptyWall_horz.push_back(std::make_pair(colEmpty, row)); // <X,Y>
    }

    auto elem = m_visited_halves.emplace(m_walls_coord.back());

    // Choose one of 2 generated halves
    int half = rand()%2; // 1 for halves from north or west and 0 for south or est
    if(half==1){ // north or west -> same origin as before
        if(orient==m_VERT){
            new_wid=col-x; // new width is column's index minus origin X coordinate 
            new_hgt=hgt;   // height is unchanged
            rest_x = col;  // other half X is column index
            rest_y = y;    // other half Y is origin Y
        }
        if(orient==m_HORZ){
            new_wid=wid;
            new_hgt=row-y;
            rest_x = x;
            rest_y = row;
        }
        new_x = x;
        new_y = y;        
    }

    if(half==0){ // south or est
        if(orient==m_VERT){
            new_wid=(x+wid)-col; // Last coordinate minus new column used for split
            new_hgt=hgt;
            new_x=col;
            new_y=y;
        }
        if(orient==m_HORZ){
            new_wid=wid;
            new_hgt=(y+hgt)-row;
            new_x=x;
            new_y=row;
        }
        rest_x = x; // coordinates of untouched half are the same as the origin of initial square
        rest_y = y;
    }

    this->showWall();
    
    // Coordinates and dimenssions of untouched half
    // Width of untouched half is original width minus new width(choosen half), same for height
    int storage_wid = (wid==new_wid ? wid : wid-new_wid); // Avoid storage of 0 when halves are equal(1st case)
    int storage_hgt = (hgt==new_hgt ? hgt : hgt-new_hgt);
    // Avoid storage of under resolution halves to avoid generation of wrong random coordinates
    if(storage_wid>1 && storage_hgt>1){            
        Coord data_half(rest_x, rest_y, storage_wid, storage_hgt);
        m_que_halves.emplace(data_half);
    }
    // Change focus to another half when resolution of 1 is reached
    if((new_wid<2 || new_hgt<2) && !m_que_halves.empty()){         
        auto coord = m_que_halves.top();
        new_x = coord.m_x;
        new_y = coord.m_y;
        new_wid = coord.m_wid;
        new_hgt = coord.m_hgt;
        m_que_halves.pop();
    }
    else{       
    }

    if(logging::OFF){
        std::cout <<"\n"<< counter <<". ";
        std::cout << " row = " << row;
        std::cout << " col = " << col;
        std::cout << " half = " << half;
        std::cout << " orientation = " << (orient==1 ? "vertical":"horizontal"); 
        std::cout << " new_x = " << new_x; 
        std::cout << " new_y = " << new_y; 
        std::cout << " new_wid = " << new_wid; 
        std::cout << " new_hgt = " << new_hgt << std::endl;
    }
    split(new_x, new_y, new_wid, new_hgt);
}

void RecursiveDiv::showWall(){
    auto elem = m_walls_coord.back();
    int x = elem.m_x;
    int y = elem.m_y;
    int wid = elem.m_wid;
    int hgt = elem.m_hgt;
    int w_quad = m_sp_grid->getSqrW();
    int h_quad = m_sp_grid->getSqrH();
    int out = m_sp_grid->getOutlineW();
    int tck = m_sp_grid->getWallThick()/2;

    // Entire Wall 
    m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out,y*h_quad+out)));
    m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+wid*w_quad+tck,y*h_quad+out)));
    m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+wid*w_quad+tck,y*h_quad+out+hgt*h_quad+1*tck)));
    m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out, y*h_quad+out+hgt*h_quad+1*tck)));
    // Last 4 pushed elements
    m_vect_walls.end()[-1].color = sf::Color(utils::clr::green);        
    m_vect_walls.end()[-2].color = sf::Color(utils::clr::green);        
    m_vect_walls.end()[-3].color = sf::Color(utils::clr::green);        
    m_vect_walls.end()[-4].color = sf::Color(utils::clr::green);            
    
    // Horizontal empty wall
    if(hgt==0){
        x = m_emptyWall_horz.back().first;
        y = m_emptyWall_horz.back().second;
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+1/2*tck,        y*h_quad+out)));
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+w_quad+1/2*tck, y*h_quad+out)));
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+w_quad+1/2*tck, y*h_quad+out+tck)));
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+1/2*tck,        y*h_quad+out+tck)));
    }
    // Vertical empty wall
    if(wid==0){
        x = m_emptyWall_vert.back().first;
        y = m_emptyWall_vert.back().second;
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out,    y*h_quad+out+0*tck)));
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+tck,y*h_quad+out+0*tck)));
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out+tck,y*h_quad+out+h_quad+0*tck)));
        m_vect_walls.push_back(sf::Vertex(sf::Vector2f(x*w_quad+out,    y*h_quad+out+h_quad+0*tck)));
    }

    // Last 4 pushed elements
    m_vect_walls.end()[-1].color = sf::Color(sf::Color::Yellow);        
    m_vect_walls.end()[-2].color = sf::Color(sf::Color::Yellow);        
    m_vect_walls.end()[-3].color = sf::Color(sf::Color::Yellow);        
    m_vect_walls.end()[-4].color = sf::Color(sf::Color::Yellow);                

    // m_window.clear(sf::Color::White);
    m_window->draw(*this);
    m_window->display();
    std::this_thread::sleep_for(std::chrono::milliseconds{50});     
}

// Choose direction of split considering actual size, avoid split on long edge
int RecursiveDiv::chooseOrientation(int width, int height){
    if(width>height){
        return m_VERT;
    }
    else if(width<height){
        return m_HORZ;
    }
    else{
        return rand()%2==0 ? m_HORZ : m_VERT;
    }      
}