//
// Created by Administrator on 2019/2/25.
//

#include "Plus_List.h"
#include "node.cpp"

template<typename T>
Plus_List<T>::Plus_List()
{
    head = new node<T>();
}

template<typename T>
T Plus_List<T>::get(u16 index)
{
    node<T>* p = head;
    u16 count = 0;
    while(p != nullptr)
    {
        if(count == index+1)
        {
            return p->data;
        }
        p = p->next;
        count++;
    }
    return (T) nullptr;
}

template<typename T>
u16 Plus_List<T>::index(T data)
{
    node<T>* p = head;
    u16 count = 0;
    while(p->next != nullptr)
    {
        if(p->data == data)
        {
            return (u16)(count-1);
        }
        p = p->next;
        count++;
    }
    return 0;
}

template<typename T>
u16 Plus_List<T>::length()
{
    node<T>* p = head;
    u16 count = 0;
    while(p->next != nullptr)
    {
        p = p->next;
        count++;
    }
    return count;
}

template<typename T>
u16 Plus_List<T>::add(T data)
{
    node<T>* p = head;
    u16 count = 0;
    while(p->next != nullptr)
    {
        p = p->next;
        count++;
    }
    p->next = new node<T>(data);
    return count;
}



