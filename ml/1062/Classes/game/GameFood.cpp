//
//  FoodRead.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/21.
//
//

#include "GameFood.h"
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"
#include "CocosHelper.h"
using namespace cocostudio;
using namespace std;

int GameFood::z_order = 0;
string foodNames[] = {
    "bacon"
    ,"calamari"
    ,"chicken-drumstick"
    ,"cod-fillet"
    ,"mozzarella-cheese-sticks"
    ,"popcorn-chicken"
    ,"onion-flower"
    ,"shrimp"
    ,"potato-wedges"
    ,"wings"
};
GameFood::GameFood():_foodIndex(3){
    
}

Node* GameFood::createBeforeDip(){
    positions.clear();
    auto dipped = CSLoader::createNode("AfterdipEgg/"+foodNames[_foodIndex]+".csb");
    CocosHelper::visitAllNode(dipped, [this](Node* child){
        if(child->getName().find("food") != std::string::npos)
            positions.pushBack(dynamic_cast<Sprite*>(child));
    });
    
    foods.clear();
    auto node = CSLoader::createNode("foodInPlate/"+foodNames[_foodIndex]+".csb");
    z_order = 0;
    CocosHelper::visitAllNode(node, [this](Node* child){
        if(child->getName().find("food") != std::string::npos){
            child->setCascadeOpacityEnabled(false);
            foods.push_back(dynamic_cast<Sprite*>(child));
            child->getParent()->reorderChild(child, z_order);
            z_order++;
        }
    });
    return node;
}

Node* GameFood::createBeforePowder(){
    foods.clear();
    auto node = CSLoader::createNode("coatefood/"+foodNames[_foodIndex]+".csb");
    z_order = 0;
    CocosHelper::visitAllNode(node, [this](Node* child){
        if(child->getName().find("food") != std::string::npos){
            child->setCascadeOpacityEnabled(false);
            foods.push_back(dynamic_cast<Sprite*>(child));
            child->getParent()->reorderChild(child, z_order);
            z_order++;
        }
    });
    return node;
}

Node* GameFood::createFryFood(){
    foods.clear();
    auto node = CSLoader::createNode("AfterdipEgg/"+foodNames[_foodIndex]+".csb");
    auto node2 = CSLoader::createNode("coatefood/"+foodNames[_foodIndex]+".csb");
    z_order = 0;
    CocosHelper::visitAllNode(node, [this,node2](Node* child){
        if(child->getName().find("food") != std::string::npos){
            child->setCascadeOpacityEnabled(false);
            auto food = dynamic_cast<Sprite*>(child);
            food->setSpriteFrame(dynamic_cast<Sprite*>(ui::Helper::seekNodeByName(node2,"dipping"))->getSpriteFrame());
            
            auto fry = Sprite::create(this->getFryPath());
            fry->setName("fry");
            fry->setPosition(food->getContentSize()*.5);
            fry->setOpacity(0);
            food->addChild(fry);
            
            auto fryFailed = Sprite::create(this->getFryFailedPath());
            fryFailed->setName("fryFailed");
            fryFailed->setPosition(food->getContentSize()*.5);
            fryFailed->setOpacity(0);
            food->addChild(fryFailed);
            
            foods.push_back(food);
            child->getParent()->reorderChild(child, z_order);
            z_order++;
        }
    });
    return node;
}

Node* GameFood::createFryEndInplate(){
    auto node = CSLoader::createNode("AfterdipEgg/"+foodNames[_foodIndex]+".csb");
    CocosHelper::visitAllNode(node, [this](Node* child){
        if(child->getName().find("food") != std::string::npos){
            auto food = dynamic_cast<Sprite*>(child);
            food->setSpriteFrame(Sprite::create(getFryPath())->getSpriteFrame());
        }else {
            child->setCascadeOpacityEnabled(false);
            child->setOpacity(0);
        }
    });
    return node;
}

const vector<Sprite*>& GameFood::getFoods(){
    return foods;
}

Sprite* GameFood::getDippedFood(int index){
    Sprite* food = nullptr;
    if(index>=0&&index<positions.size())
        food = positions.at(index);
    return food;
}

std::vector<std::string> GameFood::getDipPath(){
    vector<string> path;
    path.push_back("png/2/food1/"+foodNames[_foodIndex]+"0.png");
    path.push_back("png/2/food1/"+foodNames[_foodIndex]+"1.png");
    return path;
}

std::string GameFood::getFryPath(){
    return "png/4/food/"+foodNames[_foodIndex]+"0.png";
}

std::vector<std::string> GameFood::getChoosePath(){
    vector<string> choosePath;
    for(int i=0;i<sizeof(foodNames)/sizeof(foodNames[0]);i++)
        choosePath.push_back("png/1/menu_"+foodNames[i] +".png");
    return choosePath;
}

std::string GameFood::getDecorationPath(){
    return "png/5/"+foodNames[_foodIndex]+"0.png";
}

const std::vector<Sprite*>& GameFood::getInOilFood(){
    foods.clear();
    auto node = CSLoader::createNode("FryFood/"+foodNames[_foodIndex]+".csb");
    CocosHelper::visitAllNode(node, [this](Node* child){
        if(child->getName().find("food") != std::string::npos){
            child->setCascadeOpacityEnabled(false);
            foods.push_back(dynamic_cast<Sprite*>(child));
        }
    });
    return foods;
}

std::string GameFood::getFryFailedPath(){
     return "png/fryFail/"+foodNames[_foodIndex]+"1.png";
}

std::vector<Vec2> GameFood::getCoateEndPos(){
    std::vector<Vec2> positions;
    auto node = CSLoader::createNode("coateEndPos/"+foodNames[_foodIndex]+".csb");
    for(auto child:node->getChildren().at(0)->getChildren())
            positions.push_back(child->getPosition());
    return positions;
}
