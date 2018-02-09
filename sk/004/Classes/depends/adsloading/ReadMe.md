[使用方法]

将项目作为外链添加到Classes目录下，在项目中添加引用即可。
 
[V1.2版本更新内容]
 
1. AdLoadLayerBase适配新版广告(AdsSDK1.0);
 
2. 去掉包含STAds，其功能用公共库AdsManager代替。
 
3. 文件存在目录变更:以前的AdCross, AdJni, AdIos都去掉了，仅剩下AdLoadingLayerBase.h 及 .cpp;所以更新外链时，可能需要你重新import到工程。
 
4. **调用接口没有变化**。

 