
#ifndef __GameDataManager__
#define __GameDataManager__


#include "SingleTon.h"
#include "DressUpData.h"
#include <iostream>
#include <vector>
#include "StringHelp.h"

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

static string gCandleName[] ={
    "fireworks",
    "letter",
    "number",
    "sample"
};
class FoodData {
    
    
public:
    int package; //bbq or skewer
    std::vector<string> dataVector; //<catagory,index>
    
    void display(){
        log("package: %d",package);
        for_each(dataVector.begin(), dataVector.end(), [=](string str){
            std::vector<std::string> vec = StringHelp::split(str, ",");
            int type = stoi(vec.at(0));
            int index = stoi(vec.at(1));
            log("type: %d   index: %d",type,index);
            
        });
    }
};
class GameDataManager : public SingleTon<GameDataManager>
{
public:
    GameDataManager();
    ~GameDataManager();
    
    void setup();
    
//    3种食物 每玩一个会cout＋＋ （按顺序来的）
    void setFoodCount(int count);
    int getFoodCount();

//    口味（下标0开始）
    int m_nFlavorIndex = 0;
    int m_nStickIndex = 0;
    int m_nShapeIndex = 0;
    
    int m_nFrostingType = 0;
    int m_nFrostingIndex = 0;
    
    int m_nDecorateStep = 0;
    
    int m_nPackage = 0;
    
    int m_nBgIndex = 0;
    
    bool unlockNewFood = false;
    bool homeToPackage = false;
    
    std::vector<Vec3> m_VectorCandle;
protected:
    
};

#endif