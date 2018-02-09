//
//  AlumnAdapter.h
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#ifndef __Doctor2__AlumnAdapter__
#define __Doctor2__AlumnAdapter__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class AlumnAdapter 
{
private:
    AlumnAdapter(){chooseCallback = nullptr;};
public:
    static    AlumnAdapter* getInstance(){
        static AlumnAdapter instance;
        return &instance;
    };
public:
    //保存图片到相册
     void saveToAlumn(Image* apCCImg);
     void openAlumn();
     std::function<void(Texture2D*)> chooseCallback;
};

#endif /* defined(__Doctor2__AlumnAdapter__) */
