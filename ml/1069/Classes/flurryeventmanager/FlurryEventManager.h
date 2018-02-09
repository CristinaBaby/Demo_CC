//
//  FlurryEventManager.h
//
//
//

#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
using namespace std;


//timed, recordInPath, param
class FlurryEventManager
{
public:
    static FlurryEventManager* getInstance();
    
#pragma mark --这里是直接适配flurry现在功能的接口
    /*
     *  普通的event，用于统计某事件，调一次其计数会加1.
     */
    int logEvent(const string name);
    
    /*
     *  带计时功能的event，用于统计某事件，调一次其计数会加1.务必调用下面的logEventTimedEnd，用于结束事件计时.
     */
    int logEventTimed(const string name);
    
    /*
     *  事件计时结束。和logEventTimed成对调用。
     */
    void logEventTimedEnd(const string name);
    
    /*
     *  带一个参数的event。
     */
    int logEventWithParam(const string eventName, const string key, const string value);
    
    /*
     *  带多个参数(map)的event
     */
    int logEventWithParam(const string eventName, map<string, string> mapParam);
    
    /*
     *  带计时功能，并带一个参数的event. 请务必在表示事件结束的时候调用logEventTimedEndWithParam
     */
    int logEventTimedWithParam(const string eventName, const string key, const string value);
    
    /*
     *  带计时功能，带多个参数(map)的event. 请务必在表示事件结束的时候调用logEventTimedEndWithParam
     */
    int logEventTimedWithParam(const string eventName, map<string, string> mapParam);
    
    /*
     *  事件计时结束。和logEventTimedWithParam成对调用。
     *  @prama key, value。可用于更新Event开始时的值，没有可以为"".或调用logEventTimedEnd
     */
    void logEventTimedEndWithParam(const string eventName, const string key, const string value);
    
    /*
     *  事件计时结束。和logEventTimedWithParam成对调用。
     *  @prama mapParam。可用于更新Event开始时的值，没有可以空map.或调用logEventTimedEnd
     */
    void logEventTimedEndWithParam(const string eventName, map<string, string> mapParam);
    
#pragma mark --这里是封装对某个模块(场景)的计时功能
    
    /*
     * 记录某个模块开始，通常在你要计时的场景的onEnter里调用。
     * @prama moduleName 模块名称
    */
    void logCurrentModuleEnterEvent(string moduleName);
    
    /*
     * 记录进入某个模块，通常在你要计时的场景的onEnter里调用。务必一定要在onExit里对应调用下面的logCurrentModuleExitEvent方法表示计时结束
     * @prama moduleName 模块名称
     * @prama mapParam 自定义参数，以map的形式。如你想记录用户玩到哪一步了，你可以加 mapParam["Step"] = 3, 表示玩到第3步了。
     */
    void logCurrentModuleEnterEvent(string moduleName, map<string, string> mapParam);
    
    /*
     * 记录某个模块结束，通常在你要计时的场景的onExit里调用。
     * @prama moduleName 模块名称
     */
    void logCurrentModuleExitEvent(string moduleName);
    
    /*
     * 记录某个模块结束，通常在你要计时的场景的onExit里调用。
     * @prama moduleName 模块名称
     * @prama mapParam 自定义参数.用以更新在logCurrentModuleEnterEvent里参数的值，只有在logCurrentModuleEnterEvent中出现的参数才有效更新到。
     */
    void logCurrentModuleExitEvent(string moduleName, map<string, string> mapParam);
    
private:
    //record user path
    void pushModule(string name);
    
    void clearModules();
private:
    vector<string> vModulesNamePath;
};

