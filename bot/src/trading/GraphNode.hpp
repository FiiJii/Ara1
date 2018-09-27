//
// Created by forest on 26/09/18.
//

#ifndef TRADING_GRAPHNODE_HPP
#define TRADING_GRAPHNODE_HPP

#include <vector>

namespace trading {
template <typename ValueType,template<typename> typename ReferenceType>
    class GraphNode {
    public:
        ValueType value;
        std::vector<ReferenceType<GraphNode>> neighbors;
    };
}


#endif //TRADING_GRAPHNODE_HPP
