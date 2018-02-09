//
//  TimerManager.hpp
//  TOYBM0701
//
//  Created by huangwen on 15/11/24.
//
//

#ifndef TimerManager_hpp
#define TimerManager_hpp

#include "cocos2d.h"
#define xTimer     TimerManager::getTimer()
USING_NS_CC;

enum PACK_TYPE{
    PICTURE_PACK = 111,
    PENS_PACK
};

class TimerManager{
public:
    static TimerManager* getTimer();
    
    void setPicturePackTime(std::string time, int index);
    
    void setPensPackTime(std::string time, int index);
    
    void setPackTime(PACK_TYPE type, std::string time, int index);
    
    std::string getPicturePackTime(int index);
    
    std::string getPensPackTime(int index);
    
    bool isLocked(PACK_TYPE type, int index);
    
    std::string getCurrTime();
    
    bool offsetTime(std::string time);
    
private:
    static TimerManager* _timer;
    
    UserDefault* _user = UserDefault::getInstance();
};
#endif /* TimerManager_hpp */
