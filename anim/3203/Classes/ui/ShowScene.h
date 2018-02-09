//
// ShowScene.h
// ANIM3203
//
// Created by dengcheng on 15/9/26.
//
//

#ifndef __ANIM3203__ShowScene__
#define __ANIM3203__ShowScene__

#include "cocos2d.h"
#include "BaseScene.h"
#include "AnimalModel.h"
#include "WriteTouchNode.h"

USING_NS_CC;

class ShowScene : public BaseScene
{
public:
    ShowScene();
    ~ShowScene();

    static ShowScene* create();
    virtual bool init();

protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    bool m_bFirstIn;
    
    AnimalModel *m_pModel;
    
    WriteTouchNode *m_pPen;
    
    Sprite *m_pBg;
    
    Sprite *m_pPhotoFrame;
    
    Node *m_pPhotoNode;
    
    Sprite *m_pPhoto;
    
    Image *m_pPhotoImage;
    
    Texture2D *m_pPhotoTexture;
    
    CheckBox *m_pFirstBox;
    
    EventListenerCustom *m_pCustom;
    
    Button *m_pClose;
    
    Button *m_pSave;
    
    bool m_bCamera;
    
    bool m_bButton;
    
private://method
    
    void onButtonCallback(Ref *aButton, Widget::TouchEventType aType);
    
    void onChangePaintType(Ref*, CheckBox::EventType);
    
    void onChangePenColor(Ref*, CheckBox::EventType);
    
    void onCompleteCamera(float aTime);
    
    void onAdsShowDone();
    
};

#endif /* defined(__ANIM3203__ShowScene__) */