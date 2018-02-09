//
//  DeviceInfo.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang on 16/4/7.
//
//

#include "DeviceInfo.h"
#include <sys/utsname.h>
#include "cocos2d.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#define CLASS_NAME "com/make/base/DeviceInfo"
#endif


MachineInfo DeviceInfo::_machineInfo = {"",0,""};


float DeviceInfo::getDiagonal(){
    return getMachineInfo().diagonal;
}

MachineInfo DeviceInfo::getMachineInfo(){
    if(_machineInfo.key.empty()) {
        
        MachineInfo form[]= {
            {"i386"   ,4.5 ,"Simulator"},
            {"x86_64" ,4.5 ,"Simulator"},
            
            {"iPod1,1" ,3.5 ,"ipod touch"   },
            {"iPod2,1" ,3.5 ,"ipod touch 2G"},
            {"iPod3,1" ,3.5 ,"ipod touch 3G"},
            {"iPod4,1" ,3.5 ,"ipod touch 4G"},
            {"iPod5,1" ,4.0 ,"ipod touch 5G"},
            {"iPod7,1" ,4.0 ,"ipod touch 6G"},
            
            {"iPhone1,1" ,3.5 ,"iPhone"        },
            {"iPhone1,2" ,3.5 ,"iPhone 3G"     },
            {"iPhone2,1" ,3.5 ,"iPhone 3GS"    },
            {"iPhone3,1" ,3.5 ,"iPhone4"       },
            {"iPhone3,2" ,3.5 ,"iPhone4"       },
            {"iPhone3,3" ,3.5 ,"iPhone4"       },
            {"iPhone4,1" ,3.5 ,"iPhone 4S"     },
            {"iPhone5,1" ,4.0 ,"iPhone 5"      },
            {"iPhone5,2" ,4.0 ,"iPhone 5"      },
            {"iPhone5,3" ,4.0 ,"iPhone 5c"     },
            {"iPhone5,4" ,4.0 ,"iPhone 5c"     },
            {"iPhone6,1" ,4.0 ,"iPhone 5s"     },
            {"iPhone6,2" ,4.0 ,"iPhone 5s"     },
            {"iPhone7,2" ,4.7 ,"iPhone 6"      },
            {"iPhone7,1" ,5.5 ,"iPhone 6 Plus" },
            {"iPhone8,1" ,4.7 ,"iPhone 6s"     },
            {"iPhone8,2" ,5.5 ,"iPhone 6s Plus"},
            {"iPhone8,4" ,4.0 ,"iPhone SE"     },
            
            {"iPad1,1" ,9.70 ,"iPad"                },
            {"iPad2,1" ,9.70 ,"iPad 2"              },
            {"iPad2,2" ,9.70 ,"iPad 2"              },
            {"iPad2,3" ,9.70 ,"iPad 2"              },
            {"iPad2,4" ,9.70 ,"iPad 2"              },
            {"iPad3,1" ,9.70 ,"iPad 3"              },
            {"iPad3,2" ,9.70 ,"iPad 3"              },
            {"iPad3,3" ,9.70 ,"iPad 3"              },
            {"iPad3,4" ,9.70 ,"iPad 4"              },
            {"iPad3,5" ,9.70 ,"iPad 4"              },
            {"iPad3,6" ,9.70 ,"iPad 4"              },
            {"iPad4,1" ,9.70 ,"iPad Air"            },
            {"iPad4,2" ,9.70 ,"iPad Air"            },
            {"iPad4,3" ,9.70 ,"iPad Air"            },
            {"iPad5,3" ,9.70 ,"iPad Air 2"          },
            {"iPad5,4" ,9.70 ,"iPad Air 2"          },
            {"iPad6,7" ,12.9 ,"iPad Pro (12.9 inch)"},
            {"iPad6,8" ,12.9 ,"iPad Pro (12.9 inch)"},
            {"iPad6,3" ,9.70 ,"iPad Pro (9.7 inch)" },
            {"iPad6,4" ,9.70 ,"iPad Pro (9.7 inch)" },
            
            {"iPad2,5" ,7.90 ,"iPad mini"  },
            {"iPad2,6" ,7.90 ,"iPad mini"  },
            {"iPad2,7" ,7.90 ,"iPad mini"  },
            {"iPad4,4" ,7.90 ,"iPad mini 2"},
            {"iPad4,5" ,7.90 ,"iPad mini 2"},
            {"iPad4,6" ,7.90 ,"iPad mini 2"},
            {"iPad4,7" ,7.90 ,"iPad mini 3"},
            {"iPad4,8" ,7.90 ,"iPad mini 3"},
            {"iPad5,1" ,7.90 ,"iPad mini 3"},
            {"iPad5,2" ,7.90 ,"iPad mini 4"},
        };

        
        struct utsname systemInfo;
        uname(&systemInfo);
        std::string s = systemInfo.machine;
        bool isFound = false;
        for(int i=0;i<sizeof(form)/sizeof(*form);i++)
            if(form[i].key.compare(s) == 0){
                _machineInfo = form[i];
                isFound = true;
                break;
            }
        if(!isFound){
            _machineInfo.key = s;
            _machineInfo.diagonal = 4.5;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getDiagonal", "()F"))
        {
            jfloat f = t.env->CallStaticFloatMethod(t.classID, t.methodID);
            _machineInfo.diagonal = (float)f;
        }
#endif
    }
    return _machineInfo;
}