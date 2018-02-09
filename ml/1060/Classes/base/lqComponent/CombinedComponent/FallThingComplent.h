//
//  FallThingComplent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#ifndef FallThingComplent_hpp
#define FallThingComplent_hpp
#include "TouchMoveComponent.h"
#include "TouchAccleRoteComponent.h"

/**用于倾倒材料的compnent
 1.先将材料拖到指定区域
 2.点击或者重力感应倾倒
 */
class FallThingComplent: public TouchMoveComponent{
    DECLARE_COMPONENT_DYNCLASS(FallThingComplent);
    /**倾倒时的位置*/
    CC_SYNTHESIZE(Vec2, _moveEndPos, MoveEndPos);
    /**倾倒时的角度*/
    CC_SYNTHESIZE(float,_fallingRote,FallingRote);
    /**移动到指定位置是的图片*/
    CC_SYNTHESIZE(string,_moveEndPath,moveEndPath);
    /**倾倒时的图片*/
    CC_SYNTHESIZE(string,_fallingPath,FallingPath);
public:
    void dispatcherEvent(string _eventName);
    FallThingComplent();
    virtual ~FallThingComplent();
protected:
    virtual bool init();
    virtual void accleCallBack(Node*,Component*,OperateListner*);
};

#endif /* FallThingComplent_hpp */
