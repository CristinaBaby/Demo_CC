//
//  IceManualMachineNode.h
//  BHUG1066
//
//  Created by maxiang on 6/24/16.
//
//

#ifndef IceManualMachineNode_h
#define IceManualMachineNode_h

#include "MakeBaseNode.h"

#define MANUAL_MACHINE1     "res/snow_cone/3-6/ice_crusher1_2.png"
#define MANUAL_MACHINE2     "res/snow_cone/3-6/ice_crusher1_3.png"
#define MANUAL_MACHINE3     "res/snow_cone/3-6/ice_crusher1_4.png"
#define MANUAL_MACHINE4     "res/snow_cone/3-6/ice_crusher1_5.png"
#define MANUAL_MACHINE_DOOR "res/snow_cone/3-6/ice_crusher1_1.png"
#define MANUAL_MACHINE_BAR  "res/snow_cone/3-6/ice_crusher1_0.png"
#define MANUAL_MACHINE_ICE  "res/snow_cone/3-6/ice.png"
#define MANUAL_ICE_CONTENT1  "res/snow_cone/3-6/ice_crusher1_6.png"
#define MANUAL_ICE_CONTENT2  "res/snow_cone/3-6/ice_crusher1_7.png"
#define MANUAL_ICE_CONTENT3  "res/snow_cone/3-6/ice_crusher1_8.png"

//手动刨冰机
class IceManualMachineNode : public MakeBaseNode
{
public:
    CREATE_FUNC(IceManualMachineNode);
    virtual bool init() override;
    
    //显示手指点击指示
    void openDoor(const std::function<void()> callback);
    void closeDoor(const std::function<void()> callback);

    //倒一块冰进去
    void pourOneIce(const std::function<void()>& callback);
    
    void makeIce(const std::function<void()>& callback);
    
    //移除下面装冰渣的容器
    void removeBottom();
    
    std::function<void()> machineBarClicked = nullptr;
public:
    IceManualMachineNode():
    _makeProgress(1)
    {};
    virtual ~IceManualMachineNode();
    
protected:
    Sprite *_machineDoor;
    Sprite *_machineBar;
    Vec2 _originalPosition;
    int _iceContentIndex;
    int _makeProgress;
};

#endif /* IceManualMachineNode_h */
