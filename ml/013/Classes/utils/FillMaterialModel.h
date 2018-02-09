//
//  FillMaterialModel.h
//  COOK016_TacoMaker
//
//  Created by jianghai on 15-4-23.
//
//

#ifndef __COOK016_TacoMaker__FillMaterialModel__
#define __COOK016_TacoMaker__FillMaterialModel__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define kShotScreenEvent "ShotScreenEvent"

using namespace std;

typedef enum {
    NONE,
    MOVE,
    SCALE,
    ROTATE,
} EventType;

class FillMaterialModel : public Sprite
{
public:
    FillMaterialModel();
    ~FillMaterialModel();
    static FillMaterialModel* create(const std::string& ImageName, bool small = false);
    
    virtual bool init(const string& ImageName, bool small);
    
    void removeCloseBtn();
    void changeItemTexture(const string& filename);
    void removeTouch();
    void enableTouch();

    CC_SYNTHESIZE_READONLY(string, _resourceName, ResourceName);
    CC_SYNTHESIZE(bool, _enable, Enable);
    CC_SYNTHESIZE(bool, isShotScreen, ShotScreen);
    CC_SYNTHESIZE(bool, hidden, IsHidden);
private:
    void deleteSelf(Ref *sender, Control::EventType type);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    virtual void onEnter();
    virtual void onExit();
    
    virtual void updateTipsFrameStatus(float);
    
    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);
    void onReciveNotify(Ref* pRef);
private:
    ControlButton *_close;
    Sprite *_magnify;
    Sprite *_item;
    EventListenerTouchOneByOne *_listener;
    EventType _eventType;
    float _orignalRotate;
    bool _isTipsFrameShow;
    int ItemSpace = 254;
    string linefile;
    string transportfile;
};

#endif /* defined(__COOK016_TacoMaker__FillMaterialModel__) */
