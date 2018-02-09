//
//  MyBaseAdapter.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/9/14.
//
//

#ifndef __MyCppGame__MyBaseAdapter__
#define __MyCppGame__MyBaseAdapter__

#define CREATE_ADAPTER(__TYPE__) \
static __TYPE__* create(const std::vector<std::string>& file_path) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->file_path = file_path;\
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

#include <iostream>
#include"EventSprite.h"
#include "cocos2d.h"
USING_NS_CC;
NS_KD_BEGIN
class MyScollView;
typedef std::function<void(EventSprite*,size_t,MyScollView*)> ItemClick;
class MyBaseAdapter :public Ref{
public:
    MyBaseAdapter();
    CREATE_ADAPTER(MyBaseAdapter);
    virtual EventSprite * getItem(size_t position);
   virtual size_t getCount();
    std::vector<std::string> getFilePath();
    virtual void setItemClick(ItemClick click);
    virtual ItemClick  getItemClick();
    virtual void setMyScollView(MyScollView* scol);
protected:
    std::vector<std::string>file_path;
    ItemClick itemClick;
    MyScollView* scol;
};
NS_KD_END
#endif /* defined(__MyCppGame__MyBaseAdapter__) */
