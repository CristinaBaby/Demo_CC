//
//  ActionController.cpp
//  Make
//
//  Created by liji on 15/6/5.
//
//

#include "ActionController.h"


void ActionController::popUp(float pDelaytime, Node* pNode)
{
    pNode -> runAction(Sequence::create(DelayTime::create(pDelaytime),
                                        ScaleTo::create(0.2, 1.2),
                                        ScaleTo::create(0.2, 1),
                                        NULL));
}