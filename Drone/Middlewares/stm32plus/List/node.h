//
// Created by Administrator on 2019/2/25.
//

#ifndef STM32_PLUS_NODE_H
#define STM32_PLUS_NODE_H

template <typename T>
class node
{
public:
    node();
    explicit node(T);

public:
    T data;
    node<T>* next;
};




#endif //STM32_PLUS_NODE_H
