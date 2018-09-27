//
// Created by forest on 26/09/18.
//

#ifndef TRADING_INDEXREFERENCE_HPP
#define TRADING_INDEXREFERENCE_HPP

#include <vector>
namespace trading {
    template<typename ValueType, typename ContainerType, typename IndexType>
    class IndexReference {
    public:
        IndexReference(ContainerType  &container, IndexType index):container{container},index{index},valid{true}{};
        IndexReference(ContainerType  &container):valid{false},container{container}{};

        ValueType* operator->() {
            return &container[index];
        }
        operator ValueType&(){
            return container[index];
        }
        ValueType& get(){
            return container[index];
        }
    private:
        ContainerType  &container;
        IndexType index;
        bool valid;
    };

    template<typename T>
    using VectorReference =IndexReference<T, std::vector<T>, long >;

}
#endif //TRADING_INDEXREFERENCE_HPP
