//
//  TaskController.cpp
//
//  Created by tanshoumei on 15/5/28.
//
//

#include "TaskController.h"

void TaskController::addTask(const int aTaskId, const int aCount)
{
    if( _mTasks.find(aTaskId) != _mTasks.end() ){
//        assert("repeat task id");
        return;
    }
    _mTasks.insert(make_pair(aTaskId, aCount));
    _mTasksDid.insert(make_pair(aTaskId, 0));
}

int TaskController::getTaskDidCount(const int aTaskId)
{
    if (_mTasksDid.find(aTaskId) != _mTasksDid.end()) {
        return _mTasksDid[aTaskId];
    }
    return 0;
}

bool TaskController::isAllTaskDone()
{
    for (auto itor = _mTasks.begin(); itor != _mTasks.end(); ++itor) {
        if( itor->second > _mTasksDid[itor->first])
            return false;
    }
    return true;
}