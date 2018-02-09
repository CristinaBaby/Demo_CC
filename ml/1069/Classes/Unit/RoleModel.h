
#ifndef __RoleModel__
#define __RoleModel__


#include "cocos2d.h"
#include <stdio.h>
#include "ui/CocosGUI.h"
#include "CocoStudioHelper.h"

USING_NS_CC;
using namespace cocos2d::ui;

class RoleModel : public Node
{
public:
    RoleModel();
    ~RoleModel();
    
    CREATE_FUNC(RoleModel);
    
    virtual bool init();
    
    void setRole(std::string Path,std::string name,int count);
    void playArmation(std::string name,bool loop);
    void playArmation(int index,bool loop);
    void gotoAndPause(int index){
        ArmatureAnimation* lArmation = m_pRole->getAnimation();
        lArmation->stop();
        lArmation->gotoAndPause(index);
    }
    Armature* getArmature(){
        return m_pRole;
    }
    std::function<void(Armature *armature, MovementEventType movementType, const std::string& movementID)> onArmationCallback;
protected:
    
    Armature* m_pRole;
};

#endif