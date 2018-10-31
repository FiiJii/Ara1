//
// Created by forest on 03/10/18.
//

#include "utils.hpp"
#include <sstream>
double s2d(const std::string &str, const std::locale &loc)
    {
        double val=0;
        std::stringstream ss(str);
        ss.imbue(loc);
        ss>>val;
        return val;
    }

