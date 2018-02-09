//
//  Configure.h
//  MLABS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/29.
//
//

#ifndef __MLABS1044_BeachFoodParty__Configure__
#define __MLABS1044_BeachFoodParty__Configure__

#include <stdio.h>

//对话框显示文字
#define ExitGame   "Are you sure you want to quite game?"
#define ReturnHome "Are you sure you want to start over?"

//创建Scene的宏
#define CREATE_SCENE(__TYPE__) \
static Scene* createScene() \
{ \
Scene* scene = Scene::create(); \
__TYPE__* layer = __TYPE__::create(); \
scene->addChild(layer); \
return scene; \
}

#endif /* defined(__MLABS1044_BeachFoodParty__Configure__) */
