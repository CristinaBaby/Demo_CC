//
//  WeddingFoodData.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/1.
//
//

#include "WeddingFoodData.h"
using namespace std;
using namespace cocos2d;

vector<decltype(WeddingFoodData::cookieShape)> WeddingFoodData::cookieShapes = vector<decltype(WeddingFoodData::cookieShape)>(6);
decltype(WeddingFoodData::cookieFlavor) WeddingFoodData::cookieFlavor = 2;
decltype(WeddingFoodData::cookieShape) WeddingFoodData::cookieShape = 0;
decltype(WeddingFoodData::cookieShape) WeddingFoodData::foodIndex = 0;
std::vector<std::string> WeddingFoodData::eatFoodPath = std::vector<std::string>(3);
std::vector<std::string> WeddingFoodData::platesPaths =std::vector<std::string>(3);
std::string WeddingFoodData::forksPath = "";

void WeddingFoodData::setCookieShapes(const decltype(cookieShapes)& temp){
    cookieShapes = temp;
}

 auto WeddingFoodData::getCookieShapes() ->  decltype(cookieShapes){
    return cookieShapes;
}

vector<string> WeddingFoodData::getBakeEndCookie(){
    vector<std::string> temp;
    stringstream sstream;
    for(auto index:cookieShapes){
        sstream.str("");
        sstream.clear();
        sstream<<"png/cookies/6/cookies"<<cookieFlavor<<"/cookies"<<cookieFlavor<<"_"<<index<<".png";
        temp.push_back(sstream.str());
    }
    return temp;
}

void WeddingFoodData::setCookieFlavor(decltype(cookieFlavor) index){
    cookieFlavor = index;
}

auto  WeddingFoodData::getCookieFlavor() ->decltype(cookieFlavor){
    return cookieFlavor;
}

void WeddingFoodData::setShape(decltype(cookieShape) _shape) {
    cookieShape = _shape;
}

auto WeddingFoodData::getShape() ->decltype(cookieShape){
    return cookieShape;
}

string WeddingFoodData::getDecorationFood(){
    stringstream sstream;
    switch (foodIndex) {
        case 1:
            sstream<<"png/cookies/6/cookies"<<cookieFlavor<<"/cookies"<<cookieFlavor<<"_"<<cookieShape<<".png";
            break;
        case 2:{
            auto str =  FileUtils::getInstance()->getWritablePath() + "addFruitFritTart.png";
            Director::getInstance()->getTextureCache()->removeTextureForKey(str);
            if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(str))){
                sstream<<str;
            }else
                sstream<<"png/Fruit Tart/4/send.png";
        }
            break;
        default:
            sstream<<"png/cookies/6/cookies"<<cookieFlavor<<"/cookies"<<cookieFlavor<<"_"<<cookieShape<<".png";
    }
    
    return sstream.str();
}

std::vector<std::string> WeddingFoodData::getMixingCookie(){
    vector<std::string> temp;
    stringstream sstream;
    vector<std::string> flavorNames = {
        "png/cookies/2/cookie0_original"
        ,"png/cookies/2/cookie1_double-chocolate"
        ,"png/cookies/2/cookie2_strawberry"
        ,"png/cookies/2/cookie3_Blueberry"
        ,"png/cookies/2/cookie4_chocolate-chip"
        ,"png/cookies/2/cookie5_Raspberry"
        ,"png/cookies/2/cookie6_mocha"
        ,"png/cookies/2/cookie7_lemon"
    };
    for(int i=0;i<5;++i){
        sstream.str("");
        sstream.clear();
        sstream<<flavorNames[cookieFlavor]<<i<<".png";
        temp.push_back(sstream.str());
    }
    return temp;
}

std::vector<std::string> WeddingFoodData::getRollCookie(){
    vector<std::string> temp;
    stringstream sstream;
    vector<int> indexs = {1,3,7,8};
    for(auto i:indexs){
        sstream.str("");
        sstream.clear();
        sstream<<"png/cookies/3/cookies"<<cookieFlavor<<"_"<<i<<".png";
        temp.push_back(sstream.str());
    }
    return temp;
}

std::string WeddingFoodData::getCookieVanillaBottle(){
    stringstream sstream;
    sstream<<"vanillaBottle/cookie_"<<cookieFlavor<<".csb";
    return sstream.str();
}
std::string WeddingFoodData::getCookieVanilla(){
    stringstream sstream;
    sstream<<"vanillaInbowl/cookie_"<<cookieFlavor<<".csb";
    return sstream.str();
}

void WeddingFoodData::setFoodIndex(decltype(foodIndex) index){
    platesPaths[index] = "";
    forksPath = "";
    foodIndex = index;
}
auto WeddingFoodData::getFoodIndex()->decltype(foodIndex){
    return foodIndex;
}

std::string WeddingFoodData::getEatPath(){
    if(eatFoodPath[foodIndex].empty())
        return "png/cookies/6/cookies0/cookies0_0.png";
    else
        return FileUtils::getInstance()->getWritablePath() + eatFoodPath[foodIndex];
}

std::string WeddingFoodData::getEatLayerPath(){
    stringstream sstream;
    sstream<<"eatFood/food_"<<foodIndex<<".csb";
    return sstream.str();
}

std::string WeddingFoodData::getListViewPath(){
    stringstream sstream;
    sstream<<"listView/food_"<<foodIndex<<".csb";
    return sstream.str();
}

void  WeddingFoodData::saveFood(RenderTexture* render){
    stringstream stringstrem;
    stringstrem<<"eatFood"<<foodIndex<<".png";
    auto str = stringstrem.str();
    std::remove((FileUtils::getInstance()->fullPathForFilename(FileUtils::getInstance()->getWritablePath() + str)).c_str());
    Director::getInstance()->getTextureCache()->removeTextureForKey(FileUtils::getInstance()->getWritablePath() + str);
    Director::getInstance()->getRenderer()->render();
    eatFoodPath[foodIndex] = str;
    render->saveToFile(str);
    Director::getInstance()->getRenderer()->render();
}

void WeddingFoodData::saveAddFruitFritTart(cocos2d::RenderTexture* render){
    render->saveToFile("addFruitFritTart.png");
    Director::getInstance()->getRenderer()->render();
}

vector<std::string> WeddingFoodData::getDecorateCategore(){
    vector<std::string> temp;
    if(foodIndex == 1){
        temp.push_back("icing");
        temp.push_back("candy");
        temp.push_back("cookie_cream");
        temp.push_back("cookie_fruit");
    }else {
        temp.push_back("nuts");
        temp.push_back("cream");
        temp.push_back("topping");
        temp.push_back("plates");
        temp.push_back("forks");
    }
    return temp;
}
std::vector<std::string> WeddingFoodData::getIcingIcon(){
    vector<std::string> temp;
    stringstream stringstrem;
    for(int i=0;i<=3;i++){
        stringstrem.str("");
        stringstrem.clear();
        stringstrem<<"icon_category/icing/icing"<<4*cookieShape+i<<".png";
        temp.push_back(stringstrem.str());
    }
    for(int i=40;i<=52;i++){
        stringstrem.str("");
        stringstrem.clear();
        stringstrem<<"icon_category/icing/icing"<<i<<".png";
        temp.push_back(stringstrem.str());
    }
    return temp;
}

std::string WeddingFoodData::getDecIcing(size_t index){
    stringstream stringstrem;
    if(index<=3)
        stringstrem<<"category/icing/icing"<<4*cookieShape+index<<".png";
    else
        stringstrem<<"category/icing/icing"<<index+40-4<<".png";
    return stringstrem.str();
}

const std::string& WeddingFoodData::getPlatePath(){
    return platesPaths[foodIndex];
}
void WeddingFoodData::setPlatePath(const std::string& s){
    platesPaths[foodIndex] = s;
}
void WeddingFoodData::setForksPath(const std::string& s){
    forksPath =s;
}
const std::string& WeddingFoodData::getForksPath(){
    return forksPath;
}

std::string WeddingFoodData::getDecorateTtitle(){
    switch (foodIndex) {
        case 2:
            return "ui/dec/text_fruittart.png";
            break;
            
        default:
            return "ui/dec/text_cookie.png";
            break;
    }
}

const std::vector<std::string>& WeddingFoodData::getAllPlatePath(){
    return platesPaths;
}

std::vector<std::string> WeddingFoodData::getAllEatPath(){
    std::vector<std::string> temps;
    if(eatFoodPath.size() < 3){
        for(int i=0;i<3;i++)
            temps.push_back("png/cookies/6/cookies0/cookies0_0.png");
    }else {
        for(int i=0;i<3;i++){
            if(eatFoodPath[i].empty())
                temps.push_back("png/cookies/6/cookies0/cookies0_0.png");
            else
                temps.push_back( FileUtils::getInstance()->getWritablePath() + eatFoodPath[i]);
        }
    }
    return temps;
}

bool WeddingFoodData::checkPlayAllFoods(){
    bool flag = true;
    for(auto str:eatFoodPath)
        if(str.empty()){
            flag = false;
            break;
        }
    return flag;
}

Sprite* WeddingFoodData::createEatDonwFood(decltype(foodIndex) index){
    Sprite* s;
    switch (index) {
        case 0:
        case 2:
            s = Sprite::create(FileUtils::getInstance()->getWritablePath() + eatFoodPath[index]);
           
            break;
        default:{
            stringstream sstream;
            sstream<<"png/cookies/6/cookies"<<cookieFlavor<<"/cookies"<<cookieFlavor<<"_"<<cookieShape<<".png";
            s = Sprite::create(sstream.str());
        }
    }
    return s;
}