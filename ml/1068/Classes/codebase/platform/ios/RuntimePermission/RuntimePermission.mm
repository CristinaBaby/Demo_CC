//
//  RuntimePermission.m
//  AdsDemo
//
//  Created by tangjinlong on 16/6/14.
//
//

#include <stdio.h>
#include "RuntimePermission.h"
#include "RuntimePermissionManager.h"
#if __cplusplus > 201100L
RuntimePermission * RuntimePermission::m_pInstance=nullptr;
#else
RuntimePermission * RuntimePermission::m_pInstance=NULL;
#endif

RuntimePermission::~RuntimePermission(){
    
}

RuntimePermission* RuntimePermission::getInstance(){
    if(!m_pInstance){
        m_pInstance=new RuntimePermission();
    }
    return m_pInstance;
}

void RuntimePermission::requestRuntimePermissions(int requestCode,long permission){
    //暂时先直接默认已被授权，以后iOS端再实现权限管理流程
#if __cplusplus > 201100L
    if (RuntimePermissionManager::getInstance()->onPermissionGrantedResult) {
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult(requestCode,true);
        }
#else
    if (RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate) {
        RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate->onPermissionGrantedResult(requestCode,true);
    }
#endif
}
