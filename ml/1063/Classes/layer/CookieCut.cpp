//
//  CookieCut.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#include "CookieCut.h"
#include "DragBtnAdapter.h"
#include "FilePathUtil.h"
#include "BaseShopLayer.h"
#include "MoveSprite.h"
#include "CookieBake.h"
#include "TouchComponent.h"
#include "ChangeLoading.h"
USING_NS_KD;
REGIST_SECENE(CookieCut);
class CutterAdapter : public DragBtnAdapter {
public:
    CREATE_ADAPTER(CutterAdapter);
    
    CutterAdapter(){
        freeClick = nullptr;
        itemClick = std::bind(&CutterAdapter::_itemClick, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    }
    
    EventSprite * getItem(size_t position){
        if(!IAPManager::getInstance()->isFree("cutter",position+1)){
            auto item = EventSprite::create(file_path.at(position));
            auto lock = Sprite::create("lock.png");
            lock->setName("_lockName");
            lock->setPosition(item->getContentSize()*.5);
            item->addChild(lock);
            return item;
        } else {
            return DragBtnAdapter::getItem(position);
        }
    }
    
    virtual void setItemClick(ItemClick click){
        freeClick = click;
    }
protected:
    ItemClick freeClick;
protected:
    void _itemClick(EventSprite* t,size_t _index,MyScollView* s){
        if(!IAPManager::getInstance()->isFree("cutter",_index+1)){
            auto _shop = dynamic_cast<BaseShopLayer*>( DynObject::createDynObject("ShopLayer"));
            if(_shop != nullptr) {
                Director::getInstance()->getRunningScene()->addChild(_shop);
                _shop->_callBack = [s,this](){
                    auto adapter = CutterAdapter::create(file_path);
                    adapter->setRects(this->limitRects);
                    adapter->setItemClick(freeClick);
                    s->setAdapter(adapter);
                };
            }
        }else if(freeClick != nullptr){
            freeClick(t,_index,s);
        }
    }
};

CookieCut::CookieCut(){
     cookieIndexs = WeddingFoodData::getCookieShapes();
}
bool CookieCut::init(){
    if(BaseStepLayer::init()) {
       
        
        _operate = StudioLayer::create("CookieCut.csb");
        addChild(_operate);
        
        CocosHelper::visitAllNode(_operate, [](Node* node){
            auto _paritcle = dynamic_cast<ParticleSystemQuad*>(node);
            if(_paritcle != nullptr)
                _paritcle->stopSystem();
        });
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        auto paths = WeddingFoodData::getRollCookie();
        _operate->changeNode("dough", *(paths.end()-1));
        
        for(int i=0;i<=5;i++)
            limits.push_back(CocosHelper::getNodeRectInWorld(_operate->getNodeByName(StringUtils::format("limit_%d",i))));
        auto toolBg = _operate->getNodeByName("toolBg");
        
        auto size = Director::getInstance()->getVisibleSize();
        _listView = MyScollView::create(size.width, 129);
        _listView->setDirection(MyScollView::Direction::HORIZONTAL);
        _listView->setPosition(toolBg->getContentSize()*.5);
        toolBg->addChild(_listView);
        vector<string> filePath;
        int num = FilePathUtil::getFileNum("png/cookies/4/cutter_big/cutter_small");
        for(int i=0;i<num;i++)
            filePath.push_back(StringUtils::format("png/cookies/4/cutter_big/cutter_small/cutter_small%d.png",i));
        auto _adapter = CutterAdapter::create(filePath);
        Vector<LQPolygon *> rects;
        for(int i=0;i<num;i++)
            rects.pushBack(LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName(StringUtils::format("dough")))));
        _adapter->setRects(rects);
        _listView->setSpace(20);
        _listView->setAdapter(_adapter);

        _listView->getContainer()->setPositionX(-400);
        _adapter->setItemClick([this](EventSprite * s, size_t index, MyScollView *){
            for(auto itor = limits.begin();itor != limits.end();++itor){
                auto p = s->convertToWorldSpace(s->getContentSize()*.5);
                if(itor->containsPoint(p)){
                    _operate->playNodeSound("dough");
                    auto dough = _operate->getNodeByName("dough");
                    _eventDispatcher->setEnabled(false);
                    s->setScale(0);
                    dynamic_cast<MoveSprite*>(s)->moveBack();
                    auto cookie = Sprite::create(StringUtils::format("png/cookies/4/cutter_big/cutter_big%zu.png",index));
                    cookie->runAction(Sequence::create(ScaleTo::create(0.3, 0.8),ScaleTo::create(0.3, 1), nullptr));
                    cookie->setPosition(dough->convertToNodeSpace(Vec2(itor->getMidX(),itor->getMidY())));
                    dough->addChild(cookie);
                    *itor = Rect::ZERO;
                    cookieIndexs[itor-limits.begin()] = index;
                    s->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([](){Director::getInstance()->getEventDispatcher()->setEnabled(true);}), nullptr));
                    bool isEnd = true;
                    for(auto rect:limits)
                        if(!rect.equals(Rect::ZERO)){
                            isEnd = false;
                            break;
                        }
                    
                    if(isEnd){
                        auto mixEndParticle = _operate->getNodeByName<ParticleSystemQuad*>("mixEndPraticle");
                         _operate->playNodeSound("mixEndPraticle");
                        if(mixEndParticle != nullptr)
                            mixEndParticle->resetSystem();
                        ActionHelper::delayFunc(0.4, this, [this](){
                            auto dough = _operate->getNodeByName("dough");
                            dough->runAction(Spawn::create(ScaleTo::create(0.7, 1.2),MoveBy::create(0.7, Vec2(0,70)),nullptr));
                        });
                       
                        auto next = _operate->getNodeByName("btn_next");
                        ActionHelper::showBackInOut(next, next->getPosition(), ActionHelper::ShowDirection::show_from_right,[this](){ _operate->getNodeByName("btn_next_light")->setVisible(true);});
                        WeddingFoodData::setCookieShapes(cookieIndexs);
                        next->setVisible(true);
                        ActionHelper::hide(_operate->getNodeByName("toolBg"), ActionHelper::ShowDirection::show_from_top);
                    }
                    auto btn_reset = _operate->getNodeByName("btn_reset");
                    if(!btn_reset->isVisible()){
                        this->stopPrompt();
                        ActionHelper::showBackInOut(btn_reset, btn_reset->getPosition(), ActionHelper::ShowDirection::show_from_left);
                        btn_reset->setVisible(true);
                    }
                    return;
                }
            }
            dynamic_cast<MoveSprite*>(s)->moveBack();
        });
        return true;
    }
    return false;
}
void CookieCut::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    auto toolBg = _operate->getNodeByName("toolBg");
    auto mixEndParticle = _operate->getNodeByName<ParticleSystemQuad*>("mixEndPraticle");
    if(mixEndParticle != nullptr)
        mixEndParticle->resetSystem();
     _operate->playNodeSound("mixEndPraticle");
    SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    ActionHelper::showBackInOut(toolBg, toolBg->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
        _listView->getContainer()->runAction(MoveBy::create(0.6, Vec2(400,0)));
        ActionHelper::runBezierMoveHideAction(_prompt, _prompt->getPosition(), getContentSize()*.5);
        _prompt->setVisible(true);
        
        auto title = _operate->getNodeByName("text");
        ActionHelper::showBackInOut(title, title->getPosition(), ActionHelper::ShowDirection::show_from_top,[title](){
            
            auto _lis = EventListenerTouchOneByOne::create();
            title->runAction(Sequence::create(DelayTime::create(2.5),CallFunc::create([_lis,title](){
                title->stopAllActions();
                _lis->setEnabled(false);
                ActionHelper::hide(title, ActionHelper::ShowDirection::show_from_top);
            }), nullptr));
            _lis->onTouchBegan = [_lis,title](Touch*,Event*){
                title->stopAllActions();
                _lis->setEnabled(false);
                ActionHelper::hide(title, ActionHelper::ShowDirection::show_from_top);
                return false;
            };
            _lis->setSwallowTouches(false);
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lis, title);
        
        });
        title->setVisible(true);
     
    });
    toolBg->setVisible(true);
}

void CookieCut::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_next"){
        ChangeLoading::loading([](){
             Director::getInstance()->replaceScene(TransitionFade::create(0.8,CookieBake::createScene(),Color3B::WHITE));
        });
       
    } else if(widget->getName() == "btn_reset"){
        Director::getInstance()->replaceScene(CookieCut::createScene());
    }
}