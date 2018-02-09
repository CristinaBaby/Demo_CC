
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
    int index; //序列号
    bool empty; //空的 没有pizza
    bool packed; //是否打包
    std::string path;  //图片存储路径名
    float price;  //AI自识别 建议零售价
    float selfPrice; //自定义价格
    
    void init(){
        index = 0;
        empty = true;
        packed = false;
        path = "";
        price = 0;
        selfPrice = 0;
    }
    void display(){
        log("index: %d",index);
        log("empty: %d",empty);
        log("packed: %d",packed);
        log("path: %s",path.c_str());
        log("price: %f",price);
        log("selfPrice: %f",selfPrice);
    }
    std::string convertToString(){
        std::stringstream ostr;
        ostr<<index<<","<<packed<<","<<path<<","<<price<<","<<selfPrice;
        return ostr.str();
    }
    
    static FoodData convertToFoodData(std::string str){
        FoodData data;
        data.init();
        std::vector<std::string> strVector = StringHelp::split(str, ",");
        if(strVector.size()==5){
            data.index = atoi(strVector.at(0).c_str());
            data.packed = atoi(strVector.at(1).c_str());
            data.path = strVector.at(2);
            data.price = atoi(strVector.at(3).c_str());
            data.selfPrice = atoi(strVector.at(4).c_str());
            data.empty = false;
        }
        return data;
    }
};
class GameDataManager : public SingleTon<GameDataManager>
{
public:
    GameDataManager();
    ~GameDataManager();
    
    void setup();
    void setShapeLocked(int index,bool locked = true);
    bool getShapeLocked(int index);
    
    void setPriceData(int index,FoodData data);
    FoodData getPriceData(int index);
    
    void setPriceDataString(int index,std::string data);
    std::string getPriceDataString(int index);
    
    void setBoxName(std::string data);
    std::string getBoxName();
    
    void setCoint(float count);
    float getCoint();
//    3种食物 每玩一个会cout＋＋ （按顺序来的）
    void setFoodCount(int count);
    int getFoodCount();

    bool m_bColored = false;
    bool m_bPacked = false;
    
//    口味（下标0开始）
    int m_nFlavorIndex = 0;
    int m_nStickIndex = 0;
    int m_nShapeIndex = 0;
    
    int m_nFrostingType = 0;
    int m_nFrostingIndex = 0;
    
    int m_nDecorateStep = 0;
    
    int m_nPackage = 0;
    
    int m_nBgIndex = 0;
    int m_nIcecreamIndex = 0;
    
    bool unlockNewFood = false;
    bool homeToPackage = false;
    
    std::vector<Vec3> m_VectorCandle;
protected:
    
};

#endif