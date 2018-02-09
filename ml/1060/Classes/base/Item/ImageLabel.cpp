//
//  ImageLabel.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/13.
//
//

#include "ImageLabel.h"
using namespace ui;

ImageLabel::ImageLabel():
    nodes(nullptr)
    ,_container(nullptr)
    ,_direction(ImageLabel::Direction::center)
,_space(0)
{
    this->ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(Size::ZERO);
}

ImageLabel::~ImageLabel(){
    CC_SAFE_DELETE(nodes);
}

ImageLabel* ImageLabel::create(const map<char,string>& paths){
    auto _label = new ImageLabel();
    if(_label&&_label->init()){
        _label->setNodesPath(paths);
        _label->autorelease();
    } else 
        CC_SAFE_RELEASE_NULL(_label);
        return _label;
}

 ImageLabel* ImageLabel::createWithPlist(const string& _fileNames){
     auto _label = new ImageLabel();
     if(_label&&_label->init()){
         _label->setPlistFile(_fileNames);
        
         _label->autorelease();
     } else
         CC_SAFE_RELEASE_NULL(_label);
     return _label;
}

void ImageLabel::setPlistFile(const string& _fileNames){
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_fileNames);
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(_fileNames);
    if (fullPath.size() != 0)
    {
        ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
        auto _itor = dict.find("frames");
        if(_itor != dict.end()){
            auto paths = _itor->second.asValueMap();
            for(auto _path:paths){
                auto _key = _path.first;
                addNode(_key.at(_key.size()-5),Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_key)));
            }
        }
    }
}

    void ImageLabel::setNodes(const Map<char,Sprite*>& _map){
        if(nodes == nullptr)
            nodes = new Map<char,Sprite*>();
        else 
            nodes->clear();
        *nodes = _map;
    }
    void ImageLabel::setNodesPath(const map<char,string>& paths){
        for(auto itor = paths.begin();itor != paths.end();itor++) {
            auto s = Sprite::create(itor->second);
            addNode(itor->first,s);
        }
    }

    void ImageLabel::addNode(char key,Sprite* node) {
        if(nodes == nullptr)
            nodes = new Map<char,Sprite*>();
        nodes->insert(key,node);
    }

    void ImageLabel::setText(const string& s){
        if(s.empty()|| nodes==nullptr)
            return;
        if(_container == nullptr) {
            _container = Node::create();
            addChild(_container);
            _container->ignoreAnchorPointForPosition(false);
            _container->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        }

        _container->removeAllChildren();
        auto _data = s.data();
        float _width(0),height(-1);
        for (int i = 0; i < s.size(); i++)
        {
            auto _itor = nodes->find(_data[i]);
            if(_itor != nodes->end()){
                auto s = Sprite::createWithSpriteFrame( _itor->second->getSpriteFrame());
                if(height<s->getContentSize().height)
                    height = s->getContentSize().height;
                s->setPositionX(_width+_itor->second->getContentSize().width*.5);
                _width += s->getContentSize().width+_space;
                _container->addChild(s);
            }
        }
        _width = _width-_space;
        _container->setContentSize(Size(_width,height));
        for(auto _child:_container->getChildren()){
            _child->setPositionY(height*.5);
        }
        auto _lableSize = this->getContentSize();
        switch (_direction)
        {
        case ImageLabel::Direction::center:
            _container->setPosition(_lableSize*.5);
            _container->setAnchorPoint(Vec2(0.5,0.5));
            break;
        case ImageLabel::Direction::right:
            _container->setPosition(_lableSize.width,_lableSize.height*.5);
            _container->setAnchorPoint(Vec2(1,0.5));
            break;
        case ImageLabel::Direction::left:
                _container->setPosition(0,_lableSize.height*.5);
            _container->setAnchorPoint(Vec2(0,0.5));
            break;
        default:
            _container->setPosition(_lableSize*.5);
            _container->setAnchorPoint(Vec2(0.5,0.5));
            break;
        }
        
    }