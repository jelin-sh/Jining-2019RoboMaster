//
// Created by Administrator on 2019/5/1.
//

#ifndef INFENTERY_C6010_H
#define INFENTERY_C6010_H


#include <C6020/C6020.h>

class C6010: public C6020
{
public:
    C6010 (u16 port, PID *pid);
};



#endif //INFENTERY_C6010_H
