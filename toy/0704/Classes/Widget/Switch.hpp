//
//  Switch.hpp
//  TOYBM0701
//
//  Created by huangwen on 15/11/4.
//
//

#ifndef Switch_hpp
#define Switch_hpp
#include "cocos2d.h"
#include "UICpp.h"
#include "../Model/CBAppGlobal.h"

USING_NS_CC;

typedef std::function<void(bool)> SwichAction;

class Switch : public cocos2d::Layer
{
public:
    CREATE_FUNC(Switch);
    
    virtual bool init();
    
    void setBgmBackground(std::vector<ImageView*> image);
    void setEffetBackground(std::vector<ImageView*> image);
    void initData();
    void Bgm(bool tag);
    void Effect(bool tag);
    
    void setSwitchAction(const SwichAction& action);
    
protected:
    void bgmAction();
    void effectAction();
protected:
    std::vector<ImageView *> b_image;
    std::vector<ImageView *> e_image;
    
    int  _isOn;
    SwichAction _switchAction;
    int     _bgmValue = 0;
    int     _effValue = 0;
};

#endif /* Switch_hpp */
