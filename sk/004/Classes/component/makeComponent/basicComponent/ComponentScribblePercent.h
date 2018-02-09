//
//  ComponentScribblePercent.h
//  BHUG1062
//
//  Created by tanshoumei on 4/19/16.
//
//

#ifndef ComponentScribblePercent_hpp
#define ComponentScribblePercent_hpp

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "3rdparty_depends/scribble/ScribbleNode.h"

/// 检测scribble 完成度
class ComponentScribblePercent: public ComponentBase
{
public:
    CREATE_FUNC(ComponentScribblePercent);
    
    ComponentScribblePercent();
    
    ~ComponentScribblePercent();
    
    void start() override;
    
    virtual void pause() override;
    
    virtual void resume() override;
    
    void stop() override;
    
    //[0-1.0]
    std::function<void(float percenet)> scribbleCompletePercentCall = nullptr;
    
    /**
     *  true: 擦掉；false: 刷出来. Default: true
     */
    CC_SYNTHESIZE(bool, _isScribbleBrushTypeEraser, IsScribbleBrushTypeEraser);
    
    /**
     *  涂抹对象图片路径
     */
    CC_SYNTHESIZE(std::string, _scribbleTargetPath, ScribbleTargetPath);
    
    /**
     *  check schedule duration
     */
    CC_SYNTHESIZE(float, _checkDuration, CheckDuration);
    
    /**
     *  完成度0-1.0计
     */
    CC_SYNTHESIZE_READONLY(float, _percentComplete, PercentComplete);
private:
    void checkSchedule(float dt);
    
private:
    ScribbleNode* _scribbleNode = nullptr;
    
    long _originNonAlphaCount = 0;
    
    long _targetNonAlphaCount = 0;
};

#endif /* ComponentScribblePercent_hpp */
