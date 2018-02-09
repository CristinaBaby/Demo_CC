VersionChecker

1.0.0 - 2013.10.09
------------------------
介绍：
  1.启动一个app，当这个app有比现在更新的版本可供更新时，弹出一个pop up窗口，询问用户是否更新到最新版。
  2. New Version Detector的弹窗，与News Blast的弹窗相互排斥，不同时出现。 New Version Detector的优先级高于News Blast。 也就是说：
    启动app时，先检测这个app是都有new version
    如果这个app有更新的版本，则弹出New Version Available的弹窗，同时不再弹出任何News Blast的弹窗
    如果这个app没有New Version updated，就正常弹出News Blast的弹窗

集成到项目：参看VersionChecker/trunk/VersionDectorDemo这个demo。

