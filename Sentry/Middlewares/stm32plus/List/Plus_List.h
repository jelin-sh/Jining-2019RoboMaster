//
// Created by Administrator on 2019/2/25.
//

#ifndef STM32_PLUS_LIST_H
#define STM32_PLUS_LIST_H

#include "stm32plus.h"
#include "node.h"


template <typename T>
class Plus_List
{
private:
    node<T>* head;
public:
    Plus_List();

    u16 add(T data);

    T get(u16 index);

    virtual u16 index(T data);

    u16 length();

};
#endif //STM32_PLUS_LIST_H
