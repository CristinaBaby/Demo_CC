//
//  DeviceInfo.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang on 16/4/7.
//
//

#ifndef DeviceInfo_hpp
#define DeviceInfo_hpp

#include <iostream>
struct MachineInfo {
    std::string key;
    float diagonal;
    std::string name;
};

class DeviceInfo {
public:
    static float getDiagonal();
    static MachineInfo getMachineInfo();
protected:
    static MachineInfo _machineInfo;
};

#endif /* DeviceInfo_hpp */
