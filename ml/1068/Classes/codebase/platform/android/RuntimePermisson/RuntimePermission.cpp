//
//  RuntimePermission.cpp
//  AdsDemo
//
//  Created by tangjinlong on 16/6/13.
//
//

#include <stdio.h>
#include "RuntimePermission.h"
#include "RuntimePermissionJNI.h"
RuntimePermission* RuntimePermission::m_pInstance = nullptr;
RuntimePermission::~RuntimePermission()
{
    
}

RuntimePermission* RuntimePermission::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new RuntimePermission();
    }
    return m_pInstance;
}
void RuntimePermission::requestRuntimePermissions(int requestCode,long permission){
    RuntimePermissionJNI::getInstance()->requestRuntimePermissions(requestCode,permission);
}
