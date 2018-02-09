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
        kFoodCakepop,
        kFoodCupcake,
        kFoodSoda,
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
        int flavorIndex = 1;
        int shapeIndex = 3;
        int stickIndex = 1;
        bool hasOtherFood = false;
        bool isServe = true;
    }CottonCandy;
    
    typedef struct _soda
    {
        int cupIndex = 12;
        int creamIndex = -1;
        int lidIndex = 1;
        vector<int> flavorIndexVec = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        vector<int> flavorInBlenderVec = {1};
        int flavorIndex = 1;
        int iceIndex = 3; 
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

    typedef struct _pizzacone
    {
        int sauceIndex = -1;
        int cheeseIndex = 1;
        bool isServe = true;
    }Pizzacone;

    typedef struct _corndog
    {
        int stickIndex = 1;
        int sauceIndex = -1;
        int dishIndex = -1;
        bool isServe = true;
    }Corndog;
    
    typedef struct _rainbowcakepop
    {
        vector<int> colorIndexVec = {2,10,11,12};
        int stickIndex = 1;
        int chocolateIndex = 1;
        bool isServe = true;
    }Rainbowcakepop;

    typedef struct _rainbowcupcake
    {
        vector<int> colorIndexVec;
        int cupIndex = 1;
        int toppingIndex = 1;
        bool isServe = true;
    }Rainbowcupcake;

    ESDataManager();
    ~ESDataManager(){};
    
    ESDataManager::CutFood* getCutFoodByName(const string &pName);
    ESDataManager::CutFood* getCutFoodByIndex(int index);
    
    void removePaint();
    
    //cottoncandy
    string getFruitFlavorNameByIndex(int flavorIndex);
    string getCurrentFruitFlavorName();
    int getFruitCount();
    vector<string> getJuiceCrashedPath();
    string getJuicePath();
    
    
    vector<string> getSprinklePath(int index);
    void setCoinNumber(int number);
    int getCoinNumber();
    
    
    void resetSodaShare();
    void resetPopcornShare();
    void resetPizzaShare();
    
    bool showAds();
    bool getIsFirstAnimation();
    
    void setSodaStepUnlocked(int stepIndex);
    bool getSodaStepUnlocked(int stepIndex);
    
    void setNextStep(int index)
    {
        _nextStep = index;
        setSodaStepUnlocked(_nextStep);
    }
    
    int getNextStep()
    {
        return _nextStep;
    }
    
    void initRainbowCupcakeColor();

public:
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<CutFood*>, _cutFruitVector, CutFruitVector);
    CC_SYNTHESIZE(CottonCandy, _cottonCandyFood, CottonCandyFood);
    CC_SYNTHESIZE(Soda, _sodaFood, SodaFood);
    CC_SYNTHESIZE(Popcorn, _popcornFood, PopcornFood);
    CC_SYNTHESIZE(TexasPizza, _texasPizzaFood, TexasPizzaFood);
    CC_SYNTHESIZE(Pizzacone, _pizzaconeFood, PizzaconeFood);
    CC_SYNTHESIZE(Corndog, _corndogFood, CorndogFood);
    CC_SYNTHESIZE(Rainbowcakepop, _rainbowcakepopFood, RainbowcakepopFood);
    CC_SYNTHESIZE(Rainbowcupcake, _rainbowcupcakeFood, RainbowcupcakeFood);
    CC_SYNTHESIZE(FoodType,_currentFood, CurrentFood);
    CC_SYNTHESIZE(int, _adsCount, AdsCount);
    CC_SYNTHESIZE(bool, _showRate, ShowRate);
private:
    void initCutConfig(SSCCSVParse* parse,int index);
    
private:
    vector<vector<string>> _juiceFruits; //多种水果混合

    vector<string> _fruitNames;

    bool _isFirstAnimation;
    
    int _nextStep;
    
};
#endif /* ESDataManager_h */
