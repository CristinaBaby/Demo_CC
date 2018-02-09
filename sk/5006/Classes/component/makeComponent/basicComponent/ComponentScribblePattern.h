//
//  ComponentScribblePattern.h
//  
//
//  Created by maxiang on 4/20/16.
//
//

#ifndef ComponentScribblePattern_h
#define ComponentScribblePattern_h

#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "3rdparty_depends/scribble/ScribblePatternNode.h"
#include "3rdparty_depends/scribble/ScribbleUtil.h"

class ComponentScribblePattern : public ComponentBase
{
public:
    static ComponentScribblePattern *create(const std::string& maskFile);
    
    void setPattern(Sprite *pattern);
    
    void setMask(const std::string& file);
    
    /* @brief 设置笔刷
     */
    void setBrush(const std::string& brushFile);
    
    /* @brief 设置随手指移动的涂抹工具
     */
    void setScribbleTool(const std::string& toolFile,
                         const Vec2& position,
                         Node *parent);
    void showScribbleTool(const Vec2& toPosition);

    /* @brief 所有设置完成后调用
     */
    virtual void start() override;
    
    /* @brief 停止Component
     */
    virtual void stop() override;
    
    /* @brief 获得scribble node
     */
    ScribbleNode *getScribbleNode() {return _scribblePattern;};
    
    /* @brief 获取涂色后的sprite
     */
    Sprite *getSprite();
    
    std::function<void(Touch* touch)> touchBeganCall = nullptr;
    std::function<void(Touch* touch)> touchMovedCall = nullptr;
    std::function<void(Touch* touch)> touchEndedCall = nullptr;
    std::function<void(Touch* touch)> touchCancelledCall = nullptr;
    
public:
    bool init(const std::string& maskFile);
    virtual void onAdd() override;
    ComponentScribblePattern();
    virtual ~ComponentScribblePattern();
    
protected:
    void addTouchEventListener();
    void removeTouchEventListener();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
protected:
    EventListenerTouchOneByOne *_eventListenerTouchOneByOne;
    
    ScribblePatternNode *_scribblePattern;
    
    bool _isTakeColor;
    bool _isColouring;
    
    Image *_patternMaskImage;
    Sprite *_patternMaskSprite;
    
    Sprite *_scribbleToolSprite;

    Sprite *_pattern;
    
    Vec2 _touchInToolLocation;
    
    std::string _brushFile;
};

#endif /* ComponentScribblePattern_h */
