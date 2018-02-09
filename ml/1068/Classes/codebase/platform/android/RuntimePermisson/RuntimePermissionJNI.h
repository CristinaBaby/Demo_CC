/*
 * ST_SystemFunction_Android.h
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#ifndef RUNTIME_PERMISSION_MANAGER_JNI_H_
#define RUNTIME_PERMISSION_MANAGER_JNI_H_

#include "JNIHelper.h"
#include <string>
#include <functional>

using namespace std;
class RuntimePermissionJNI
{
public:
	RuntimePermissionJNI() :
			ClassSF(0),
			stSFJava(0),
            MethodRequestPermissons(0)
	{
	}
	~RuntimePermissionJNI();

	static RuntimePermissionJNI* getInstance();
	bool init(JNIEnv * pEnv, jobject pJava);
public:
    
    void requestRuntimePermissions(int requestCode,long permission);
private:
	/*>>>>>>>> below declaration is used by jni <<<<<<<<*/
	// Cached Classes.
	jclass ClassSF;
	// Cached java object
	jobject stSFJava;
    
	// Cached active Methods.
    jmethodID MethodRequestPermissons;
    
};
#endif /* ST_SYSTEMFUNCTION_ANDROID_H_ */
