// ********************************************************
// @file recursive_div.hpp
// @brief Build and display maze using recursive division algorithm
// ********************************************************

#ifndef _RECURSIVE_DIV_HPP
#define _RECURSIVE_DIV_HPP

#include <vector>
#include <utility>
#include <memory>
#include <stack>
#include "grid.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Coord{
 public:   
   int m_x = 0;
   int m_y = 0;
   int m_wid = 0;
   int m_hgt = 0;
   Coord(int x, int y, int wid, int hgt):
   m_x(x),
   m_y(y),
   m_wid(wid),
   m_hgt(hgt)
   {};

   bool operator<(const Coord &other) const
   {
   if (m_x < other.m_x)
      return true;
   if (m_x == other.m_x && m_y < other.m_y)
      return true;
   if (m_x == other.m_x && m_y == other.m_y && m_wid < other.m_wid)
      return true;
   return (m_x == other.m_x && m_y == other.m_y && m_wid == other.m_wid && m_hgt < other.m_hgt);
   }
};

struct EmptyWall{
   int m_x = 0;
   int m_y = 0;
   int m_orientation = 0;
   EmptyWall(int x, int y, int orientation):
   m_x(x),
   m_y(y),
   m_orientation(orientation)
   {};
};

class RecursiveDiv : public sf::Drawable{
 public:
   RecursiveDiv(std::shared_ptr<Grid> sp_grid, sf::RenderWindow& window);
   ~RecursiveDiv();
   bool display();
 private:
   sf::RenderWindow& m_window;
   sf::VertexBuffer m_vert_walls;
   sf::VertexBuffer m_horz_walls;
   sf::VertexBuffer m_walls;
   std::vector<sf::Vertex> m_vect_walls;
   std::vector<std::pair<int,int>> m_emptyWall_horz; // <X,Y,orientation>
   std::vector<std::pair<int,int>> m_emptyWall_vert; // <X,Y,orientation> 
   std::vector<Coord> m_walls_coord; //<X,Y,width,height> of walls to be shown     
   std::stack<Coord> m_que_halves; // queue of coord and dimenssions of unprocessed halves 
   std::set<Coord> m_visited_halves;
   const int m_VERT = 1; 
   const int m_HORZ = 0; 
   std::shared_ptr<Grid> m_sp_grid;
   virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
   int chooseOrientation(int width, int height);
   void split(int old_wid, int old_hgt, int width, int height);
   void showWall();
};
#endif