//
//  ComponentCycleDisplay.cpp
//  BHUG1062
//
//  Created by liji on 16/4/19.
//
//

#include "ComponentCycleDisplay.hpp"


void ComponentCycleDisplay::start()
{
    float duration = 1.5;
    
    Vec2 move1;
    Vec2 move2;
    
    if(_isFirst)
    {
        switch (_direction)
        {
            case kDirectionLeftToRight:
            {
                move1 = Vec2(_size.width, 0);
                move2 = Vec2(_orPos.x-_size.width,_orPos.y);
            }
                break;
            case kDirectionRightToLeft:
            {
                move1 = Vec2(-_size.width, 0);
                move2 = Vec2(_orPos.x+_size.width,_orPos.y);
            }
                break;
            case kDirectionBottomToUp:
            {
                move1 = Vec2(0, _size.height);
                move2 = Vec2(_orPos.x,_orPos.y-_size.height);
            }
                break;
            case kDirectionUpToBottom:
            {
                move1 = Vec2(0, -_size.height);
                move2 = Vec2(_orPos.x,_orPos.y+_size.height);
            }
                break;
                
            default:
                break;
        }
        
        _orSprite->runAction(RepeatForever::create(Sequence::create(MoveBy::create(duration, move1),
                                                                    Place::create(_orPos),
                                                                    NULL)));
        _tokenSprite->runAction(RepeatForever::create(Sequence::create(MoveBy::create(duration, move1),
                                                                       Place::create(move2),
                                                                       NULL)));
        
        _isFirst = false;
    }
    else
    {
        this->resume();
    }

}


void ComponentCycleDisplay::setName(const std::string& name)
{
    Component::setName(name);
    _tokenSprite->setName(name);

}


void ComponentCycleDisplay::onAdd()
{
    _orSprite = dynamic_cast<Sprite*>(getOwner());
    auto parent = _orSprite->getParent();
    _orPos = _orSprite->getPosition();
    _size = _orSprite->getContentSize();

    _tokenSprite = Sprite::createWithTexture(_orSprite->getTexture());
    parent->addChild(_tokenSprite,_orSprite->getZOrder());
    _tokenSprite->setName(kToken);
    _tokenSprite->setPosition(_orPos.x-_size.width,_orPos.y);

    

}

void ComponentCycleDisplay::setDirection(kMoveDirection direction)
{
    _direction = direction;
    
    switch (_direction)
    {
        case kDirectionLeftToRight:
        {
            _tokenSprite->setPosition(_orPos.x-_size.width,_orPos.y);
        }
            break;
        case kDirectionRightToLeft:
        {
            _tokenSprite->setPosition(_orPos.x+_size.width,_orPos.y);
        }
            break;
        case kDirectionBottomToUp:
        {
            _tokenSprite->setPosition(_orPos.x,_orPos.y-_size.height);
        }
            break;
        case kDirectionUpToBottom:
        {
            _tokenSprite->setPosition(_orPos.x,_orPos.y+_size.height);
        }
            break;
            
        default:
            break;
    }

}



void ComponentCycleDisplay::stop()
{
    _orSprite->stopAllActions();
    _tokenSprite->stopAllActions();
    
    _orSprite->setPosition(_orPos);
    
    Vec2 pos;
    switch (_direction)
    {
        case kDirectionLeftToRight:
        {
            pos = Vec2(_orPos.x-_size.width,_orPos.y);
        }
            break;
        case kDirectionRightToLeft:
        {
            pos = Vec2(_orPos.x+_size.width,_orPos.y);
        }
            break;
        case kDirectionBottomToUp:
        {
            pos = Vec2(_orPos.x,_orPos.y-_size.height);
        }
            break;
        case kDirectionUpToBottom:
        {
            pos = Vec2(_orPos.x,_orPos.y+_size.height);
        }
            break;
            
        default:
            break;
    }
    
    _tokenSprite->setPosition(pos);
}

void ComponentCycleDisplay::pause()
{
    Director::getInstance()->getActionManager()->pauseTarget(_orSprite);
    Director::getInstance()->getActionManager()->pauseTarget(_tokenSprite);
    _tokenSprite->setVisible(_orSprite->isVisible());
    _tokenSprite->setScale(_orSprite->getScale());
}

void ComponentCycleDisplay::resume()
{
    Director::getInstance()->getActionManager()->resumeTarget(_orSprite);
    Director::getInstance()->getActionManager()->resumeTarget(_tokenSprite);
    _tokenSprite->setVisible(true);
    _tokenSprite->setScale(_orSprite->getScale());
}