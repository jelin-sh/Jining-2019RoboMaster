//
// Created by Administrator on 2019/2/25.
//

#include "node.h"

template<typename T>
node<T>::node()
{
    next = nullptr;
}

template<typename T>
node<T>::node(T _data)
{
    data = _data;
    next = nullptr;
}
