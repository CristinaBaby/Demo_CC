//
//  SodaCutFruitScene.cpp
//  CCATS022
//
//  Created by liji on 16/11/7.
//
//

#include "SodaCutFruitScene.h"
#include "ShopScene.h"
#include "SceneHeader.h"
#include "MakeNodeManager.h"
#include "ComponentCutter.h"


SodaCutFruitScene::SodaCutFruitScene():
_partIndex(0),
_board(nullptr),
_bowl(nullptr)
{

}

SodaCutFruitScene::~SodaCutFruitScene()
{

}

bool SodaCutFruitScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
//    _screenEventName = "food0.0_milkshake_cutfruit";

    this->setBackground("res/1cut_fruit/make_bg1.png");
    
    _board = Sprite::create("res/1cut_fruit/chopping_board.png");
    this->addToContentLayer(_board);
    ActionHelper::moveInToSceneBounce(_board, CMVisibleRect::getPosition(320, 352), kActionDirectionLeft,CC_CALLBACK_0(SodaCutFruitScene::initChooseFruit, this));
    
    this->onNextButtonTouchCallback = []
    {
        kESDataManager->setNextStep(1);
        SceneManager::getInstance()->replaceWithDefaultTransition(SodaChooseStepScene::scene());
    };
    
    auto food = kESDataManager->getSodaFood();
    food.flavorIndexVec.clear();
    kESDataManager->setSodaFood(food);
    
    return true;
}

void SodaCutFruitScene::initChooseFruit()
{
    _pieceVec.clear();
    
    DecorationConfig config = DecorationConfig("flavor",
                                               "flavor",
                                               "",
                                               "",
                                               "res/1cut_fruit/icon_fruit/%d.png",
                                               "",
                                               0,
                                               14
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(560,180),
     "ui/tips/choose.png",
     "ui/tips/choose_up.png",
     CMVisibleRect::getPosition(320, 210,kBorderNone,kBorderTop),
     CMVisibleRect::getPosition(320, 210-300,kBorderNone,kBorderTop),
     110+30
    );
    
    
    auto callback = [=](ui::Widget* item)
    {
        //            setTypeEnable(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getSodaFood();
        food.flavorIndexVec.push_back(itemTag);
        kESDataManager->setSodaFood(food);
        
        
        this->removeScrollView(true);
        this->initCutFruit(itemTag);
        this->hideNextButton();
    };
    
    if (kESDataManager->getSodaFood().flavorIndexVec.size()>0)
    {
        this->showNextButton();
    }
    

    
    this->createScrollView(config,callback,sConfig,true);
}


void SodaCutFruitScene::initCutFruit(int index)
{
    auto flavorName = kESDataManager->getFruitFlavorNameByIndex(index);
    ESDataManager::CutFood *cutFood = kESDataManager->getCutFoodByName(flavorName);
    Vec2 delta = Vec2(177,136)/2;
    vector<Vec2> cutPos = cutFood->cutterOffsetVector;
    string name = cutFood->name;
    int partsCount = cutFood->partsCount;
    _partIndex = 0;
    
    auto cFoodNode = Node::create();
    for (int i = 0; i < partsCount; ++i)
    {
        auto file = StringUtils::format("res/1cut_fruit/fruit/%s%d.png",flavorName.c_str(),i+1);
        auto partSprite = Sprite::create(file);
        partSprite->setTag(i);
        cFoodNode->addChild(partSprite, cutFood->partsCount - i );
        _pieceVec.push_back(partSprite);
    }
    cFoodNode->setPosition(Vec2(290,201));
    _board->addChild(cFoodNode, 5);
    
    this->cut(cutPos, cFoodNode,cutFood);
    kAudioUtil->playEffect(kVoiceJuiceCut);
    
    
}

void SodaCutFruitScene::cut(vector<Vec2> cutPos, Node *friesNode,ESDataManager::CutFood *cutFood)
{
    int totalCount = cutFood->partsCount;
    
    Vec2 offset = cutPos.at(_partIndex);
    auto file = "res/1cut_fruit/knife.png";
    auto knife = Sprite::create(file);
    knife->setPosition(offset + Vec2(-48,10));
    knife->setZOrder(totalCount - _partIndex - 1);
    knife->setScale(0.7f);
    knife->setFlippedX(true);
    friesNode->addChild(knife);

//    friesNode->setScale(0.95f);
    if (_partIndex == 0)
    {
        auto call = [=]
        {
            auto node = TipsUIController::getInstance()->showMoveTip(friesNode, knife->getPosition()+Vec2(160, 80), knife->getPosition()+Vec2(160, 0));
            node->setScale(0.8f);
        };

        ActionHelper::moveInToSceneBounce(knife, offset + Vec2(-48,10), kActionDirectionLeft,call);
    }
    //
    //    //显示第一个切割指示器
    //    dynamic_cast<CutterGuideLine*>(friesNode->getChildByName(std::to_string(_partIndex)))->start();
    //
    float height = friesNode->getContentSize().height + 230;
    if (cutFood->name == "banana")
    {
        height = friesNode->getContentSize().height + 150;
    }
    auto cutComp = ComponentCutter::create();
    knife->addComponent(cutComp);
    
    ComponentCutter::CutModeAutoConfig cutConfig = {height, 0.5, 1};
    cutComp->setCutMode(ComponentCutter::CutMode::kCutModeAuto);
    cutComp->setAutoCutConfig(cutConfig);
    
    auto touchComp = ComponentTouchMove::create();
    touchComp->setName("ComponentTouchMove");
    knife->addComponent(touchComp);
    touchComp->setIsCanMove(false);
    touchComp->touchEndedCall = [=](Touch* touch)
    {
        TipsUIController::getInstance()->removeAllTips();
        touchComp->setEnabled(false);
        cutComp->start();
        kAudioUtil->playEffect(kSoundCut,true);
        
    };
    touchComp->start();
    
    cutComp->cutFinishCallBack = [=]()
    {
        kAudioUtil->stopAllEffect();
        
        auto part = friesNode->getChildByTag(_partIndex);
        if (part)
        {
            part->runAction(Sequence::create(MoveBy::create(0.3, Vec2(40-_partIndex*12, 0)),
                                             CallFunc::create([=]
            {
                knife->removeFromParent();
                ++_partIndex;
                if (_partIndex >= totalCount-1)
                {
                    log("next");
                    kSoundInstance->playRandomStepFinishVoice();
                    this->pourInBowl();
                }
                else
                {
                    cut(cutPos,friesNode,cutFood);
                }
            }),
                                             nullptr));

        }
    };
}

void SodaCutFruitScene::pourInBowl()
{
    if (!_bowl)
    {
        _bowl = BowlNode::create("res/1cut_fruit/bowl1_down.png", "res/1cut_fruit/bowl1_up.png");
        this->addToContentLayer(_bowl,11);
        _bowl->setScale(0.9f);
        ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getPosition(457, 357), kActionDirectionRight);
    }
    TipsUIController::getInstance()->removeAllTips();

    vector<Vec2> icePosVec =
    {
        Vec2(-38, -41),
        Vec2(40, -53),
        Vec2(-80, 23),
        Vec2(-4, 10),
        Vec2(83, 16),
    };

    
    auto pourCall = [=]
    {
        for (int i = 0; i<_pieceVec.size() ;i++ )
        {
            auto part = _pieceVec.at(i);
            part->runAction(Sequence::create(DelayTime::create(i*0.2),
                                             CallFunc::create([=]
            {
                kAudioUtil->playEffect(kSoundAddCommon);
            }),
                                             Spawn::create(MoveBy::create(0.3f, Vec2(60, 0)),
                                                           FadeOut::create(0.3f),
                                                           NULL),
                                             NULL));
            
            
        }
        
        
//        auto particle = ParticleSystemQuad::create("particle/ice.plist");
//        _bowl->addContent(particle, "parts", 100);
//        particle->setPosition(22, 195);
//        particle->setTexture(_director->getTextureCache()->addImage(StringUtils::format("res/1cut_fruit/fruit_piece/%s0.png",kESDataManager->getCurrentFruitFlavorName().c_str())));
        
        for (int i = 0; i<icePosVec.size(); i++)
        {
            auto iceInBowl = Sprite::create(StringUtils::format("res/1cut_fruit/fruit_piece/%s0.png",kESDataManager->getCurrentFruitFlavorName().c_str()));
            int y = random(60, 90);
            Vec2 pos = icePosVec.at(i) + Vec2(0, y);
            iceInBowl->setRotation(rand()%220);
            iceInBowl->setScale(rand()%3*0.1+1.2);
            iceInBowl->setTag(i);
            _bowl->addContent(iceInBowl, StringUtils::format("iceInBowl%d",i),1,pos);
            iceInBowl->setOpacity(255);
            iceInBowl->setVisible(false);
            iceInBowl->runAction(Sequence::create(DelayTime::create(i*0.2),
                                                  Show::create(),
                                                  EaseBackIn::create(MoveBy::create(0.4f, Vec2(0, -y))),
                                                  CallFunc::create([=]
                                                                   {
                                                                       if (i==icePosVec.size()-1)
                                                                       {
                                                                           this->removeBowl();
                                                                       }
                                                                   }),
                                                  NULL));
        }
    };
    
    
    _board->runAction(Sequence::create(MoveBy::create(0.6f, CMVisibleRect::getPosition(-300, 0)),
                                       Spawn::create(MoveTo::create(0.5f, CMVisibleRect::getPosition(58, 720)),
                                                     RotateTo::create(0.5f, 21),
                                                     NULL),
                                       CallFunc::create([=]
    {
        pourCall();
    }),
                                       NULL));

}

void SodaCutFruitScene::removeBowl()
{
    auto resetBoardCall = [=]
    {
        _board->runAction(Sequence::create(
                                           Spawn::create(MoveTo::create(0.5f, CMVisibleRect::getPosition(320, 352)),
                                                         RotateTo::create(0.5f, 0),
                                                         NULL),
                                           CallFunc::create([=]
                                                            {
                                                                this->initChooseFruit();
                                                                _bowl->removeFromParentAndCleanup(true);_bowl=nullptr;
                                                            }),
                                           NULL));
    };
    
    _bowl->runAction(Sequence::create(DelayTime::create(0.8f),
                                      MoveBy::create(0.5f, Vec2(600, 0)),
                                      CallFunc::create(resetBoardCall),
                                      NULL));
    
}

