//
//  StickerDrawNode.cpp
//  ccats1061
//
//  Created by liji on 16/8/23.
//
//

#include "StickerDrawNode.h"
#include "ComponentCycleScroll.h"
#include "ScribbleSauce.h"
#include "CMVisibleRect.h"
#include "ComponentTouchMove.h"

bool StickerDrawNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }
    
    {
        auto layer = LayerColor::create(Color4B(0, 0, 0, 0));
        addChild(layer, 1);
        
        //屏蔽事件
        auto eventListener = EventListenerTouchOneByOne::create();
        eventListener->setSwallowTouches(true);
        eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    }

    
    auto bg = Sprite::create("ui/draw/bg.png");
    bg->setPosition(480, 366);
    this->addChild(bg,3);

    auto penNode = Node::create();
    penNode->setPosition(480, 142+100);
    this->addChild(penNode,1);
    
    ScribbleSauce* drawNode = ScribbleSauce::create(Size(394-70,221+10));
    drawNode->setBrush(_director->getTextureCache()->addImage("res/hole_stencilsamllpen.png"));
    bg->addChild(drawNode,10);
    drawNode->setPosition(Vec2(64-30,60-20));

    drawNode->callBack = [=](Touch* t,ScribbleSauce::touchType type)
    {
        if (type == ScribbleSauce::touchType::touchMove)
        {
            drawNode->paint(t->getLocation());
            Vec2 delta = CMVisibleRect::getDesignOffset();
            if (_pen)
                _pen->setPosition(t->getLocation()-delta);
            log("t->getLocation() %f,%f",t->getLocation().x,t->getLocation().y);
        }
    };
    
    Sprite* scrollBg = Sprite::create("ui/draw/penci_bg.png");
    penNode->addChild(scrollBg);

    auto cCycle = ComponentCycleScroll::create();
    cCycle->setEnableMoveItem(false);
    cCycle->setCycleScrollSize(Size(420,110));
    cCycle->setCycleScrollDirection(ScrollView::Direction::HORIZONTAL);
    cCycle->setMargin(110);
    penNode->addComponent(cCycle);
    
    for (int index = 1; index <= 6; ++index)
    {
        auto file = StringUtils::format("ui/draw/pencil%d.png", index);
        auto icon = ImageView::create(file);
        auto widget = Widget::create();
        widget->setContentSize(icon->getContentSize());
        icon->setPosition(widget->getContentSize()/2);
        widget->addChild(icon, 2);
        widget->setTag(index);
        cCycle->pushCustomItem(widget);
    }
    cCycle->itemClickCallback = [=](Widget *item, Touch *touch)
    {
        int index = item->getTag();
        drawNode->setDrawColor(_colorVec.at(index-1));
        drawNode->setDrawing(true);
        
        if (!_pen)
        {
            _pen = Sprite::create(StringUtils::format("ui/draw/pencil%d.png", index));
            this->addChild(_pen,INT_MAX);
            _pen->setPosition(1900, 320);
            _pen->runAction(MoveTo::create(0.4f, Vec2(650, 350)));
            _pen->setAnchorPoint(Vec2(0.18, 0.1));
            
        }
        _pen->setTexture(StringUtils::format("ui/draw/pencil%d.png", index));
        
    };
    cCycle->start();

    Button* confirmBtn = Button::create("ui/draw/OK_btn.png");
    confirmBtn->cocos2d::Node::setPosition(697-70, 254);
    this->addChild(confirmBtn,10,1);
    _btnVec.push_back(confirmBtn);
    
    Button* resetBtn = Button::create("ui/draw/delete_btn.png");
    resetBtn->cocos2d::Node::setPosition(313, 520-40);
    this->addChild(resetBtn,10,2);
    _btnVec.push_back(resetBtn);

    Button* closeBtn = Button::create("ui/draw/close_btn.png");
    closeBtn->cocos2d::Node::setPosition(663,520-40);
    this->addChild(closeBtn,10,3);
    _btnVec.push_back(closeBtn);
    
    bg->setScale(0);
    bg->runAction(EaseBounceOut::create(ScaleTo::create(0.8f, 1)));
    
    penNode->runAction(Sequence::create(Hide::create(),
                                        DelayTime::create(0.8f),
                                        Show::create(),
                                        MoveBy::create(0.3f, Vec2(0, -100)),
                                        CallFunc::create([=]
    {
        cCycle->scrollBy(0.3f, Vec2(200, 0), nullptr);
    }),
                                        NULL));
    for (auto btn: _btnVec)
    {
        btn->setScale(0);
        btn->runAction(Sequence::create(DelayTime::create(0.8f),
                                        EaseBackOut::create(ScaleTo::create(0.2f, 1)),
                                        NULL));
    }

    

    auto call = [=](Ref* ref,Widget::TouchEventType type)
    {
        auto btn = dynamic_cast<Button*>(ref);
        if (!_enable)
            return;
        
        
        if (type == Widget::TouchEventType::ENDED)
        {
            switch (btn->getTag())
            {
                case 1:
                {
                    float duration = 1.2;

                    auto call1 = [=](RenderTexture* rtx, const std::string& name)
                    {
                        
                        this->runAction(Sequence::create(DelayTime::create(duration),
                                                         DelayTime::create(0.4f),
                                                         CallFunc::create([=]
                        {
                            if (_confirmBtnClicked)
                                _confirmBtnClicked();
                        }),
                                                         RemoveSelf::create(),
                                                         NULL));

                    };
                    float disY = -160;
                    if (_screenName == "paint_lunchbox.png")
                    {
                        disY = 40;
                    }
                    
                    drawNode->saveScribble(_screenName,call1);
                    bg->runAction(Sequence::create(DelayTime::create(0.4),
                                                   EaseBackIn::create(Spawn::create(
                                                                                     MoveBy::create(duration, Vec2(-20, disY)),
                                                                                     ScaleTo::create(duration, 0),
                                                                                     NULL)),
                                                   NULL));
                    _enable = false;

                    for (auto btn: _btnVec)
                    {
                        btn->runAction(EaseElasticIn::create(ScaleTo::create(0.3f, 0), 1.5));
                    }
                    
                    scrollBg->setVisible(false);
                    drawNode->setDrawing(false);
                    penNode->setVisible(false);
                    if (_pen)
                        _pen->setVisible(false);
                    cCycle->setIgnoreAllTouch(true);

                }
                    break;
                case 2:
                {
                    drawNode->reset();
                }
                    break;
                case 3:
                {
                    float duration = 0.7;
                    float disY = -160;
                    if (_screenName == "paint_lunchbox.png")
                    {
                        disY = 40;
                    }

                    bg->runAction(Sequence::create(DelayTime::create(0.4),
                                                   EaseBackIn::create(Spawn::create(
                                                                                    MoveBy::create(duration, Vec2(-20, disY)),
                                                                                    ScaleTo::create(duration, 0),
                                                                                    NULL)),
                                                   CallFunc::create([=]
                    {
                        if (_closeBtnClicked)
                            _closeBtnClicked();
                    }),
                                                   TargetedAction::create(this, RemoveSelf::create()),
                                                   NULL));
                    _enable = false;
                    
                    for (auto btn: _btnVec)
                    {
                        btn->runAction(EaseElasticIn::create(ScaleTo::create(0.3f, 0), 1.5));
                    }
                    
                    scrollBg->setVisible(false);
                    drawNode->setDrawing(false);
                    penNode->setVisible(false);
                    if (_pen)
                        _pen->setVisible(false);
                    cCycle->setIgnoreAllTouch(true);
                }
                    break;
                default:
                    break;
            }
        }
    };
    closeBtn->addTouchEventListener(call);
    confirmBtn->addTouchEventListener(call);
    resetBtn->addTouchEventListener(call);

    return true;
}

void StickerDrawNode::onEnter()
{
    MakeBaseNode::onEnter();

}

void StickerDrawNode::onExit()
{
    MakeBaseNode::onExit();

}

