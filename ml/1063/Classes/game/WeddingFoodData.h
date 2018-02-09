//
//  WeddingFoodData.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/1.
//
//

#ifndef WeddingFoodData_hpp
#define WeddingFoodData_hpp

#include <iostream>
#include "cocos2d.h"
class WeddingFoodData {
protected:
    static size_t cookieShape;
    static size_t cookieFlavor;
    static size_t foodIndex;
    static std::vector<decltype(cookieShape)> cookieShapes;
    static std::vector<std::string> eatFoodPath;
    static std::vector<std::string> platesPaths;
    static std::string forksPath;
public:
    static void setCookieShapes(const decltype(cookieShapes)&);
    static  auto getCookieShapes() ->decltype(cookieShapes);
    static std::vector<std::string> getBakeEndCookie();
    static void setCookieFlavor(decltype(cookieFlavor));
    static auto  getCookieFlavor() ->decltype(cookieFlavor);
    static void setShape(decltype(cookieShape));
    static auto getShape() ->decltype(cookieShape);
    static std::string getDecorationFood();
    static std::vector<std::string> getMixingCookie();
    static std::vector<std::string> getRollCookie();
    static std::string getCookieVanillaBottle();
    static std::string getCookieVanilla();
    static void setFoodIndex(decltype(foodIndex));
    static auto getFoodIndex()->decltype(foodIndex);
    static std::string getEatPath();
    static std::string getEatLayerPath();
    static std::string getListViewPath();
    static void saveFood(cocos2d::RenderTexture*);
    static void saveAddFruitFritTart(cocos2d::RenderTexture*);
    static std::vector<std::string> getDecorateCategore();
    static std::vector<std::string> getIcingIcon();
    static std::string getDecIcing(size_t index);
    static const std::string& getPlatePath();
    static void setPlatePath(const std::string&);
    static void setForksPath(const std::string&);
    static const std::string& getForksPath();
    static  std::string getDecorateTtitle();
    static const  std::vector<std::string>& getAllPlatePath();
    static   std::vector<std::string> getAllEatPath();
    static bool checkPlayAllFoods();
    static cocos2d::Sprite* createEatDonwFood(decltype(foodIndex) index );
};
#endif /* WeddingFoodData_hpp */
