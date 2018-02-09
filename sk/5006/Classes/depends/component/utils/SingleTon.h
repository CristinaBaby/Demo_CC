//
//  SingleTon.h
//  Burgers
//
//  Created by tanshoumei on 13-6-26.
//
//

#ifndef CODEBASE_CORE_UTILS_SINGLETON_H_
#define CODEBASE_CORE_UTILS_SINGLETON_H_

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


#endif /* define (CODEBASE_CORE_UTILS_SINGLETON_H_)*/
