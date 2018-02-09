**当前版本：v1.0.0**

**作者:    		张光宗**

**最后维护更新:  	张光宗**

#简介：

#该目录下的文件对应在项目中的存放路径为：

#Classes/depends/component/layerext

#依赖外链：

AudioUtil:  [ssh://git@stash.stm.com:7999/cml/audioutil.git](ssh://git@stash.stm.com:7999/cml/audioutil.git)

ScreenAdapter:  [ssh://git@stash.stm.com:7999/cml/screenadapter.git](ssh://git@stash.stm.com:7999/cml/screenadapter.git)


#主要内容：

CFLayer: 继承自Layer与KeypadDispatchCenter，把一个layer分为了3层，背景层，内容层和UI层，并且可响应Android的返回键。

StudioLayer:继承自CFLayer，用一个cocos studio的UI文件来初始化一个界面。

keypaddispatchcenter:Android返回键监听管理类。



**简单的使用语句如下：**

```
…….
…….
…….
```

**更多使用方式和效果详情参见代码库Demo！**