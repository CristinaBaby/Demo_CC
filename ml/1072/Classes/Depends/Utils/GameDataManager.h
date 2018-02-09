
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

class IngredientData {
    
public:
    int index; //序列号
    int type; //类别
    float posX;
    float posY;
    float rotation;
    
    void init(){
        index = 0;
        posX = 0;
        posY = 0;
        rotation = 0;
    }
    void display(){
        log("type: %d",type);
        log("index: %d",index);
        log("posX: %f",posX);
        log("posX: %f",posY);
        log("rotation: %f",rotation);
    }
    std::string convertToString(){
        std::stringstream ostr;
        ostr<<type<<","<<index<<","<<posX<<","<<posY<<","<<rotation;
        return ostr.str();
    }
    
    static IngredientData convertToIngredientData(std::string str){
        IngredientData data;
        data.init();
        std::vector<std::string> strVector = StringHelp::split(str, ",");
        if(strVector.size()==5){
            data.type = atoi(strVector.at(0).c_str());
            data.index = atoi(strVector.at(1).c_str());
            data.posX = atof(strVector.at(2).c_str());
            data.posY = atof(strVector.at(3).c_str());
            data.rotation = atof(strVector.at(4).c_str());
        }
        return data;
    }
    
    static IngredientData convertToIngredientData(int type,Node* pNode)
    {
        IngredientData data;
        data.init();
        data.type = type;
        data.posX = pNode->getPositionX();
        data.posY = pNode->getPositionY();
        data.rotation =  pNode->getRotation();
        return data;
    }
};
class FoodData {
    
public:
    int index; //序列号  添加的早晚标记
    int type; //类别 -1是自己做的pizza  0-8是默认做好的pizza
    bool empty; //空的 没有pizza
    bool packed; //是否自己打包的pizza （false是默认的pizza，true是自己做的）
    std::string path;  //图片存储路径名
    float price;  //AI自识别 建议零售价
    float selfPrice; //自定义价格
    
    void init(){
        index = 0;
        empty = true;
        packed = false;
        type = 0;
        path = "";
        price = 0;
        selfPrice = 0;
    }
    void display(){
        log("index: %d",index);
        log("type: %d",type);
        log("empty: %d",empty);
        log("packed: %d",packed);
        log("path: %s",path.c_str());
        log("price: %f",price);
        log("selfPrice: %f",selfPrice);
    }
    std::string convertToString(){
        std::stringstream ostr;
        ostr<<index<<","<<empty<<","<<packed<<","<<path<<","<<price<<","<<selfPrice<<","<<type;
        return ostr.str();
    }
    
    static FoodData convertToFoodData(std::string str){
        FoodData data;
        data.init();
        std::vector<std::string> strVector = StringHelp::split(str, ",");
        if(strVector.size()==7){
            data.index = atoi(strVector.at(0).c_str());
            data.empty = atoi(strVector.at(1).c_str());
            data.packed = atoi(strVector.at(2).c_str());
            data.path = strVector.at(3);
            data.price = atoi(strVector.at(4).c_str());
            data.selfPrice = atoi(strVector.at(5).c_str());
            data.type = atoi(strVector.at(6).c_str());
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
    
    void setVideoUnlocked(std::string type,int index);
    std::string getVideoUnlockedString(std::string type);
    bool getVideoUnlocked(std::string type,int index);
    
    void setShapeLocked(int index,bool locked = true);
    bool getShapeLocked(int index);
    
    void setFirstTimePrice(bool isFirstTime);
    bool getFirstTimePrice();
    
    void setPriceData(int index,FoodData data);
    FoodData getPriceData(int index);
    
    void setPriceDataString(int index,std::string data);
    std::string getPriceDataString(int index);
    
    
//    void setIngredientData(int type,int index,IngredientData data);
//    IngredientData getIngredientData(int type,int index);
//    
//    void setIngredientData(int type,std::string data);
//    std::string getIngredientData(int type);
    
    void setBoxName(std::string data);
    std::string getBoxName();
    
    void setCoint(float count);
    float getCoint();
//    3种食物 每玩一个会cout＋＋ （按顺序来的）
    void setFoodCount(int count);
    int getFoodCount();

    bool m_bColored = false;
//    bool m_bPacked = false;
    bool m_bShowAds = false;
    
//    口味（下标0开始）
    int m_nFlavorIndex = 0;
    int m_nStickIndex = 0;
    int m_nShapeIndex = 0;
    int m_nCupIndex = 0;
    
    int m_nFrostingType = 0;
    int m_nFrostingIndex = 0;
    
    int m_nDecorateStep = 0;
    
    
    int m_nFriesBoxIndex = 7;
    
    int m_nPackage = 3;
    
    int m_nBgIndex = 0;
    int m_nIcecreamIndex = 0;
    
    bool unlockNewFood = false;
    bool homeToPackage = false;
    
    std::vector<IngredientData> m_VectorIngredient;
    std::vector<Vec3> m_VectorCandle;
    
    bool m_bFriesKetchup[15];
    int m_nNachosCount;
protected:
    
};

#endif