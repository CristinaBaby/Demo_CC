/*
 * ST_SystemFunction_Android.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "RuntimePermissionManager.h"

static RuntimePermissionManager* sp_RuntimePermissionManager = 0;

RuntimePermissionManager* RuntimePermissionManager::getInstance()
{
	if (!sp_RuntimePermissionManager)
	{
		sp_RuntimePermissionManager = new RuntimePermissionManager();
	}

	return sp_RuntimePermissionManager;
}


void RuntimePermissionManager::requestRuntimePermissions(int requestCode, long permission){
    RuntimePermission::getInstance()->requestRuntimePermissions(requestCode, permission);
}

//RuntimePermissionManager::~RuntimePermissionManager()
//{
//    
//}
