
#include "IngredientManager.h"
#include "cocos2d.h"
#include "AudioHelp.h"

USING_NS_CC;
using namespace cocos2d::ui;

void IngredientManager::addIngredient(DragNode* pIngredient)
{
    m_IngredientVector.pushBack(pIngredient);
}

void IngredientManager::removeIngredient(DragNode* pIngredient)
{
    m_IngredientVector.eraseObject(pIngredient);
}

void IngredientManager::showShadow(int index,bool show)
{
    for_each(m_IngredientVector.begin(), m_IngredientVector.end(), [=](DragNode* pNode){
        if(index == pNode->getTag()){
            ContainerNode* pContainer = (ContainerNode*)pNode->getUserData();
            if(pContainer){
                pContainer->showShadow(show);
            }
        }
    });
    
}

void IngredientManager::frozeIngredients(int index,bool froze)
{
    for_each(m_IngredientVector.begin(), m_IngredientVector.end(), [=](DragNode* pNode){
        if(froze){
            pNode->setTouchEnabled(false);
        }else{
            if(index != pNode->getTag()){
                pNode->setTouchEnabled(true);
            }
        }
    });
}

void IngredientManager::addContainer(Node* pIngredient,std::string path,std::string name[5],bool isWater)
{
    if(!isWater) {
        ContainerNode* pNode = ContainerNode::create(pIngredient);
        if(name[1]!=""){
            pNode->addShadow(path+name[1]);
        }
        if(name[2]!=""){
            pNode->addFront(path+name[2]);
        }
        if(name[3]!=""){
            pNode->addFood(path+name[3]);
        }
        if(name[4]!=""){
            pNode->addFoodShadow(path+name[4]);
        }
        pIngredient->setUserData(pNode);
    }else{
        KettleNode* pNode = KettleNode::create(pIngredient);
        if(name[1]!=""){
            pNode->addShadow(path+name[1]);
        }
        if(name[2]!=""){
            pNode->addFront(path+name[2]);
        }
        if(name[3]!=""){
            pNode->addWater(path+name[3],path+name[5]);
        }
        if(name[4]!=""){
            pNode->addPourWater(path+name[4]);
        }
        pIngredient->setUserData(pNode);
    }
}

