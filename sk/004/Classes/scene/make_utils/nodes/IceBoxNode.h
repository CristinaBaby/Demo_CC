//
//  IceBoxNode.h
//  BHUG1066
//
//  Created by maxiang on 6/23/16.
//
//

#ifndef IceBoxNode_h
#define IceBoxNode_h

#include "MakeBaseNode.h"

#define ICE_BOX                 "res/snow_cone/1/ice_box.png"
#define ICE_BOX_ON              "res/snow_cone/1/on.png"
#define ICE_BOX_OFF             "res/snow_cone/1/off.png"
#define ICE_BOX_PROGRESS_BAR    "ui/ui_png/other/progress_bar.png"
#define ICE_BOX_PROGRESS_BG     "ui/ui_png/other/progress_bar_bg.png"
#define ICE_BOX_PROGRESS_OK     "ui/ui_png/other/tips_ok_s.png"
#define ICE_BOX_DOOR            "res/snow_cone/1/door.png"
#define ICE_BOX_DOORBAD         "res/snow_cone/1/fail.png"
#define ICE_BOX_CONTENT_UP      "res/snow_cone/1/ice_up.png"
#define ICE_BOX_CONTENT_DOWN    "res/snow_cone/1/ice_down.png"


class IceBoxNode : public MakeBaseNode
{
public:
    CREATE_FUNC(IceBoxNode);
    virtual bool init() override;
    
    CC_SYNTHESIZE(float , _froozenDuration, FroozenDuration);
    
    //在外部根据想要到达的时间调用
    void froozenGood();
    void froozenBad();

    CC_SYNTHESIZE(bool, _isFroozenOk, IsFroozenOk);

    void setContent(const std::string& file, const Vec2& position = Vec2::ZERO);
    void setContent(Node *content, const Vec2& position = Vec2::ZERO);

    std::function<void(bool isOn)> onOffCallback = nullptr;
    std::function<void(float time)> froozenTimeCallback = nullptr;
    std::function<void()> successFroozenCallback = nullptr;
    std::function<void()> failFroozenCallback = nullptr;

public:
    IceBoxNode():
    _froozenDuration(6.0),
    _iceBox(nullptr),
    _isFroozenOk(false),
    _door(nullptr),
    _content(nullptr),
    _contentDown(nullptr),
    _contentUp(nullptr),
    _progressTimer(nullptr),
    _isOn(false),
    _isFinishFroozen(false),
    _okSprite(nullptr),
    _froozenTime(0.0),
    _doorBad(nullptr),
    _isAnimation(false),
    _onOffButton(nullptr)
    {};
    virtual ~IceBoxNode();
    
protected:
    void onAction();
    void offAction();
    
    void progressUpdateTimer(float time);
    
    void resetAction();
    
protected:
    Sprite *_iceBox;
    Node *_content;
    
    Sprite *_door;
    Sprite *_doorBad;

    Sprite *_okSprite;
    Sprite *_contentUp;
    Sprite *_contentDown;
    
    Button *_onOffButton;

    ProgressTimer *_progressTimer;
    
    Vec2 _originalPosition;
    
    bool _isOn;
    bool _isFinishFroozen;

    float _froozenTime;
    bool _isAnimation;
};

#endif /* IceBoxNode_h */
