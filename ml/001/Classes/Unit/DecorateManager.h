
#ifndef __DecorateManager__
#define __DecorateManager__


#include "MakeEatLayer.h"
#include "cocos2d.h"
#include "DrinkUtil.h"
#include "SauceUtil.h"
#include <string.h>
USING_NS_CC;

enum{
    eDecorateTypeFood,
    eDecorateTypeDrink,
    eDecorateTypeBoth,
};

enum{
    eDecorationLayerFoodBack,
    eDecorationLayerFood,
    eDecorationLayerFoodFront,
    eDecorationLayerDrinkBack,
    eDecorationLayerDrink,
    eDecorationLayerDrinkFood,
    eDecorationLayerDrinkFront,
    eDecorationLayerAllUneatFront,
    eDecorationLayerAllFront,
    eDecorationLayerMultDrink,
    eDecorationLayerMultSauce,
};
/*分为3类
 1 food类
 分为最下面一层的不可吃的
 中间一层的可吃的
 最上面一层的不可吃的
 
 2 drink类
 分为最下面一层的不可吃的
 中间一层的喝的
 最上面一层的不可吃的
 
 3 food&drink类
 food类和drink类同时有
 */
class DecorateManager : public Node
{
public:
    DecorateManager();
    ~DecorateManager();
    
    static DecorateManager* getInstance()
    {
        static DecorateManager* ret = nullptr;
        if(!ret){
            ret = new DecorateManager();
        }
        return ret;
    }
    void initWithParent(Node* parent,int type);
    void reset();
    void reDecorate();
    void addDecoration(Node* pUnit,int layerFlag,const std::string singleName = "",Vec2 position = Vec2::ZERO,int zOrder = 0);
    void setDrinkMaskSprite(Sprite* pMask);
    void doDrink();
    void stopDrink();
    void doEat(Vec2 location);
    
    void saveEatLayer();
    
    void clearExtra(){
        m_pAllFrontLayer->removeAllChildren();
        Vector<Node*>pChildVector = m_pFoodUneatableFrontLayer->getChildren();
        for_each(pChildVector.begin(), pChildVector.end(), [=](Node* pNode){
            if (strcmp(pNode->getName().c_str(), "bag")!=0) {
                pNode->removeFromParent();
            }
        });
        m_pAllUneatFrontLayer->removeAllChildren();
        //        m_pFoodUneatableFrontLayer->removeAllChildren();
    }
    
    void showFinishedFood(Node* pParent){
        //        不可吃的back （如杯子后部）
        Layer* pUneatableLayer = getUneatableLayer();
        if (pUneatableLayer) {
            pParent->addChild(pUneatableLayer);
        }
        if(m_pDrinkUneatableFrontLayer){
            ClippingNode* pStrawNode = dynamic_cast<ClippingNode*>(m_pDrinkUneatableFrontLayer->getChildByName("straw_clipping"));
            if (pStrawNode) {
                Sprite* pStraw = dynamic_cast<Sprite*>(pStrawNode->getChildByName("straw"));
                if(pStraw){
                    Sprite* pStrawNew = Sprite::createWithTexture(pStraw->getTexture());
                    pParent->addChild(pStrawNew);
                    pStrawNew->setPosition(pStraw->getPosition());
                    pStrawNew->setName("straw");
                    Vec2 pos = pStraw->getPosition();
                    pos = pStraw->getParent()->convertToWorldSpace(pos);
                    pos = pParent->convertToNodeSpace(pos);
                    pStrawNew->setPosition(pos);
                }
            }
        }
        //        喝的
        RenderTexture* drinkRenderTexture = getDrinkRenderTexture();
        if (drinkRenderTexture) {
            if (drinkRenderTexture->getParent()) {
                drinkRenderTexture->removeFromParent();
            }
            pParent->addChild(drinkRenderTexture);
            _showDrink(100);
        }
        if(m_pDrinkLayer){
            m_pEatableLayer = (MakeEatLayer*)getEatableLayer();
            if (m_pEatableLayer) {
                pParent->addChild(m_pEatableLayer);
            }
        }
        //        可吃的中部的
        if(!m_pDrinkLayer){
            m_pEatableLayer = (MakeEatLayer*)getEatableLayer();
            if (m_pEatableLayer) {
                pParent->addChild(m_pEatableLayer);
            }
        }
        //        不可吃的前部 （如杯子前部）
        
        if (m_pFoodUneatableFrontLayer) {
            Sprite* pNode = (Sprite*)m_pFoodUneatableFrontLayer->getChildByName("bag");
            if (pNode) {
                Size size = pNode->getContentSize()*pNode->getScale();
                RenderTexture* rt = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                          GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
                
                Vec2 pos = pNode->getPosition();
                pNode->setPosition(size*0.5);
                rt->beginWithClear(1.0f, 1.0f, 1.0f, 0);
                pNode->visit();
                rt->end();
                Director::getInstance()->getRenderer()->render();
                pNode->setPosition(pos);
                __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
                __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
                
                Sprite* unit = Sprite::createWithTexture(rt->getSprite()->getTexture());
//                Sprite* unit = Sprite::createWithTexture(pNode->getTexture());
                unit->setPosition(pNode->getPosition());
//                unit->setScale(pNode->getScale());
                unit->setFlippedY(true);
                pParent->addChild(unit);
                unit->setName("bag");
            }
        }
        Layer* pFoodFrontLayer = getFoodFrontLayer();
        if (pFoodFrontLayer) {
            pParent->addChild(pFoodFrontLayer);
        }

        Layer* pDrinkFrontLayer = getDrinkFrontLayer();
        if (pDrinkFrontLayer) {
            pParent->addChild(pDrinkFrontLayer);
        }
        
        //        可吃的前部的
        m_pEatableFrontLayer = (MakeEatLayer*)getEatableFrontLayer();
        if (m_pEatableFrontLayer) {
            pParent->addChild(m_pEatableFrontLayer);
        }
        
        //        不可吃的前部的
        Layer* pUneatableFront = getUneatableFrontLayer();
        if (pUneatableFront) {
            pParent->addChild(pUneatableFront);
        }
//        m_DrinkShareVector.clear();
//        for_each(m_DrinkVector.begin(), m_DrinkVector.end(), [=](Node* drink){
//            DrinkUtil* util = (DrinkUtil*)drink->getChildByName("drink");
//            if (util) {
//                DrinkUtil* utilClone = util->clone();
//                utilClone->setDrinkable();
//                pParent->addChild(utilClone);
//                utilClone->setPosition(pParent->convertToNodeSpace(util->getParent()->convertToWorldSpace(util->getPosition())));
//                m_DrinkShareVector.pushBack(utilClone);
//            }
//        });
//        
//        m_SauceShareVector.clear();
//        for_each(m_SauceVector.begin(), m_SauceVector.end(), [=](Node* sauce){
//            SauceUtil* util = (SauceUtil*)sauce->getChildByName("sauce");
//            if (util) {
//                Node* sauceNode = util->getDecorateResult();
//                pParent->addChild(sauceNode);
//                sauceNode->setPosition(pParent->convertToNodeSpace(util->getParent()->convertToWorldSpace(util->getPosition())));
//                m_SauceShareVector.pushBack(sauceNode);
//            }
//        });
        m_bFinishedEat = false;
        DecorateManager::getInstance()->m_nDrinkPercent = 100;
    }
    Layer* getUneatableLayer();
    Layer* getUneatableFrontLayer();
    MakeEatLayer* getEatableLayer();
    MakeEatLayer* getEatableFrontLayer();
    Layer* getFoodFrontLayer();
    Layer* getDrinkFrontLayer();
    RenderTexture* getDrinkRenderTexture();
    
    bool finishedEat(){return m_bFinishedEat;}
protected:
    void _initFoodLayer();
    void _initDrinkLayer();
    Layer* _initLayer(Layer *pLayer);
    
    void _updateDrink(float dt = 0.1f);
    void _showDrink(int percent);
    void _updateFinishedPercent();
    
    Layer* m_pFoodUneatableBackLayer;
    Layer* m_pFoodUneatableFrontLayer;
    Layer* m_pDrinkUneatableBackLayer;
    Layer* m_pDrinkUneatableFrontLayer;
    Layer* m_pFoodLayer;
    Layer* m_pDrinkLayer;
    Layer* m_pDrinkFoodLayer;
    Layer* m_pAllFrontLayer;
    Layer* m_pAllUneatFrontLayer;
    RenderTexture* m_pDrinkDecRT;
    Sprite* m_pStraw;
    ClippingNode* m_pStrawClipping;
    Node* m_pParent;
    
    //    eat 相关
    MakeEatLayer* m_pEatableLayer;
    MakeEatLayer* m_pEatableFrontLayer;
    RenderTexture* m_pDrinkRT;
    Sprite* m_pDrinkMaskSprite;
    Sprite* m_pDrinkSprite;
    
    
    Size m_DrinkSize;
    int m_nDrinkPercent;
    bool m_bFinishedEat;
    
    
    //    new add  装饰多个饮料
    Vector<Node*> m_DrinkVector;
    Vector<Node*> m_SauceVector;
    
    Vector<Node*> m_DrinkShareVector;
    Vector<Node*> m_SauceShareVector;
    
    Layer* m_pMultiDrinkLayer;
    Layer* m_pMultiSauceLayer;
};

#endif