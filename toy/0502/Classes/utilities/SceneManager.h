//
//  SceneManager.h
//  SingleEye
//
//  Created by gwsoft on 1/21/14.
//
//

#ifndef __SingleEye__SceneManager__
#define __SingleEye__SceneManager__


#include "AppGlobal.h"
#include <vector>

USING_NS_CC;

#define xScene      (SceneManager::getInstance())

//tag
enum SceneTag{
    ST_None,
    S_LOADING,
    S_HOME,
    S_RECORD,               //其实这才是真正的home
    S_PUZZLE_DIFF,
    S_PUZZLE,
    S_SPOT,
    S_COLOR_BOOK,
    S_POLICY,
    S_PREFACE,
};


struct SceneData {
    SceneTag aTag;
    int iData;
    void* pData;
    bool isForward;
};


class SceneManager : public Ref{
    
public:
    static SceneManager* getInstance();
    
public:
    ~SceneManager();    
    void forward(SceneTag aTag);
    void forward(SceneTag aTag, int iData, void* pData);    //这个pData必须独立于场景, 一直存在, 最好是全局的. 不然会出错, 有特殊需求请给我讲.(luoxp)    
    
    void back();
    void back(SceneTag aTag);       //回退到某个指定的场景, 如Home, 如果没有, 将报错.
    SceneTag getRunningSceneTag();
    SceneTag getLastSceneTag();         //这个函数暂时只能在外面用
    

    void dumpAll();
        
private:
    SceneManager();    
    bool isOldScene();
 
    void replaceScene(SceneData &data); //不要用这个函数, 暂时写这里
    
    std::vector<SceneData>  m_vData;
    SceneTag                m_eCurScene;
    Scene*                  m_pOldScene;
    SceneData               m_eBackup;
};


#endif /* defined(__SingleEye__SceneManager__) */
