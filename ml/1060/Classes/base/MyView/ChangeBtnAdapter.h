//
//  ChangeBtnAdapter.h
//  MyCppGame
//
//  Created by luotianqiang1 on 11/19/14.
//
//

#ifndef __MyCppGame__ChangeBtnAdapter__
#define __MyCppGame__ChangeBtnAdapter__
#include <iostream>
#include "MyBaseAdapter.h"

NS_KD_BEGIN
#define CREATE_ADAPTER2(__TYPE__) \
	static __TYPE__* create(std::vector<std::string>file_path,std::vector<std::string>clickPath) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->file_path = file_path;\
pRet->clickPath = clickPath;\
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


class ChangeBtnAdapter : public MyBaseAdapter{
public:
    ChangeBtnAdapter();
 
    CREATE_ADAPTER2(ChangeBtnAdapter);
    virtual EventSprite * getItem(int position);
    virtual void setItemClick(std::function<void(EventSprite*,int,MyScollView*)> click);
    virtual void click();
    virtual void setUpItemClick(std::function<void(EventSprite*,int,MyScollView*)> upClick);
protected:
    std::vector<std::string> clickPath;
    int currenIndex;
    
    void chaneBtnClick (EventSprite*,int,MyScollView*);
     std::function<void(EventSprite*,int,MyScollView*)> changeBtnitemClick,upClick;
	  CREATE_ADAPTER(ChangeBtnAdapter);
};
NS_KD_END
#endif /* defined(__MyCppGame__ChangeBtnAdapter__) */
