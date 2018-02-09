//
//  MakeSignatureReader.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/7.
//
//

#ifndef MakeSignatureReader_hpp
#define MakeSignatureReader_hpp

#include "cocos2d.h"
#include "SingleInstance.h"
USING_NS_CC;
class MakeSignatureReader:public SingleInstance<MakeSignatureReader>{
public:
     MakeSignatureReader();
     Color3B getPenColor(int index);
     Color3B getBgColor(int index);
private:
    ValueMap _cache;
    
};
#endif /* MakeSignatureReader_hpp */
