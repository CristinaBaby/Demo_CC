//
//  IceAutoMachineNode.h
//  BHUG1066
//
//  Created by maxiang on 6/27/16.
//
//

#ifndef IceAutoMachineNode_h
#define IceAutoMachineNode_h

#include "MakeBaseNode.h"

#define AUTO_MACHINE1     "res/snow_cone/3-6/ice_crusher2_up.png"
#define AUTO_MACHINE2     "res/snow_cone/3-6/ice_crusher2_down.png"
#define AUTO_MACHINE3     "res/snow_cone/3-6/ice_crusher2_box_up.png"
#define AUTO_MACHINE_DOOR "res/snow_cone/3-6/ice_crusher2_door.png"
#define AUTO_MACHINE_ICE  "res/snow_cone/3-6/ice.png"
#define AUTO_ICE_CONTENT1 "res/snow_cone/3-6/ice_crusher2_ice0.png"
#define AUTO_ICE_CONTENT2 "res/snow_cone/3-6/ice_crusher2_ice1.png"
#define AUTO_ICE_CONTENT3 "res/snow_cone/3-6/ice_crusher2_ice2.png"
#define AUTO_ON_BUTTON    "res/snow_cone/3-6/ice_crusher2_on.png"
#define AUTO_OFF_BUTTON   "res/snow_cone/3-6/ice_crusher2_off.png"

//自动刨冰机
class IceAutoMachineNode : public MakeBaseNode
{
public:
    CREATE_FUNC(IceAutoMachineNode);
    virtual bool init() override;

    //显示手指点击指示
    void openDoor(const std::function<void()> callback);
    void closeDoor(const std::function<void()> callback);
    
    //倒一块冰进去
    void pourOneIce(const std::function<void()>& callback);
    
    void makeIce(const std::function<void()>& callback);
    
    //移除下面装冰渣的容器
    void removeBottom();
public:
    IceAutoMachineNode():
    _isOn(false),
    _isEnable(false)
    {};
    virtual ~IceAutoMachineNode();
    
protected:
    void showIceContent();
    
protected:
    std::function<void()> _makeIceCallback;
    Sprite *_machineDoor;
    Vec2 _originalPosition;
    int _iceContentIndex;
    Button *_onOffButton;
    bool _isOn;
    bool _isEnable;
    int _pressBtnIdd;
};

#endif /* IceAutoMachineNode_h */
