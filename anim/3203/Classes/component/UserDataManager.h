//
//  UserDataManager.h
//  Doctor2
//
//  Created by tanshoumei on 13-9-26.
//
//

#ifndef __Doctor2__UserDataManager__
#define __Doctor2__UserDataManager__

#include <iostream>

/*
 玩家持久化数据管理
 **/
class UserDataManager
{
public:
    
    //打开音效
    static void openMusic();
    
    //关闭音效
    static void closeMusic();
    
    //获取音效状态
    static bool isOpenMusic();
    
    //打开背景音乐(如果需要播放传音乐路径)
    static void openSound(const char *pBGMusic);
    
    //关闭背景音乐
    static void closeSound();
    
    //是否打开背景音乐
    static bool isOpenSound();
    
};

#endif