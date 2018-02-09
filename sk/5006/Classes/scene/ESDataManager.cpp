//
//  ESDataManager.cpp
//  ccats1061
//
//  Created by liji on 16/8/3.
//
//

#include "ESDataManager.h"
#include "SSCCSVParse.h"
#include "SSCIAPManager.h"
#define FirstAnimation "FirstAnimation"

ESDataManager::ESDataManager():
_adsCount(0),
_isFirstAnimation(true),
_nextStep(0),
_showRate(true)
{
    
    auto parser2 = SSCCSVParse::create("config/FruitCutConfig.csv");
    
    this->initCutConfig(parser2, 1);

    _fruitNames =
    {
        "apple",
        "avocado",
        "banana",
        "blueberry",
        "grape",
        "green_apple",
        "kiwi",
        "lemon",
        "lime",
        "orange",
        "peach",
        "pineapple",
        "raspberry",
        "strawberry",
        "watermelon"
    };

    _sodaFood.flavorIndexVec =
    {
        1,2,3,4,5,8
    };
    
    
}

bool ESDataManager::showAds()
{
//    _adsCount++;
//    if (_adsCount%2 == 0)
//    {
//        return true;
//    }
//    return false;
    

    return SSCIAPManager::getInstance()->isShowAds();

}


void ESDataManager::initCutConfig(SSCCSVParse* parser,int index)
{
    std::vector<CutFood*> s_cutVec;
    
    
    for (int index = 1; index < parser->getRows(); ++index)
    {
        auto cFood = new CutFood();
        cFood->index = index;
        
        std::string name = std::string(parser->getData(index, 1));
        cFood->name = name;
        
        float count = stof(std::string(parser->getData(index, 2)));
        cFood->partsCount = count;
        do {
            
            int x1 = stoi(std::string(parser->getData(index, 3)));
            int y1 = stoi(std::string(parser->getData(index, 4)));
            cFood->cutterOffsetVector.push_back(Vec2(x1, y1));
            
            int x2 = stoi(std::string(parser->getData(index, 5)));
            int y2 = stoi(std::string(parser->getData(index, 6)));
            if (x2 == 0 && y2 == 0)
                break;
            cFood->cutterOffsetVector.push_back(Vec2(x2, y2));
            
            int x3 = stoi(std::string(parser->getData(index, 7)));
            int y3 = stoi(std::string(parser->getData(index, 8)));
            if (x3 == 0 && y3 == 0)
                break;
            cFood->cutterOffsetVector.push_back(Vec2(x3, y3));
            
            int x4 = stoi(std::string(parser->getData(index, 9)));
            int y4 = stoi(std::string(parser->getData(index, 10)));
            if (x4 == 0 && y4 == 0)
                break;
            cFood->cutterOffsetVector.push_back(Vec2(x4, y4));
            
        } while (0);
        
        s_cutVec.push_back(cFood);
    }
    
    if (index == 1)
    {
        _cutFruitVector = s_cutVec;
    }
    
}

ESDataManager::CutFood* ESDataManager::getCutFoodByName(const string &pName)
{
    for (auto cFood : _cutFruitVector)
    {
        if (cFood->name == pName)
        {
            return cFood;
        }
    }
    return _cutFruitVector.front();
}

ESDataManager::CutFood* ESDataManager::getCutFoodByIndex(int index)
{

    return _cutFruitVector.at(index);
}


void ESDataManager::removePaint()
{
    string path;
    path = FileUtils::getInstance()->getWritablePath() + "share_juice.png";
    
    if (FileUtils::getInstance()->isFileExist(path))
    {
        remove(path.c_str());
    }

}

vector<string> ESDataManager::getJuiceCrashedPath()
{
    vector<string> tempVec;
    
    for (int i = 0; i<_sodaFood.flavorInBlenderVec.size(); i++)
    {
        string s2 = StringUtils::format("res/3mix/1/crashedPiece/crashed_%s.png",_fruitNames.at(i).c_str());
        tempVec.push_back(s2);
    }
    
    string s2 = "res/3mix/1/crashedPiece/crashed_ice.png";
    tempVec.push_back(s2);

    return tempVec;
}


string ESDataManager::getFruitFlavorNameByIndex(int flavorIndex)
{
    return _fruitNames.at(flavorIndex);
}

string ESDataManager::getCurrentFruitFlavorName()
{
    return _fruitNames.at(_sodaFood.flavorIndexVec.back());
}

int ESDataManager::getFruitCount()
{
    return _fruitNames.size();
}

void ESDataManager::setCoinNumber(int number)
{
    UserDefault::getInstance()->setIntegerForKey("coinNumber", number);
}

int ESDataManager::getCoinNumber()
{
    return UserDefault::getInstance()->getIntegerForKey("coinNumber");
}


void ESDataManager::resetSodaShare()
{
    removePaint();
    _sodaFood.strawIndex = -1;
}
void ESDataManager::resetPopcornShare()
{
}
void ESDataManager::resetPizzaShare()
{
    _texasPizzaFood.boxIndex = 1;
    _texasPizzaFood.bowIndex = -1;
    _texasPizzaFood.stickerIndex = -1;
    _texasPizzaFood.bowPos = Vec2::ZERO;
    _texasPizzaFood.stickerPos = Vec2::ZERO;
}


string ESDataManager::getJuicePath()
{
//    int n = _sodaFood.flavorInBlenderVec.size();
//    int rand = random(0, n-1);
//    int index = _sodaFood.flavorInBlenderVec.at(rand);
//    _sodaFood.flavorIndex = index;

//    return StringUtils::format("res/3mix/1/inBlend/blender_%s.png",getFruitFlavorNameByIndex(index).c_str());
    return StringUtils::format("res/rainbow_slushy/blender/blender1/mixture%d.png",_sodaFood.flavorIndex);
}

vector<string> ESDataManager::getSprinklePath(int index)
{
    vector<string> temp;
    for (int i = 1; i<4;i++)
    {
        string path = StringUtils::format("res/cake_pop/decoration/sprinkles/sprinkles%d_%d.png",index,i);
//        auto path = StringUtils::format("res/cake_pop/decoration/sprinkles/sprinkles%d.png",index);
        if (FileUtils::getInstance()->isFileExist(path))
        {
            temp.push_back(path);
        }
        
    }
    
    return temp;
}

bool ESDataManager::getIsFirstAnimation()
{
    bool show = UserDefault::getInstance()->getBoolForKey(FirstAnimation, true);
    if (show)
    {
        UserDefault::getInstance()->setBoolForKey(FirstAnimation, false);
        return true;
    }
    return show;
}

void ESDataManager::setSodaStepUnlocked(int stepIndex)
{
//    if (stepIndex>=3)
//        stepIndex = 3;
    if (stepIndex<=0)
    {
        stepIndex = 0;
    }
    
    string key = StringUtils::format("step%d",stepIndex);
    
    UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
}

bool ESDataManager::getSodaStepUnlocked(int stepIndex)
{
//    if (stepIndex>=3)
//        stepIndex = 3;

    string key = StringUtils::format("step%d",stepIndex);
    if (stepIndex <= 0)
    {
        stepIndex = 0;
        return UserDefault::getInstance()->getBoolForKey(key.c_str(),true);
    }
    return UserDefault::getInstance()->getBoolForKey(key.c_str(),false);
}

void ESDataManager::initRainbowCupcakeColor()
{
    if (_rainbowcupcakeFood.colorIndexVec.size()>0)
    {
        return;
    }
    
    for (int i = 0 ; i<3; i++)
    {
        _rainbowcupcakeFood.colorIndexVec.push_back(random(1, 12));
    }
}

