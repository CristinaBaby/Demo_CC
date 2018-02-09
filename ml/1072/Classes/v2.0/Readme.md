本类库用于针对位图进行涂抹、填色和涂色。
使用方法及效果请参考Demo：
ssh://git@stash.stm.com:7999/cmr/scribbledemo.git

1. Spine动画涂抹
Cocos2d－x 3.8支持Spine动画涂抹，如需使用，请按照如下方式配置，Cocos2d－x 3.2暂不支持。
(1)iOS配置
在XCode Build Settings的Preprocessor Macros加上宏定义SCRIBBLE_USE_SPINE=1


(2) Application.mk配置加上SCRIBBLE_USE_SPINE宏，如下

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_CPPFLAGS += -DSCRIBBLE_USE_SPINE=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_CPPFLAGS += -DSCRIBBLE_USE_SPINE=1
  APP_OPTIM := release
endif

renhong