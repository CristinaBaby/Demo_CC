//
//  DynBase.cpp
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/18.
//
//

#include "DynBase.h"
#include <map>
static map<string, DynClassInfo*> *dynClassInfoMap = nullptr;

IMPLEMENT_DYNCLASS(DynObject)

bool DynObject::Register(DynClassInfo *_info){
    if(dynClassInfoMap == nullptr){
        dynClassInfoMap = new map<string, DynClassInfo*>();
    }
    if(_info){
        if(dynClassInfoMap->find(_info->getClassName()) == dynClassInfoMap->end())
            dynClassInfoMap->insert(map<string, DynClassInfo*>::value_type(_info->getClassName(),_info));
    }
    return true;
}

DynObject* DynObject::createDynObject(string name){
    map<string, DynClassInfo*>::const_iterator _iter = dynClassInfoMap->find(name);
    if(_iter != dynClassInfoMap->end()){
       return  _iter->second->createDynObject();
    }
    return nullptr;
}

bool Register(DynClassInfo* _info) {
    return DynObject::Register(_info);
}


