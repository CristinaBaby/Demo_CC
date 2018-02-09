//
//  MakeNodeManager.cpp
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#include "MakeNodeManager.h"

static MakeNodeManager *s_Singleton = nullptr;

MakeNodeManager::~MakeNodeManager()
{
    
}

MakeNodeManager* MakeNodeManager::getInstance()
{
    if (!s_Singleton) {
        s_Singleton = new (std::nothrow)(MakeNodeManager);
    }
    
    return s_Singleton;
}

void MakeNodeManager::startup()
{
    
}

void MakeNodeManager::registerRemoveNode(Node *node, ActionDirection direction)
{
    _removeNodesVector.pushBack(node);
    _removeNodesMap.insert(std::make_pair(node, direction));
}

void MakeNodeManager::unRegisterRemoveNode(Node *node)
{
    auto iterator = std::find(_removeNodesVector.begin(), _removeNodesVector.end(), node);
    if (iterator != _removeNodesVector.end())
        _removeNodesVector.erase(iterator);
    
    _removeNodesMap.erase(node);
}

void MakeNodeManager::removeAllRegisterNodes(const std::function<void()>& callback)
{
    _callOnce = false;
    for (auto node : _removeNodesVector)
    {
        ActionDirection direction = _removeNodesMap.at(node);
        auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
        Vec2 topLeft =   rect.origin + Vec2(0,rect.size.height);
        Vec2 bottomRight = rect.origin + Vec2(rect.size.width,0);
        Vec2 curentPosition = node->getPosition();
        Vec2 endPosiont;
        
        switch (direction) {
            case ActionDirection::kActionDirectionBottom:
                endPosiont = Vec2(curentPosition.x,bottomRight.y-node->getContentSize().height*(1-node->getAnchorPoint().y));
                break;
            case ActionDirection::kActionDirectionLeft:
                endPosiont = Vec2(topLeft.x -node->getContentSize().width*(1-node->getAnchorPoint().x), curentPosition.y);
                break;
            case ActionDirection::kActionDirectionRight:
                endPosiont = Vec2(rect.size.width + node->getContentSize().width/2, curentPosition.y);
                break;
            case ActionDirection::kActionDirectionTop:
                endPosiont = Vec2(curentPosition.x, topLeft.y +node->getContentSize().width*(node->getAnchorPoint().y));
                break;
            default:
                break;
        }

        auto moveAction = Sequence::create(
                                           MoveTo::create(1.0, endPosiont),
                                           CallFunc::create([=] {
            node->removeFromParent();
            if (!_callOnce && callback)
            {
                _callOnce = true;
                callback();
            }
        }), nullptr);
        node->runAction(moveAction);
    }
    
    _removeNodesVector.clear();
}



