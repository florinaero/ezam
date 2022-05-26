#include <random>
#include "utils.hpp"

namespace utils
{

    int32_t randRange(const int32_t start, const int32_t stop)
    {
        std::random_device rd;                                     // obtain a random number from hardware
        std::mt19937 gen(rd());                                    // seed the generator
        std::uniform_int_distribution<int32_t> distr(start, stop); // define the range

        return distr(gen); // generate number
    }
}
