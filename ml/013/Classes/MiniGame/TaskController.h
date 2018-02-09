//
//  TaskController.h
//
//  Created by tanshoumei on 15/5/28.
//
//

#pragma once

#include <stdio.h>
#include <map>
using namespace std;
//任务控制器
class TaskController
{
public:
    /*
     添加任务
     aTaskId: 任务标志id;
     aCount:  任务需求的数量
     */
    void addTask(const int aTaskId, const int aCount);
    
    int getTaskDidCount(const int aTaskId);
    
    bool isAllTaskDone();
private:
    //要完成的情况
    map<int, int> _mTasks;
    //已完成的情况
    map<int, int> _mTasksDid;
};