//
//  ESDataManager.h
//  ccats1061
//
//  Created by liji on 16/8/3.
//
//

#ifndef ESDataManager_h
#define ESDataManager_h

#include <stdio.h>
#include "cocos2d.h"
#include "SingleTon.h"
#include "DataManager.h"
using namespace cocos2d;
using namespace std;

#define kESDataManager ESDataManager::getInstance()
#define ScreenNameFood "ScreenNameFood.png"
#define ScreenNameCotton "ScreenNameCotton.png"

class SSCCSVParse;
class ESDataManager : public SingleTon<ESDataManager>
{
public:
    enum class FoodType
    {
        kFoodPizza = 1,
        kFoodPopcorn = 2,
        kFoodSoda = 3,
    };
    
    
    typedef struct _cutFood
    {
        int index;
        int partsCount;
        std::string name;
        std::vector<Vec2> cutterOffsetVector;
    }CutFood;

    typedef struct _cottonCandy
    {
        int flavorIndex = 0;
        int shapeIndex = 3;
        int stickIndex = 1;
        bool hasOtherFood = false;
    }CottonCandy;
    
    typedef struct _soda
    {
        int cupIndex = 1;
        int creamIndex = -1;
        int lidIndex = -1;
        int flavorIndex = 2;
        int iceIndex = -1; //-1表示没有
        int strawIndex = -1;
        bool isServe = true;
    }Soda;

    typedef struct _popcorn
    {
        int flavorIndex = 1;
        int boxIndex = 1;
        bool isServe = true;
    }Popcorn;
    
    typedef struct _texasPizza
    {   int foodIndex = 1; //烤好三个中被选择的
        int boxIndex = 1;
        int bowIndex = -1;
        int stickerIndex = -1;
        Vec2 bowPos = Vec2::ZERO;
        Vec2 stickerPos = Vec2::ZERO;
        bool isServe = true;
    }TexasPizza;

    ESDataManager();
    ~ESDataManager(){};
    
    ESDataManager::CutFood* getCutFoodByName(const string &pName);
    
    void removePaint();
    
    //cottoncandy
    string getCottonFlavorNameByIndex(int flavorIndex);
    string getCurrentCottonFlavorName();
    int getCottonFlavorCount();
    void setCoinNumber(int number);
    int getCoinNumber();
    
    
    void resetSodaShare();
    void resetPopcornShare();
    void resetPizzaShare();
    
    bool showAds();
    
public:
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<CutFood*>, _cutFruitVector, CutFruitVector);
    CC_SYNTHESIZE(CottonCandy, _cottonCandyFood, CottonCandyFood);
    CC_SYNTHESIZE(Soda, _sodaFood, SodaFood);
    CC_SYNTHESIZE(Popcorn, _popcornFood, PopcornFood);
    CC_SYNTHESIZE(TexasPizza, _texasPizzaFood, TexasPizzaFood);
    CC_SYNTHESIZE(FoodType,_currentFood, CurrentFood);
    CC_SYNTHESIZE(int, _adsCount, AdsCount);
private:
    void initCutConfig(SSCCSVParse* parse,int index);
    
private:
    vector<vector<string>> _juiceFruits; //多种水果混合
    vector<string> _juiceName;

    vector<string> _cottonFlavorNames;

    
};
#endif /* ESDataManager_h */
