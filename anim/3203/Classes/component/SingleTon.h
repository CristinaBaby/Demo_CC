//
//  SingleTon.h
//  Burgers
//
//  Created by tanshoumei on 13-6-26.
//
//

#ifndef Burgers_SingleTon_h
#define Burgers_SingleTon_h

//模板单例基类
template<class T>
class SingleTon
{
public:
    virtual ~SingleTon()
    {        
    }
    static T* getInstance()
    {
        static T instance;
        return &instance;
    }
};


#endif
