//
//  MakeNodeManager.h
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#ifndef MakeNodeManager_h
#define MakeNodeManager_h

#include "cocos2d.h"
#include "component/actions/ActionHelper.h"
using namespace cocos2d;

class MakeNodeManager
{
public:
    static MakeNodeManager* getInstance();

    //remove event
    void registerRemoveNode(Node *node, ActionDirection direction);
    void unRegisterRemoveNode(Node *node);
    void removeAllRegisterNodes(const std::function<void()>& callback = nullptr);
    
public:
    MakeNodeManager():
    _callOnce(false)
    {};
    virtual ~MakeNodeManager();
    
protected:
    void startup();
    Vector<Node*> _removeNodesVector;
    std::unordered_map<Node*, ActionDirection> _removeNodesMap;
protected:
    bool _callOnce;
};

#endif /* MakeNodeManager_h */
