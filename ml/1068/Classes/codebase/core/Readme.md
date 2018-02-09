**当前版本：v1.0.2_v3.8.1**

**最后维护更新:  	张光宗**

#简介：

最底层工具类，适用于所有项目

#依赖外链：

ssh://git@stash.stm.com:7999/cmc/crossplatformapi.git

#主要内容：

utils目录下：
1.CSVParse类             用于解析CSV文件；

2.FileUtility类          文件操作类，可用于创建，删除以及遍历文件夹；

3.FrameWorkHelper类      当前只有一个方法，操作字符串，按指定格式拆分；


common目录下：

CMVisibleRect类		适配管理类；

AudioUtil类		音效播放控制类；


platform目录下：

header文件夹下存放的是项目中需要到的头文件，包括（ads,iap,systemfunction）

cross_code文件下存放的是header头文件中android和IOS各自平台的实现

特别注意：在IOS工程中需要删除cross_code/android文件夹的引用


#修改内容：
AudioUtil修改：

1.AudioUtil更名为CMAudioUtil

2.添加通过音量关闭音效的方法

3.原public变量m_bAllAudio通过新方法`getAudioState()`获取



**简单的使用语句如下：**

```
......

......

......
```

**更多使用方式和效果详情参见代码库Demo！**

CodeBaseDemo：  [ssh://git@stash.stm.com:7999/cmr/codebasedemo.git](ssh://git@stash.stm.com:7999/cmr/codebasedemo.git)