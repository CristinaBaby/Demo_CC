//
//  QuitGameLayer.h
//  PaintDress
//
//  Created by maxiang on 7/21/15.
//
//

#ifndef __PaintDress__QuitGameLayer__
#define __PaintDress__QuitGameLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class QuitGameLayer : public cocos2d::LayerColor
{
public:
    
    CREATE_FUNC(QuitGameLayer);
    virtual bool init();
    
    void yesAction();
    void noAction();
    
    void setYesActionCallback(const std::function<void()>& callback);
    void setNoActionCallback(const std::function<void()>& callback);
    
protected:
    std::function<void()> _yesCallback;
    std::function<void()> _noCallback;
};


#endif /* defined(__PaintDress__QuitGameLayer__) */
