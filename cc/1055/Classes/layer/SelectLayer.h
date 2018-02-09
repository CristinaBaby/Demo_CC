//
//  SeceltLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/15.
//
//

#ifndef SeceltLayer_hpp
#define SeceltLayer_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class SecletLayer : public BaseLayer  {
public:
    SecletLayer();
    ~SecletLayer();
public:
    bool init();
    CREATE_FUNC(SecletLayer);
    CREATE_SCENE(SecletLayer);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    Vector<Sprite*> m_vFlavor;
    Vector<Sprite*> m_vLock;
};


#endif /* SeceltLayer_hpp */
