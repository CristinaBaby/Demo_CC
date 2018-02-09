//
//  DynBase.h
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/18.
//
//

#ifndef __MermaidSeaKingdom__DynBase__
#define __MermaidSeaKingdom__DynBase__

#include <iostream>
class DynObject;
class DynClassInfo;
typedef DynObject* (*createFun)(void);
bool Register(DynClassInfo*);
using namespace std;

class DynClassInfo {
public:
    DynClassInfo(std::string className,createFun fun)
    : _className(className),_fun(fun){
        Register(this);
    }
    virtual ~DynClassInfo(){}
    DynObject* createDynObject(){
        return _fun?(*_fun)():nullptr;
    }
    bool isDynamic() {return nullptr != _fun;}
    string getClassName(){return _className;}
    createFun getCreateFun() {return _fun;}
protected:
    string _className;
    createFun _fun;
};



#define DECLARE_DYNCLASS(name)\
protected:\
static DynClassInfo _classInfo;\
public:\
virtual DynClassInfo* getClassInfo();\
static DynObject* createObject();\

#define IMPLEMENT_DYNCLASS_COMMON(name,func)\
DynClassInfo  name::_classInfo((#name),(createFun)func);\
DynClassInfo* name::getClassInfo(){return &name::_classInfo;}

#define IMPLEMENT_DYNCLASS(name) \
IMPLEMENT_DYNCLASS_COMMON(name,name::createObject)\
DynObject* name::createObject(){return  new name;}


#define IMPLEMENT_COCOS2DX_CLASS(name)\
IMPLEMENT_DYNCLASS_COMMON(name,name::createObject)\
DynObject* name::createObject(){return name::create();}


class DynObject {
    DECLARE_DYNCLASS(DynObject)
public:
    DynObject(){}
    virtual ~DynObject(){}
    static bool Register(DynClassInfo*);
    static DynObject* createDynObject(string name);
};

#endif /* defined(__MermaidSeaKingdom__DynBase__) */
