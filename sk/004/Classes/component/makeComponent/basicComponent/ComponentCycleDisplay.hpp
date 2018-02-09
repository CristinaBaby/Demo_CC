//
//  ComponentCycleDisplay.hpp
//  BHUG1062
//
//  Created by liji on 16/4/19.
//
//

#ifndef ComponentCycleDisplay_hpp
#define ComponentCycleDisplay_hpp

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

#define kToken "tokenSprite"

typedef enum
{
    kDirectionLeftToRight,
    kDirectionRightToLeft,
    kDirectionBottomToUp,
    kDirectionUpToBottom,
    
}
kMoveDirection;


class ComponentCycleDisplay: public ComponentBase
{
public:
    /**
     *  创建由两张图拼接的循环滚动的图层
     */
    CREATE_FUNC(ComponentCycleDisplay);
    
    void start() override;
    void onAdd() override;
    void stop() override;
    void pause() override;
    void resume() override;

    void setName(const std::string& name);
    void setDirection(kMoveDirection direction);
    
private:
    Sprite *_tokenSprite = nullptr;
    Sprite *_orSprite = nullptr;
    Vec2 _orPos;
    Size _size;
    bool _isFirst = true;

    kMoveDirection _direction = kDirectionLeftToRight;
    
};




#endif /* ComponentCycleDisplay_hpp */
