/*
 * ST_SystemFunction_Android.h
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#ifndef RUNTIME_PERMISSION_MANAGER_H_
#define RUNTIME_PERMISSION_MANAGER_H_

#include <string>
#include "RuntimePermission.h"
#include <functional>

using namespace std;
class RuntimePermissionDelegate;
class RuntimePermissionManager
{
public:
	RuntimePermissionManager() :
#if _cplusplus > 201100L
    onPermissionGrantedResult(nullptr),
    mRuntimePermissionDelegate(nullptr)
#else
    mRuntimePermissionDelegate(NULL)
#endif
	{
	}
//	~RuntimePermissionManager();
	static RuntimePermissionManager* getInstance();
public:
    
//    void requestRuntimePermissions(int requestCode,const std::string *permissions,int permissionCount);
      void requestRuntimePermissions(int requestCode,long permission);
public:
#if __cplusplus > 201100L
    /**
     * C++11,可以使用std::function简化代码
     */
    std::function<void(int,bool)> onPermissionGrantedResult;
#endif
    RuntimePermissionDelegate *mRuntimePermissionDelegate;
    
};

class  RuntimePermissionDelegate{
public:
    virtual void onPermissionGrantedResult(int requestCode,bool bGranted){};
};

#endif /* ST_SYSTEMFUNCTION_ANDROID_H_ */
