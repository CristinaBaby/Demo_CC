//
//  ChangeLoading.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/20.
//
//

#include "ChangeLoading.h"
//#include "DrawLine.h"
#include "SimpleAudioEngine.h"
#include "AdsManager.h"


using namespace CocosDenshion;

ChangeLoading::ChangeLoading():endFunc(nullptr){
    
}

void ChangeLoading::loading(std::function<void()> end){
    auto _scene = Scene::create();
    auto _layer = ChangeLoading::create();
    _layer->endFunc = end;
    _scene->addChild(_layer);
    Director::getInstance()->popToRootScene();
    Director::getInstance()->replaceScene(_scene);
}

void ChangeLoading::onEnter()
{
    Layer::onEnter();
    
//    AdsManager::getInstance()->onAdsExpanded=[=](ADS_TYPE type){
//    
//        if(type == ADS_TYPE::kTypeInterstitialAds || type == ADS_TYPE::kTypeCrosspromoAds)
//            Director::getInstance()->pause();
//    
//    };
//    AdsManager::getInstance()->onAdsCollapsed=[=](ADS_TYPE type){
//        if(type == ADS_TYPE::kTypeInterstitialAds || type == ADS_TYPE::kTypeCrosspromoAds)
//            Director::getInstance()->resume();
//        
//    };
    
}
void ChangeLoading::onExit()
{
    Layer::onExit();
    
//    AdsManager::getInstance()->onAdsExpanded=nullptr;
//    AdsManager::getInstance()->onAdsCollapsed=nullptr;
//    
}
void ChangeLoading::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
}
bool ChangeLoading::init(){
    if(Layer::init()){
        
        
        isEnd = false;
        float offsetY = (getContentSize().height-640)*.5;
        float offsetX = (getContentSize().width-960)*.5;
        Vec2 offset = Vec2(offsetX,offsetY);
        
        _logo = Sprite::create("images/loading/logo.png");
        _logo->setAnchorPoint(Vec2(0,1));
        _logo->setPosition(5,getContentSize().height-5);
        addChild(_logo,30);
        _logo->setVisible(false);
        
        auto _bg = Sprite::create("images/loading/loading_bg.png");
        _bg->setPosition(getContentSize()*.5);
        addChild(_bg,0);
        
        auto tabel = Sprite::create("images/loading/desk.png");
        tabel->setPosition(getContentSize().width*.5,offsetY+99.22);
        addChild(tabel,2);
        
        Node* bear = Node::create();
        bear->setScale(0.1);
        addChild(bear,1);
        auto inner = Sprite::create("images/loading/loading_bear0.png");
        inner->setAnchorPoint(Vec2(0.5,0.12));
        bear->addChild(inner);
        
        
        Vec2 pos[]= {
            {104.88,232.93},
            {97.17,62.46},
            {109.77,261.68},
            {114.50,328.64},
            {33.17,49.66},
            {152.65,46.45}
            ,{101.44,160.60}
        };
        
        Vector<Sprite*> nodes;
        
        for(int i=0;i<sizeof(pos)/sizeof(pos[0]);i++){
            int index =i;
            if(index == 5)
                index--;
            auto s = Sprite::create(StringUtils::format("images/loading/loading_bear%d.png",index+1));
            s->setPosition(pos[i]);
            inner->addChild(s);
            nodes.pushBack(s);
        }
        
        
        Vec2 BeziePos[] = {
            Vec2(52.266304,149.655823),
            Vec2(115.911690,448.227722),
            Vec2(113.103821,448.227722),
            Vec2(161.773819,254.237228),
            Vec2(239.458649,536.208130),
            Vec2(237.586731,537.144043),
            Vec2(289.064667,289.114197),
            Vec2(394.828308,612.020996),
            Vec2(394.828308,610.149048),
            Vec2(480,204.63)
        };
        
//                auto _draw = DrawLine::create();
//                _draw->ignoreAnchorPointForPosition(false);
//                _draw->setAnchorPoint(Vec2(0.5,0.5));
//                _draw->setContentSize(Size(960,640));
//                _draw->setPosition(getContentSize()*.5);
//                addChild(_draw,30);
//                _draw->setPos(vector<Vec2>(BeziePos,BeziePos+sizeof(BeziePos)/sizeof(BeziePos[0])));
        
        bear->setPosition(BeziePos[0]+offset);
        Vector<FiniteTimeAction*> actionList;
        float scaleTime = 0.13+0.11+0.1+0.09+0.08+0.07;
        float scale[] = {0.4,0.6,1};
        int j=0;
        for(int i=1;i<sizeof(BeziePos)/sizeof(BeziePos[0]);i=i+3){
            
            float time = 0.6;
            ccBezierConfig _config;
            _config.endPosition = BeziePos[i+2]+offset;
            _config.controlPoint_1 = BeziePos[i]+offset;
            _config.controlPoint_2 = BeziePos[i+1]+offset;
            actionList.pushBack(Spawn::create(BezierTo::create(time, _config),ScaleTo::create(time, scale[j]), nullptr));
            actionList.pushBack(CallFunc::create([j]{
                
                SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("images/loading/sound/loading_%d.mp3",j+1).c_str());
            }));
            if(j== 2){
                actionList.pushBack(CallFunc::create([inner](){
                    ActionInterval *ret = NULL;
                    ScaleTo *lScale1 = ScaleTo::create(0.13, 1.2, 0.78);
                    ScaleTo *lScale2 = ScaleTo::create(0.11, 0.92, 1.0);
                    ScaleTo *lScale3 = ScaleTo::create(0.10, 1.08, 0.86);
                    ScaleTo *lScale4 = ScaleTo::create(0.09, 0.98, 1.0);
                    ScaleTo *lScale5 = ScaleTo::create(0.08, 1.0, 0.92);
                    ScaleTo *lScale6 = ScaleTo::create(0.07, 1, 1.0);
                    ret = Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
                    inner->runAction(ret);
                }));
                actionList.pushBack(DelayTime::create(scaleTime));
            }else
                actionList.pushBack(DelayTime::create(0.3));
            j++;
        }
        
        bear->runAction(Sequence::create(DelayTime::create(2),CallFunc::create([bear](){
            bear->getParent()->reorderChild(bear, 3);
        }), nullptr));
        
        
        bear->runAction(Sequence::create(DelayTime::create(2.3-0.6),CallFunc::create([bear, inner,nodes,this](){
            
            int fallIndexs[] = {3,2,1,6,4,5,0};
            for(int j= 0;j<nodes.size();j++){
                int index = fallIndexs[j];
                auto node= nodes.at(index);
                node->runAction(Sequence::create(DelayTime::create(0.1*j),CallFunc::create([node,index,this,j,nodes](){
                    auto _particle = ParticleSystemQuad::create("images/loading/fallThing.plist");
                    _particle->setPosition(node->convertToWorldSpace(node->getContentSize()*.5));
                    _particle->setTag(index);
                    _particle->setPositionType(ParticleSystem::PositionType::GROUPED);
                    _particle->setTexture(node->getTexture());
                    this->addChild(_particle,5);
                    node->runAction(Sequence::create(DelayTime::create(1.2), CallFunc::create([index,node,this,j,nodes](){
                        node->setVisible(false);
                        getChildByTag(index)->setPosition(node->convertToWorldSpace(node->getContentSize()*.5));
                        if(j == nodes.size()-1)
                            isEnd = true;
                    }),nullptr));
                }), nullptr));
                
            }
        }), nullptr));
        
        bear->runAction(Sequence::create(Sequence::create(actionList),CallFunc::create([bear, inner,nodes,this](){
            SimpleAudioEngine::getInstance()->playEffect("images/loading/sound/hippoTap.mp3");
            float time = 0.05;
            float rote = 17;
            int repeat = 5;
            auto a1 = RotateTo::create(time, rote);
            auto a2 = RotateTo::create(time*2, -rote);
            auto a3 = RotateTo::create(time, 0);
            auto se = Spawn::create(Repeat::create(Sequence::create(a1,a2,a3, nullptr),repeat),JumpBy::create(time*repeat*4, Vec2::ZERO, 40, 1), nullptr);
          
            bear->runAction(RepeatForever::create(Sequence::create(se,CallFunc::create([bear,inner,this](){
                if(isEnd){
                    bear->stopAllActions();
                    inner->runAction(Sequence::create(DelayTime::create(0.6),EaseExponentialOut::create(RotateBy::create(0.7,30)),CallFunc::create([inner](){
                         SimpleAudioEngine::getInstance()->playEffect("images/loading/sound/yay.mp3");
                        inner->setSpriteFrame(Sprite::create("images/loading/loading_bearsmile.png")->getSpriteFrame());
                    }), DelayTime::create(0.2), EaseExponentialIn::create(RotateBy::create(0.7, -30)),CallFunc::create(CC_CALLBACK_0(ChangeLoading::showLogo, this)), nullptr));
                    
                }
            }), DelayTime::create(0.2), nullptr)));
            
        }), nullptr));
        return true;
    }
    return false;
}

void ChangeLoading::showLogo(){
     SimpleAudioEngine::getInstance()->playEffect("images/loading/sound/mold in .mp3");
    auto _pos = _logo->getPosition();
    _logo->setPositionY(_logo->getPositionY()+500);
    _logo->setVisible(true);
    _logo->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.9, _pos)),CallFunc::create([this](){
        if(endFunc != nullptr){
//            auto scene = endFunc();
//            if(scene != nullptr){
//                Director::getInstance()->replaceScene(scene);
//            }
            endFunc();
            
        }
    }), nullptr));
}

