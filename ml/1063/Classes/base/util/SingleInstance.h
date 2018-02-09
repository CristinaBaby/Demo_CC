//
//  SingleTon.h
//  Burgers
//
//  Created by tanshoumei on 13-6-26.
//
//

#ifndef Burgers_SingleInstance_h
#define Burgers_SingleInstance_h

//模板单例基类
template<class T>
class SingleInstance
{
public:
    virtual ~SingleInstance()
    {        
    }
    static T* getInstance()
    {
        if(instance == nullptr)
            instance = new T;
        return instance;
    }
protected:
    SingleInstance() {
        
    }
    static T* instance;
};
template<class T> T* SingleInstance<T>::instance = nullptr;
#endif
