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

#define ICE_BOX                 "res/rainbow_slushy/ice/ice_box.png"
#define ICE_BOX_ON              "res/rainbow_slushy/ice/on.png"
#define ICE_BOX_OFF             "res/rainbow_slushy/ice/off.png"
#define ICE_BOX_PROGRESS_BAR    "ui/other/plan.png"
#define ICE_BOX_PROGRESS_BG     "ui/other/plan_bg.png"
#define ICE_BOX_PROGRESS_OK     "ui/other/OK.png"
#define ICE_BOX_DOOR            "res/rainbow_slushy/ice/door.png"
#define ICE_BOX_DOORBAD         "res/rainbow_slushy/ice/door.png"
#define ICE_BOX_CONTENT_UP      "res/2ice/ice/ice_broken.png"
#define ICE_BOX_CONTENT_DOWN    "res/2ice/ice/ice_broken.png"

class IceBoxNode : public MakeBaseNode
{
public:
    CREATE_FUNC(IceBoxNode);
    virtual bool init() override;
    
    CC_SYNTHESIZE(float , _froozenDuration, FroozenDuration);
    CC_SYNTHESIZE(Vec2 , _frozenDoneZone, FrozenDoneZone);
    
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
    
    void showProgressTimer();
    void autoFreeze();

public:
    IceBoxNode():
    _froozenDuration(20),
    _frozenDoneZone(Vec2(10,16)),
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
    _onOffButton(nullptr),
    _isClicked(false),
    _showOk(true),
    _progressBg(nullptr)
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
    Sprite *_progressBg;
    
    Vec2 _originalPosition;
    
    bool _isOn;
    bool _isFinishFroozen;

    float _froozenTime;
    bool _isAnimation;
    bool _isClicked;
    
    bool _showOk;

};

#endif /* IceBoxNode_h */
