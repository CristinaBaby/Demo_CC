
#include "MoreGameUrl.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#define CLASS_NAME "com/make/base/MyJinHelp"
using namespace cocos2d;

string MoreGameUrl::getMoreGameUrl(){
    JniMethodInfo t;
			if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getMoreGameUrl", "()Ljava/lang/String;"))
			{
				jstring path = static_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
				const char* pathchars = t.env->GetStringUTFChars(path, NULL);
				string ret(pathchars);
				t.env->ReleaseStringUTFChars(path, pathchars);
				return ret;
			}
    return "";
}
