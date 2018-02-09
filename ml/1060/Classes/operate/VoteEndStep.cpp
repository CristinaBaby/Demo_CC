//
//  VoteEndStep.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#include "VoteEndStep.h"
#include "LQ_adLoadingLayer.h"
#include "MyDialog.h"
#include "HomeLayer.hpp"
#include "AblumLayer.h"
#include "Pricelayer.h"
#include "ChangeLayer.h"
#include "DecorationLayer.hpp"
#include "FlurryEventManager.h"
const string VoteEndStep::_eventName = "Vote prize";
IMPLEMENT_COCOS2DX_CLASS(VoteEndStep);
VoteEndStep::VoteEndStep() {
    playIndex = -1;
}
void VoteEndStep::touchEnd(ui::Widget* widget){
    if(widget->getName() == "home_btn"){
        auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
        _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
            if(index == 0) {
                _operate->setIsChange(true);
                auto _endcallBack = [](){
                    LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                    auto _scne = Scene::create();
                    _scne->addChild(HomeLayer::create());
                    Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scne,Color3B::WHITE));
                };
                if(IAPManager::getInstance()->isShowAds()){
                    LQ_adLoadingLayer::loadingDoneCallback = _endcallBack;
                    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
                } else
                    _endcallBack();
                
            }
        };
        
        getScene()->addChild(_dialog);
    }else if(widget->getName().compare("btn_next") == 0){
        ChangeLayer::_changeLayer = [](){
            return Pricelayer::create();
        };
        ChangeLayer::loading();
    }else if(widget->getName().compare("btn_camer") == 0){
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        _operate->getNodeByName("btn_next")->setVisible(false);
        _operate->getNodeByName("btn_camer")->setVisible(false);
          _operate->getNodeByName("btn_reset")->setVisible(false);
        auto _size = Director::getInstance()->getVisibleSize();
        auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        _render->begin();
        visit();
        _render->end();
        Director::getInstance()->getRenderer()->render();
        _operate->getNodeByName("btn_camer")->setVisible(true);
         _operate->getNodeByName("btn_reset")->setVisible(true);
        _operate->getNodeByName("btn_next")->setVisible(true);
        auto _layer = AblumLayer::create();
        addChild(_layer,30);
        _layer->setRender(_render);
        SoundPlayer::getInstance()->playEffect("sound/camera.mp3");
    }else if(widget->getName() == "btn_reset"){
        auto _dialog = MyDialog::create("Do you want to decorate a new donut?", MyDialog::twoBtnPath);
        _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
            if(index == 0) {
                _operate->setIsChange(true);
                ChangeLayer::_changeLayer = [](){
                    return DecorationLayer::create();
                };
                ChangeLayer::loading();
                
            }
        };
        
        getScene()->addChild(_dialog);
        
    }else if(widget->getName().find("balloon") != string::npos){
      
        widget->setEnabled(false);
        widget->stopAllActions();
        widget->setRotation(0);
        auto _pos = widget->getPosition();
        widget->runAction(Sequence::create(MoveBy::create(4+rand()%1, Vec2(0,800)),MoveBy::create(0, Vec2(0,-1300)),DelayTime::create(3), MoveTo::create(2, _pos),CallFunc::create([widget](){
            widget->stopAllActions();
            widget->setRotation(0);
            widget->setEnabled(true);
        }), nullptr));
        RotateBy *lRotate1 = RotateBy::create(0.5, 10);
        RotateBy *lRotate2 = RotateBy::create(0.5, -10);
        Sequence *lSeq1 = Sequence::create(lRotate1, lRotate2, nullptr);
        Repeat *lRepeat = Repeat::create(lSeq1, 20);
        DelayTime *lDelay1 = DelayTime::create(0.1);
        Sequence *lSeq2 = Sequence::create(lDelay1, lRepeat, nullptr);
        widget->runAction(RepeatForever::create(lSeq2));
    }
}

bool VoteEndStep::init() {
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("cocostudio/VoteEnd.csb");
        addChild(_operate);
        
        starParticle = _operate->getNodeByname<ParticleSystemQuad*>("starParticle");
        starParticle->stopSystem();
        _operate->getNodeByname<ParticleSystemQuad*>("fallParticle")->stopSystem();
        _operate->getNodeByName("balloon0")->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.8, -15), RotateBy::create(1.6, 30), RotateBy::create(0.8, -15), nullptr)));
        ActionHelper::delayFunc(0.8, this, [this](){
              _operate->getNodeByName("balloon1")->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.8, -15), RotateBy::create(1.6, 30), RotateBy::create(0.8, -15), nullptr)));
        
        });
        
        return true;
    }
    return false;
}
void VoteEndStep::showImage(int index){
    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    auto _image = _operate->getNodeByName(StringUtils::format("Image_%d",index));
    ActionHelper::showBackInOut(_image, _image->getPosition(), ActionHelper::ShowDirection::show_from_top,[this,_image,index](){
        _operate->getNodeByName(StringUtils::format("shade%d",index))->setVisible(true);
        int starts[] = {0,1,3};
         startIndex = 1;
        for(int i = starts[index];i<starts[index]+index+1;i++){
            auto _star = _operate->getNodeByName(StringUtils::format("star_%d",i));
            _star->runAction(Sequence::create(DelayTime::create((i-starts[index])*0.35),ScaleTo::create(0, 0.7),CallFunc::create([_star,this](){
                auto _particle = _operate->getNodeByname<ParticleSystemQuad*>("fallStarParticle");
                _particle->setPosition(_star->getPosition());
                _particle->resetSystem();
                  SoundPlayer::getInstance()->playEffect(StringUtils::format( "sound/other/star_%d.mp3",startIndex));
                _star->setVisible(true);
                startIndex++;
            }),EaseBackOut::create(ScaleTo::create(0.2, 1)), CallFunc::create([i,this](){
                switch (i) {
                    case 0:
                        ActionHelper::delayFunc(0.5, this, [this](){ showImage(1);});
                        break;
                    case 2:
                          ActionHelper::delayFunc(0.5, this, [this](){ showImage(2);});
                        break;
                    case 5:{
                        auto crown = _operate->getNodeByName("crown");
                         SoundPlayer::getInstance()->playEffect("sound/Sound/prize_crown.mp3",0.2);
                        ActionHelper::showBackInOut(crown, crown->getPosition(), ActionHelper::ShowDirection::show_from_top,[this,crown](){
                            auto _node = _operate->getNodeByName("Image_2");
                            CocosHelper::reAddNode(crown, _node);
                      
                            float _time = 0.09;
                            Vector<FiniteTimeAction*> actionList;
                            float rote[] =   {0,    11,    11.2,  0, 0,    -11.2,  -11.2, 0};
                            float scaleX[] = {1.03, 1.03,  1.03,  1, 1.03,  1.03,  1.03,  1};
                            float scaleY[] = {1.03, 1.066, 1.114, 1, 1.03,  1.096, 1.114, 1};
                            float seTime[] =  {_time,  _time,   _time,  _time, _time, _time*2  ,_time ,_time};
                            for(int i=0;i<sizeof(rote)/sizeof(rote[0]);i++){
                                actionList.pushBack(Spawn::create(ScaleTo::create(seTime[i],scaleX[i],scaleY[i]),RotateTo::create(seTime[i], rote[i]), nullptr));
                            }
//                                actionList.pushBack(CallFunc::create([_node,i,rote,scaleX,scaleY](){
//                                    _node->setRotation(rote[i]);
//                                    _node->setScale(scaleX[i], scaleY[i]);
//                                }));
//                                if(i==4)
//                                    actionList.pushBack(DelayTime::create(0.2));
//                                else
//                                actionList.pushBack(DelayTime::create(0.1));
//                            }
                            
                            _node->runAction(RepeatForever::create(Sequence::create(Repeat::create(Sequence::create(actionList),2),DelayTime::create(2), nullptr)));
                            
                            auto _ligth = _operate->getNodeByName("light");
                            _ligth->setScale(0,0.8);
                            _ligth->runAction(Sequence::create(ScaleTo::create(0.5, 1,1),CallFunc::create([this](){
                                starParticle->resetSystem();
                                _operate->getNodeByname<ParticleSystemQuad*>("fallParticle")->resetSystem();
                            }), nullptr));
                            
                            _ligth->setVisible(true);
                            auto btn_next = _operate->getNodeByName("btn_next");
                            ActionHelper::showBackInOut(btn_next, btn_next->getPosition(), ActionHelper::ShowDirection::show_from_top);
                            btn_next->setVisible(true);
                            auto btn_reset = _operate->getNodeByName("btn_reset");
                            ActionHelper::showBackInOut(btn_reset, btn_reset->getPosition(), ActionHelper::ShowDirection::show_from_top);
                            btn_reset->setVisible(true);
                            auto btn_ca = _operate->getNodeByName("btn_camer");
                            ActionHelper::showBackInOut(btn_ca, btn_ca->getPosition(), ActionHelper::ShowDirection::show_from_right,[btn_ca](){
                                btn_ca->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9, 1.1),ScaleTo::create(0.9, 1), nullptr)));
                            });
                            btn_ca->setVisible(true);
                        });
                        crown->setVisible(true);
                    }
                        break;
                    default:
                        break;
                }
            }),nullptr));
        }
    },0.6);
    _image->setVisible(true);
}
 void VoteEndStep::onExitTransitionDidStart(){
     FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
     BaseStepLayer::onExitTransitionDidStart();
}

void VoteEndStep::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
    bool idOnlyOne = true;
    if(playIndex == -1){
        playIndex = UserDefault::getInstance()->getIntegerForKey("playIndex",-1);
        if(playIndex == -1){
        playIndex = 2;
        int flag = rand()%10;
        if(flag<4)
            playIndex = 1;
        }
        idOnlyOne = false;
    }
    
    map<char,string> _paths;
    _paths.insert(map<char,string>::value_type('1',"ui/clock/1.png"));
     _paths.insert(map<char,string>::value_type('2',"ui/clock/2.png"));
     _paths.insert(map<char,string>::value_type('3',"ui/clock/3.png"));
    _timeLabel = ImageLabel::create(_paths);
    auto _clock = _operate->getNodeByName("clock_1");
    _timeLabel->setPosition(_clock->getContentSize()*.5);
    _timeLabel->setPositionY(_timeLabel->getPositionY()-80);
    _timeLabel->setText("3");
    _clock->addChild(_timeLabel);
    auto _endRote = 360-83.45;
    auto _clockPoint = _operate->getNodeByName("clockPointer");
    auto _roteAction = RotateBy::create(1.6, _endRote/3.0f);
    _clockPoint->runAction(Sequence::create(_roteAction->clone(),CallFunc::create([this](){_timeLabel->setText("2");}), _roteAction->clone(),CallFunc::create([this](){_timeLabel->setText("1");}),_roteAction->clone(),CallFunc::create([this,idOnlyOne](){
        _operate->getNodeByName("timerLayer")->setVisible(false);
    
        if(idOnlyOne)
             showImage(2);
        else
           showImage(0);
    }),nullptr));

   
    vector<string> voteLayers;
    for(int i=0;i<=19;i++)
        voteLayers.push_back(StringUtils::format("png/donuts/donuts%d.png",i));
    
    for(int i=0;i<3;i++) {
        auto _image = _operate->getNodeByName(StringUtils::format("Image_%d",i));
        if(i==playIndex) {
            auto _path = FileUtils::getInstance()->fullPathForFilename(FileUtils::getInstance()->getWritablePath() + "eatFood.png");
            if(FileUtils::getInstance()->isFileExist(_path)){
                dynamic_cast<Sprite*>(_image->getChildByName("image"))->setSpriteFrame(CocosHelper::spriteCreateWithNoCache(_path.c_str())->getSpriteFrame());
            }
        } else {
            auto _inner = _image->getChildByName("image");
            _inner->setCascadeOpacityEnabled(false);
            _inner->setOpacity(0);
            auto _itor = voteLayers.begin()+rand()%voteLayers.size();
            auto _layer = Sprite::create(*_itor);
            _layer->setScale(2.2);
            _layer->setPosition(_inner->getContentSize()*.5);
            _inner->addChild(_layer,4);
            voteLayers.erase(_itor);
        }
    }
    
 
}