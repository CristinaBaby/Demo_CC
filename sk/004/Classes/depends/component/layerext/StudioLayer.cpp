//
//  StudioLayer.cpp
//  CodeBase
//
//  Created by zhangguangzong1 on 12/1/14.
//
//

#include "depends/component/layerext/StudioLayer.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
#include "depends/component/audio/CMAudioUtil.h"

StudioLayer::StudioLayer():_rootNode(nullptr),_uiAction(nullptr),_bgSprite(nullptr)
{
}

StudioLayer::~StudioLayer()
{
}

StudioLayer* StudioLayer::create(const string& jsonFile)
{
    StudioLayer* layer = new StudioLayer();
    if (layer && layer->initWithFile(jsonFile))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return layer;
}

bool StudioLayer::initWithFile(const string& jsonFile)
{
    if (!CFLayer::init())
    {
        return false;
    }
    
    _rootNode = GUIReader::getInstance()->widgetFromJsonFile(jsonFile.c_str());
    if (_rootNode)
    {
        this->addToUILayer(_rootNode,kUI);
    }
    
    return true;
}

StudioLayer* StudioLayer::createWithCSB(const string& csbFile)
{
    StudioLayer* layer = new StudioLayer();
    if (layer && layer->initWithCSBFile(csbFile))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return layer;
}

bool StudioLayer::initWithCSBFile(const string& csbFile)
{
    if (!CFLayer::init())
    {
        return false;
    }
    
    _rootNode = CSLoader::createNode(csbFile);
    if (_rootNode)
    {
        _rootNode->setContentSize(Director::getInstance()->getVisibleSize());
        ui::Helper::doLayout(_rootNode);
        this->addToUILayer(_rootNode,kUI);
        
        this->setClickForBtns(_rootNode);
        this->setClickForCheckBoxs(_rootNode);
        
        /*加载动画*/
        _uiAction = CSLoader::createTimeline(csbFile, _rootNode);
        if(_uiAction)
        {
            _rootNode->runAction(_uiAction);
        }
    }
    
    return true;
}

void StudioLayer::setBackground(const string &path)
{
    if (_bgSprite)
    {
        _bgSprite->setTexture(path);
    }
    else
    {
        _bgSprite = Sprite::create(path);
        CMVisibleRect::setPositionAdapted(_bgSprite, CMVisibleRect::getDesignCenter());
        this->addToBgLayer(_bgSprite);
    }
}

void StudioLayer::setClickForBtns(Node* root,Widget::ccWidgetTouchCallback callback)
{
    if (!root)
        return;
    Button* rootBtn = dynamic_cast<Button*>(root);
    if(rootBtn)
    {
        if (callback)
        {
            rootBtn->addTouchEventListener(callback);
        }
        else
        {
            rootBtn->addTouchEventListener(CC_CALLBACK_2(StudioLayer::onClick, this));
        }
    }
    
    for (auto child: root->getChildren())
    {
        this->setClickForBtns(child,callback);
        Button* pNode = dynamic_cast<Button*>(child);
        if(pNode)
        {
            if (callback)
            {
                pNode->addTouchEventListener(callback);
            }
            else
            {
                pNode->addTouchEventListener(CC_CALLBACK_2(StudioLayer::onClick, this));
            }
        }
    }
}

void StudioLayer::onClick(Ref* ref,Widget::TouchEventType type)
{
    if (_fnBtnCallback)
    {
        _fnBtnCallback(ref,type);
    }
    else
    {
        this->onButtonClick(ref, type);
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        Node* node = dynamic_cast<Node*>(ref);
        if (node)
            this->playEffectByNode(node);
    }
}

void StudioLayer::onButtonClick(Ref* ref,Widget::TouchEventType type)
{
    log("do something here...");
}

void StudioLayer::setClickForCheckBoxs(Node* root,CheckBox::ccCheckBoxCallback callback)
{
    if (!root)
        return;
    CheckBox* rootCheckbox = dynamic_cast<CheckBox*>(root);
    if(rootCheckbox)
    {
        if (callback)
        {
            rootCheckbox->addEventListener(callback);
        }
        else
        {
            rootCheckbox->addEventListener(CC_CALLBACK_2(StudioLayer::onCBClick,this));
        }
    }
    
    for (auto child: root->getChildren())
    {
        this->setClickForCheckBoxs(child,callback);
        CheckBox* pNode = dynamic_cast<CheckBox*>(child);
        if(pNode)
        {
            if (callback)
            {
                pNode->addEventListener(callback);
            }
            else
            {
                pNode->addEventListener(CC_CALLBACK_2(StudioLayer::onCBClick, this));
            }
        }
    }
}

void StudioLayer::onCBClick(Ref* ref,CheckBox::EventType type)
{
    if(_fnCbCallback)
    {
        _fnCbCallback(ref,type);
    }
    else
    {
        this->onCheckBoxClick(ref, type);
    }

    Node* node = dynamic_cast<Node*>(ref);
    if (node)
        this->playEffectByNode(node);
}

void StudioLayer::onCheckBoxClick(Ref*,CheckBox::EventType)
{
    log("do something here...");
}

void StudioLayer::setMusicClick(const string& name)
{
    CheckBox* pSoundCheck = dynamic_cast<CheckBox*>(Helper::seekNodeByName(_rootNode,name));
    if(pSoundCheck)
    {
        pSoundCheck->addEventListener( [=](Ref* ref,CheckBox::EventType type)
                                      {
                                          kAudioUtil->switchAllAudio();
                                          this->playEffectByNode((Node*)ref);
                                      });
        pSoundCheck->setSelected(!kAudioUtil->getAudioState());
    }
}

void StudioLayer::setMusicClick(const string& name,const string& sound)
{
    CheckBox* pSoundCheck = dynamic_cast<CheckBox*>(Helper::seekNodeByName(_rootNode,name));
    if(pSoundCheck)
    {
        pSoundCheck->addEventListener( [=](Ref*,CheckBox::EventType type)
                                      {
                                          kAudioUtil->switchAllAudio();
                                          kAudioUtil->playEffect(sound.c_str());
                                      });
        pSoundCheck->setSelected(!kAudioUtil->getAudioState());
    }
}

void StudioLayer::setBtnClickCall(Node* parent,Widget::ccWidgetTouchCallback call)
{
    if (!parent)
    {
        return;
    }
    
    for (auto child: parent->getChildren())
    {
        Button* pNode = dynamic_cast<Button*>(child);
        if(pNode)
            pNode->addTouchEventListener(call);
    }
}

void StudioLayer::playEffectByNode(Node* node)
{
    std::string audioName = StringUtils::format("%s%s",node->getName().c_str(),AUDIO_SUFFIX);
    Node* audioNode = ui::Helper::seekNodeByName(node, audioName);
    if (audioNode)
    {
        auto audio = dynamic_cast<ComAudio*>(audioNode->getComponent(audioName));
        if (kAudioUtil->getAudioState())
        {
            audio->playEffect();
        }
    }
}
