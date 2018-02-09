//
//  ESDataManager.cpp
//  ccats1061
//
//  Created by liji on 16/8/3.
//
//

#include "ESDataManager.h"
#include "SSCCSVParse.h"

ESDataManager::ESDataManager():
_adsCount(0)
{
    _cottonFlavorNames =
    {
        "blue_raspberry",
        "blueberry",
        "cherry",
        "chocolate",
        "grape",
        "honey",
        "lemon",
        "orange",
        "pink_vanilla",
        "rainbow",
        "strawberry",
        "apple",
        "watermelon",
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
    return true;
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

void ESDataManager::removePaint()
{
    string path;
    path = FileUtils::getInstance()->getWritablePath() + "paint_juice.png";
    
    if (FileUtils::getInstance()->isFileExist(path))
    {
        remove(path.c_str());
    }

}

string ESDataManager::getCottonFlavorNameByIndex(int flavorIndex)
{
    return _cottonFlavorNames.at(flavorIndex);
}

string ESDataManager::getCurrentCottonFlavorName()
{
    return _cottonFlavorNames.at(_cottonCandyFood.flavorIndex);
}

int ESDataManager::getCottonFlavorCount()
{
    return _cottonFlavorNames.size();
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
    _sodaFood.creamIndex = -1;
    _sodaFood.lidIndex = -1;
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
