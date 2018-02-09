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
    void backAction(float dt);
    void reload(const int picturePageIndex);
    void touchDown(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void beBack() { _beBack = true; };
    void onDialog(const std::string& name);
    
private:
    int _picturePageIndex;
    int _packIndex;
    bool _hasChoosed;
    cocos2d::ui::ListView* _listView;
    bool _beBack = false;
    bool _block = false;
    cocos2d::ui::Button * _back;
    int _pictureIndex;
};

#endif /* defined(__ColorBook__CBPictureScene__) */
