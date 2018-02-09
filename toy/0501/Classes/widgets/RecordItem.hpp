//
//  RecordItem.hpp
//  BedtimeStory
//
//  Created by maxiang on 10/22/15.
//
//

#ifndef RecordItem_hpp
#define RecordItem_hpp

#include "cocos2d.h"
#include "cocosGUI.h"
#include "MiniGameManager.h"

class RecordItem : public cocos2d::Layer
{
public:
    enum class State
    {
        Pause = 0,
        Play
    };
    
    RecordItem();
    virtual ~RecordItem();
    
    CREATE_FUNC(RecordItem);
    CC_SYNTHESIZE(int, _index, Index);
    CC_SYNTHESIZE(State, _currentState, CurrentState);
    CC_SYNTHESIZE(cocos2d::Rect, _boudingBox, BoudingBox);

    //CC_SYNTHESIZE(bool, _isPlaying, IsPlaying);
    CC_SYNTHESIZE(bool, _isSelected, IsSelected);
    CC_SYNTHESIZE(RecordInfo *, _recordInfo, RecordInfo);

    bool init() override;
    
    void setPlayCallback(const std::function<void(RecordItem *)>& callback);
    void setPauseCallback(const std::function<void(RecordItem *)>& callback);

    void setBottomWhiteLineVisible(bool visible);
    
    void setTitle(const std::string& title);
    void setDate(const std::string& date);
    void setTime(const std::string& time);
    
    void setComponentOffset(const cocos2d::Vec2& offset, bool withWhiteLine, bool animation = true);
    void setComponentToOriginal();
    void setColorLayerVisible(bool visible, bool animation);
    
    void backToOriginalPosition(bool animation);
    
    void stopPlay();
    void startPlay();
    
    void changeToSeletedStyle();
    void changeToUnselectedStyle();

protected:

    
protected:
    cocos2d::Label *_titleLabel;
    cocos2d::Label *_dateLabel;
    cocos2d::Label *_timeLabel;
    cocos2d::ui::Button *_playButton;
    cocos2d::LayerColor *_bottomWhiteLine;
    cocos2d::LayerColor *_colorLayer;

    std::function<void(RecordItem *)> _playCallback;
    std::function<void(RecordItem *)> _pauseCallback;
    std::function<void(bool)> _setComponentToOriginal;
};


#endif /* RecordItem_hpp */
