//
//  PlayRecordItemBar.hpp
//  BedtimeStory
//
//  Created by maxiang on 10/22/15.
//
//

#ifndef PlayRecordItemBar_hpp
#define PlayRecordItemBar_hpp

#include "cocos2d.h"
#include "cocosGUI.h"
#include "RecordItem.hpp"
#include "MiniGameManager.h"

class PlayRecordItemBar : public cocos2d::Layer
{
public:
    PlayRecordItemBar();
    virtual ~PlayRecordItemBar();
    
    CREATE_FUNC(PlayRecordItemBar);
    
    bool init() override;
    
    void showToolBarLayer();
    void hideToolBarLayer();
    
    void setDeleteCallback(const std::function<void()>& callback);
    void setRefreshCallback(const std::function<void()>& callback);

    void fadeIn(bool animation);
    void fadeOut(bool animation);
    
    void setCurrentTime(const std::string& time);
    void setEndTime(const std::string& time);
    
    void setSliderPercent(const float percent);
    
    CC_SYNTHESIZE(RecordItem*, _currentRecordItem, CurrentRecordItem);
    CC_SYNTHESIZE(bool, _isShowToolBarLayer, IsShowToolBarLayer);
    CC_SYNTHESIZE(bool, _isAnimation, IsAnimation);
protected:
    cocos2d::LayerColor *_colorLayer;
    cocos2d::LayerColor *_toolBarLayer;

    cocos2d::Label *_currentTimeLabel;
    cocos2d::Label *_endTimeLabel;
    cocos2d::ui::Slider *_timeSlider;
    cocos2d::ui::Button *_refreshButton;
    cocos2d::ui::Button *_deleteButton;
    cocos2d::ui::Button *_showButton;

    std::function<void()> _deleteCallback;
    std::function<void()> _refreshCallback;
};

#endif /* PlayRecordItemBar_hpp */
