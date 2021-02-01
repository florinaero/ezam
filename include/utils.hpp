// ********************************************************
// @file  utils.hpp
// @brief A bunch of general stuff
// ********************************************************

#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>

namespace utils{

    namespace clr{
        const uint32_t pistachio = 0x93C572FF;
        const uint32_t green = 0x5BE11AFF;
        const uint32_t cyan = 0x27FBDFFF;
    }

    // Return random number in closed interval
    int32_t randRange(const int32_t start, const int32_t stop);    
    
    class ObjectMaze{
    public:
        virtual bool display(std::shared_ptr<sf::RenderWindow> window) = 0;
    };
}

#endif 