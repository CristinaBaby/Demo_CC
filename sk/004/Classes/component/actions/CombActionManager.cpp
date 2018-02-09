//
//  CombActionManager.cpp
//
//  Created by tanshoumei on 15/5/15.
//
//

#include "component/actions/CombActionManager.h"

NS_CombAction_BEGIN

CombActionManager::~CombActionManager()
{
    unregisterAllAction();
}

void CombActionManager::registerAction(CombActionNS::CombActionBase *apAction, CombActionTriggerType aeType, std::string asParam)
{
    if(apAction == nullptr)
        return;
    
    apAction->setTriggerType(aeType);
    apAction->setTriggerParam(asParam);
    apAction->start();
    _vActions.pushBack(apAction);
    apAction->retain();
}

void CombActionManager::unregisterAction(CombActionNS::CombActionBase *apAction)
{
    _vActions.eraseObject(apAction);
    apAction->release();
}

void CombActionManager::unregisterAllAction()
{
    for (auto& action: _vActions)
    {
        action->release();
    }
    _vActions.clear();
}

NS_CombAction_END
