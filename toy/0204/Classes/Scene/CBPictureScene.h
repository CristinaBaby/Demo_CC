//
//  CBPictureScene.h
//  ColorBook
//
//  Created by huangwen on 8/13/15.
//
//

#ifndef __ColorBook__CBPictureScene__
#define __ColorBook__CBPictureScene__

#include "cocos2d.h"
#include "cocosgui.h"


class PictureScene : public cocos2d::Layer
{
public:
    //create scene
    static cocos2d::Scene* createSceneWithPackIndex(const int packIndex);
    
    //create func
    CREATE_FUNC(PictureScene);
    
    ~PictureScene();
    
    virtual bool init();
    
    void setPackIndex(const int index);
    
    void backAction();
    
    void reload(const int picturePageIndex);
    
    void touchDown(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    
private:
    int _picturePageIndex;
    
    int _packIndex;
    
    bool _hasChoosed;
    
    cocos2d::ui::ListView* _listView;
};

#endif /* defined(__ColorBook__CBPictureScene__) */
