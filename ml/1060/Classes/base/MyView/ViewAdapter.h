//
//  ViewAdapter.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/9/14.
//
//

#ifndef __MyCppGame__ViewAdapter__
#define __MyCppGame__ViewAdapter__

#define CREATE_ViewAdapter(__TYPE__) \
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
#include "cocos2d.h"
USING_NS_CC;
typedef std::function<void(Node*,size_t)> ClcikFunc;
class ViewAdapter :public Ref{
    CC_SYNTHESIZE(bool, _recycleTexture, RecycleTexture)
public:
    ViewAdapter();
    CREATE_ViewAdapter(ViewAdapter);
    virtual Node * getItem(size_t position);
    virtual size_t getCount();
    const std::vector<std::string>& getFilePath();
    virtual void setItemClick(ClcikFunc click);
    virtual ClcikFunc getItemClick();
    virtual void recycle(Node* cell,size_t index);
    virtual void setItemSize(const Size&);
    virtual const Size& getItemSize(size_t index);
protected:
    std::vector<std::string> file_path;
    ClcikFunc itemClick;
    Size _itemSize;
};
#endif /* defined(__MyCppGame__ViewAdapter__) */
