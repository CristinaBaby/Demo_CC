//
//  ActionController.h
//  Make
//
//  Created by liji on 15/6/5.
//
//

#ifndef __Make__ActionController__
#define __Make__ActionController__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class ActionController : public Ref
{
public:
    
    static void popUp(float pDelaytime, Node* pNode);

};

#endif /* defined(__Make__ActionController__) */
