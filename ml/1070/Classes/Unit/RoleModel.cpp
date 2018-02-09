
#include "RoleModel.h"
#include "AudioHelp.h"
#include "AnimationHelp.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
RoleModel::RoleModel()
{
    m_pRole = nullptr;
    onArmationCallback = nullptr;
}

RoleModel::~RoleModel()
{
    
}
bool RoleModel::init()
{
    if ( !Node::init() )
    {
        return false;
    }    
    
    return true;
}


void RoleModel::setRole(std::string path,std::string name,int count)
{
    if(m_pRole){
        m_pRole->removeFromParent();
    }
    for (int i = 0; i<count; i++) {
        std::stringstream ostr;
        ostr<<name<<i;
        ArmatureDataManager::getInstance()->addArmatureFileInfo(path+ostr.str()+".png", path+ostr.str()+".plist", path+name+".ExportJson");
    }
    m_pRole = Armature::create(name);
    this->addChild(m_pRole);
    ArmatureAnimation* lArmation = m_pRole->getAnimation();
    lArmation->setMovementEventCallFunc([=](Armature *arm, MovementEventType type, const std::string &id){
        if (onArmationCallback) {
            onArmationCallback(arm,type,id);
        }
    });
}

void RoleModel::playArmation(std::string name,bool loop)
{
    ArmatureAnimation* lArmation = m_pRole->getAnimation();
    lArmation->stop();
    lArmation->play(name,-1,loop);
    
}

void RoleModel::playArmation(int index,bool loop){
    
    ArmatureAnimation* lArmation = m_pRole->getAnimation();
    lArmation->playWithIndex(index,-1,loop);
}