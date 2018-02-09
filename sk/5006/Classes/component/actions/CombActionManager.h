//
//  CombActionManager.h
//
//  Created by tanshoumei on 15/5/15.
//
//

#ifndef CombActionManager_H
#define CombActionManager_H

#include <stdio.h>
#include "component/actions/CombActionHeader.h"
#include "component/actions/ActionMacro.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

NS_CombAction_BEGIN

class CombActionManager: public Ref{
public:
    ~CombActionManager();
    
    void registerAction(CombActionBase *apAction, CombActionTriggerType aeType, std::string asParam);
    
    void unregisterAction(CombActionBase *apAction);
    
    void unregisterAllAction();
    
private:
    void _checkActionState();
    
protected:
    Vector<CombActionBase*> _vActions;
};

NS_CombAction_END

#endif

