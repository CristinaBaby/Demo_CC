/*
 * ST_SystemFunction_Android.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "ST_SystemFunction_Android.h"
#include "platform/android/jni/JniHelper.h"

static SystemFunctionManager* sp_SystemFunctionManager = 0;

SystemFunctionManager* SystemFunctionManager::getInstance()
{
	if (!sp_SystemFunctionManager)
	{
		sp_SystemFunctionManager = new SystemFunctionManager();
	}

	return sp_SystemFunctionManager;
}

void SystemFunctionManager::destroy()
{
	if (sp_SystemFunctionManager)
	{
		delete sp_SystemFunctionManager;

		sp_SystemFunctionManager = 0;
	}
}

bool SystemFunctionManager::init(JNIEnv * pEnv, jobject pJava)
{
    LOGE("%s", __func__);
	// get class
	ClassSF = pEnv->GetObjectClass(pJava);
	if (!ClassSF)
	{
		LOGE("get STSystemFunction class Failed!");
		return false;
	}
	ClassSF = (jclass) JNIHelper::makeGlobalRef(pEnv, ClassSF);

	// get method id --- purchaseManaged(String)
	MethodSendMailByIntent = pEnv->GetMethodID(ClassSF, "sendMailByIntent", "(Ljava/lang/String;Ljava/lang/String;)V");
	if (!MethodSendMailByIntent)
	{
		LOGE("get Method SendEmail id Failed!");
		return false;
	}

	// SharedPreferences

	MethodPopAlertDialog = pEnv->GetMethodID(ClassSF, "popAlertDialog", "(Ljava/lang/String;)V");
	if (!MethodPopAlertDialog)
	{
		LOGE("get Method popAlertDialog id Failed!");
		return false;
	}

	MethodMakeToast = pEnv->GetMethodID(ClassSF, "makeToast", "(Ljava/lang/String;)V");
	if (!MethodMakeToast)
	{
		LOGE("get Method makeToast id Failed!");
		return false;
	}

	MethodShowMoreGame = pEnv->GetMethodID(ClassSF, "showMoreGame", "()V");
	if (!MethodShowMoreGame)
	{
		LOGE("get Method ShowMoreGame id Failed!");
		return false;
	}
	MethodGo2MarketDetail = pEnv->GetMethodID(ClassSF, "go2MarketDetail", "(Ljava/lang/String;)V");
	if (!MethodGo2MarketDetail)
	{
		LOGE("get MethodGo2MarketDetail id Failed!");
		return false;
	}
	MethodRating = pEnv->GetMethodID(ClassSF, "rating", "()V");
	if (!MethodRating)
	{
		LOGE("get MethodRating id Failed!");
		return false;
	}
	// get method id --- void contactUs()
	MethodContactUs = pEnv->GetMethodID(ClassSF, "contactUs", "()V");
	if (!MethodContactUs)
	{
		LOGE("get MethodContactUs id Failed!");
		return false;
	}
	// get method id --- void sendEmailAndAssetPic(final String subject, final String text, final String assetName)
	MethodSendEmailAndAssetPic = pEnv->GetMethodID(ClassSF, "sendEmailAndAssetPic",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (!MethodSendEmailAndAssetPic)
	{
		LOGE("get MethodSendEmailAndAssetPic id Failed!");
		return false;
	}
	// get method id --- void sendEmailAndFilePic(final String subject, final String text, final String fileName)
	MethodSendEmailAndFilePic = pEnv->GetMethodID(ClassSF, "sendEmailAndFilePic",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (!MethodSendEmailAndFilePic)
	{
		LOGE("get MethodSendEmailAndFilePic id Failed!");
		return false;
	}
	// get method id --- String getSdCardPath()
	MethodGetSdCardPath = pEnv->GetMethodID(ClassSF, "getSdCardPath", "()Ljava/lang/String;");
	if (!MethodGetSdCardPath)
	{
		LOGE("get MethodGetSdCardPath id Failed!");
		return false;
	}
	// check network available --- boolean checkNetworkAvailable()
	MethodCheckNetworkAvailable = pEnv->GetMethodID(ClassSF, "checkNetworkAvailable", "()Z");
	if (!MethodCheckNetworkAvailable)
	{
		LOGE("get MethodCheckNetworkAvailable id Failed!");
		return false;
	}
	//istablet
	MethodIsTabletAvailable = pEnv->GetMethodID(ClassSF, "isTabletAvailable", "()Z");
	if (!MethodIsTabletAvailable)
	{
		LOGE("get MethodIsTableAvailable id Failed!");
		return false;
	}
	MethodDensityScale = pEnv->GetMethodID(ClassSF, "densityScale", "()F");
	if (!MethodDensityScale)
	{
		LOGE("get MethodDensityScale id Failed!");
		return false;
	}
	MethodRateUs = pEnv->GetMethodID(ClassSF, "rating", "()V");
	if (!MethodRateUs)
	{
		LOGE("get MethodRateUs id Failed!");
		return false;
	}
//	MethodDismissRateus = pEnv->GetMethodID(ClassSF, "dismissRateUs", "()V");
//	if (!MethodDismissRateus)
//	{
//		LOGE("get MethodDismissRateus id Failed!");
//		return false;
//	}
    MethodNewsBlast =pEnv->GetMethodID(ClassSF, "newsBlast", "()V");
	if (!MethodNewsBlast)
	{
		LOGE("get MethodNewsBlast id Failed!");
		return false;
	}
    
	MethodEndSession = pEnv->GetMethodID(ClassSF, "endSession", "()V");
	if (!MethodEndSession)
	{
		LOGE("get MethodEndSession id Failed!");
		return false;
	}
    
    MethodRefreshDCIM = pEnv->GetMethodID(ClassSF, "refreshDCIM", "(Ljava/lang/String;)V");
	if (!MethodRefreshDCIM)
	{
		LOGE("get MethodRefreshDCIM id Failed!");
		return false;
	}

//	MethodCreateDialog = pEnv->GetMethodID(ClassSF, "createDialog",
//			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
//	if (!MethodCreateDialog)
//	{
//		LOGE("get MethodRefreshDCIM id Failed!");
//		return false;
//	}
//
//	MethodCreateDialog1 = pEnv->GetMethodID(ClassSF, "createDialog1",
//				"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
//		if (!MethodCreateDialog1)
//		{
//			LOGE("get MethodRefreshDCIM id Failed!");
//			return false;
//		}

	// Caches objects.

	// Caches objects.
	stSFJava = JNIHelper::makeGlobalRef(pEnv, pJava);
	if (!stSFJava)
	{
		LOGE("Cache stSFJava Failed!");
		return false;
	}

	return true;
}

void SystemFunctionManager::sendEmail(const char* subject, const char* body)
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::sendEmail() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	// Convert the C++ string to C-string, then to JNI String (jstring) for method args
	jstring sub = lEnv->NewStringUTF(subject);
	jstring content = lEnv->NewStringUTF(body);

	lEnv->CallVoidMethod(stSFJava, MethodSendMailByIntent, sub, content);

	lEnv->DeleteLocalRef(sub);
	lEnv->DeleteLocalRef(content);
}

void SystemFunctionManager::popAlertDialog(const char* message)
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::popAlertDialog() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	// Convert the C++ string to C-string, then to JNI String (jstring) for method args
	jstring msg = lEnv->NewStringUTF(message);

	lEnv->CallVoidMethod(stSFJava, MethodPopAlertDialog, msg);

	lEnv->DeleteLocalRef(msg);
}

void SystemFunctionManager::makeToast(const char* message)
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::makeToast() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	// Convert the C++ string to C-string, then to JNI String (jstring) for method args
	jstring msg = lEnv->NewStringUTF(message);

	lEnv->CallVoidMethod(stSFJava, MethodMakeToast, msg);

	lEnv->DeleteLocalRef(msg);
}

void SystemFunctionManager::showMoreGame()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::showMoreGame() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	lEnv->CallVoidMethod(stSFJava, MethodShowMoreGame);
}
void SystemFunctionManager::go2MarketDetail(std::string packagename)
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::Go2MarketDetail() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jstring jniPackagename = lEnv->NewStringUTF(packagename.c_str());

	lEnv->CallVoidMethod(stSFJava, MethodGo2MarketDetail, jniPackagename);
	lEnv->DeleteLocalRef(jniPackagename);
}

void SystemFunctionManager::rating()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::rating() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	lEnv->CallVoidMethod(stSFJava, MethodRating);
}

void SystemFunctionManager::contactUs()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::ContactUs() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	lEnv->CallVoidMethod(stSFJava, MethodContactUs);
}
void SystemFunctionManager::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::sendEmailAndAssetPic() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jstring jniSubject = lEnv->NewStringUTF(subject.c_str());
	jstring jniMessage = lEnv->NewStringUTF(message.c_str());
	jstring jniAssetName = lEnv->NewStringUTF(assetName.c_str());

	lEnv->CallVoidMethod(stSFJava, MethodSendEmailAndAssetPic, jniSubject, jniMessage, jniAssetName);
	lEnv->DeleteLocalRef(jniSubject);
	lEnv->DeleteLocalRef(jniMessage);
	lEnv->DeleteLocalRef(jniAssetName);
}
void SystemFunctionManager::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::sendEmailAndFilePic() failed!");
		return;
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jstring jniSubject = lEnv->NewStringUTF(subject.c_str());
	jstring jniMessage = lEnv->NewStringUTF(message.c_str());
	jstring jniFileName = lEnv->NewStringUTF(fileName.c_str());

	lEnv->CallVoidMethod(stSFJava, MethodSendEmailAndFilePic, jniSubject, jniMessage, jniFileName);
	lEnv->DeleteLocalRef(jniSubject);
	lEnv->DeleteLocalRef(jniMessage);
	lEnv->DeleteLocalRef(jniFileName);
}
std::string SystemFunctionManager::getSdCardPath()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::getSdCardPath() failed!");
		return "";
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	//lEnv->CallObjectMethod(stSFJava, MethodGetSdCardPath);

	jstring path = static_cast<jstring>(lEnv->CallObjectMethod(stSFJava, MethodGetSdCardPath));
	if (path == NULL)
	{
		return "";
	}
	const char* pathchars = lEnv->GetStringUTFChars(path, NULL);
	string ret(pathchars);
	lEnv->ReleaseStringUTFChars(path, pathchars);
	return ret;

}
bool SystemFunctionManager::checkNetworkAvailable()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::checkNetworkAvailable() failed!");
		return "";
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	jboolean ret = lEnv->CallBooleanMethod(stSFJava, MethodCheckNetworkAvailable);
	return (ret == JNI_TRUE ? true : false);
}
bool SystemFunctionManager::isTabletAvailable()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::isTabletAvailable() failed!");
		return "";
	}

	JNIEnv* lEnv = JNIHelper::getJNIEnv();

	jboolean ret = lEnv->CallBooleanMethod(stSFJava, MethodIsTabletAvailable);
	return (ret == JNI_TRUE ? true : false);
}
float SystemFunctionManager::densityScale()
{
	if (!stSFJava)
	{
		LOGE("SystemFunctionManager::densityScale() failed!");
		return 0;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jfloat f = lEnv->CallFloatMethod(stSFJava, MethodDensityScale);
	return (float) f;
}
void SystemFunctionManager::rateUs(){
	if (!stSFJava)
		{
			LOGE("SystemFunctionManager::rateUs() failed!");
			return;
		}

		JNIEnv* lEnv = JNIHelper::getJNIEnv();

		lEnv->CallVoidMethod(stSFJava, MethodRateUs);
}
void SystemFunctionManager::dismissRateUs(){
//	if (!stSFJava)
//		{
//			LOGE("SystemFunctionManager::dismissRateUs() failed!");
//			return;
//		}
//
//		JNIEnv* lEnv = JNIHelper::getJNIEnv();
//
//		lEnv->CallVoidMethod(stSFJava, MethodDismissRateus);
}

void SystemFunctionManager::doNewsBlast(){
    if (!stSFJava)
    {
        LOGE("SystemFunctionManager::doNewsBlast() failed!");
        return;
    }
    
    LOGE("%s %s", __FILE__, __FUNCTION__);
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    
    lEnv->CallVoidMethod(stSFJava, MethodNewsBlast);
}

void SystemFunctionManager::endSession(){
    if (!stSFJava)
    {
        LOGE("SystemFunctionManager::endSession() failed!");
        return;
    }

    JNIEnv* lEnv = JNIHelper::getJNIEnv();

    lEnv->CallVoidMethod(stSFJava, MethodEndSession);
}

void SystemFunctionManager::refreshDCIM(std::string sFileDir)
{
    if (!stSFJava)
    {
        LOGE("SystemFunctionManager::%s() failed!", __FUNCTION__);
        return;
    }
    LOGE("%s %s", __FILE__, __FUNCTION__);
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    jstring msg = lEnv->NewStringUTF(sFileDir.c_str());
    lEnv->CallVoidMethod(stSFJava, MethodRefreshDCIM, msg);
}

//void SystemFunctionManager::createDialog(std::string meaasge, std::string positive,std::string negative,const char*p1,const char*p2)
//{
//    if (!stSFJava)
//    {
//        LOGE("SystemFunctionManager::%s() failed!", __FUNCTION__);
//        return;
//    }
//    return;
//    LOGE("%s %s", __FILE__, __FUNCTION__);
//	JNIEnv* lEnv = JNIHelper::getJNIEnv();
//	jstring jniSubject = lEnv->NewStringUTF(meaasge.c_str());
//	jstring jniMessage = lEnv->NewStringUTF(positive.c_str());
//	jstring jniAssetName = lEnv->NewStringUTF(negative.c_str());
//
//	jstring jniP1 = lEnv->NewStringUTF(p1);
//	jstring jniP2 = lEnv->NewStringUTF(p2);
//
//	lEnv->CallVoidMethod(stSFJava, MethodSendEmailAndAssetPic, jniSubject, jniMessage, jniAssetName, jniP1, jniP2);
//	lEnv->DeleteLocalRef(jniSubject);
//	lEnv->DeleteLocalRef(jniMessage);
//	lEnv->DeleteLocalRef(jniAssetName);
//
//}

//void SystemFunctionManager::createDialog1(std::string meaasge, std::string positive,std::string negative)
//{
//    if (!stSFJava)
//    {
//        LOGE("SystemFunctionManager::%s() failed!", __FUNCTION__);
//        return;
//    }
//    return;
//    LOGE("%s %s", __FILE__, __FUNCTION__);
//	JNIEnv* lEnv = JNIHelper::getJNIEnv();
//	jstring jniSubject = lEnv->NewStringUTF(meaasge.c_str());
//	jstring jniMessage = lEnv->NewStringUTF(positive.c_str());
//	jstring jniAssetName = lEnv->NewStringUTF(negative.c_str());
//
//	jstring jniP1 = lEnv->NewStringUTF(p1);
//	jstring jniP2 = lEnv->NewStringUTF(p2);
//
//	lEnv->CallVoidMethod(stSFJava, MethodSendEmailAndAssetPic, jniSubject, jniMessage, jniAssetName, jniP1, jniP2);
//	lEnv->DeleteLocalRef(jniSubject);
//	lEnv->DeleteLocalRef(jniMessage);
//	lEnv->DeleteLocalRef(jniAssetName);
//
//}


SystemFunctionManager::~SystemFunctionManager()
{
	JNIHelper::deleteGlobalRef(ClassSF);
	JNIHelper::deleteGlobalRef(stSFJava);
}



// TODO static method
#define  CLASS_NAME "com/skycastleapps/android_hotballoonmaker/AppActivity"

typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;
// get env and cache it
static JNIEnv* getJNIEnv(void) {
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    if (NULL == jvm) {
        LOGD("Failed to get JNIEnv. JniHelper::getJavaVM() is NULL");
        return NULL;
    }
    
    JNIEnv *env = NULL;
    // get jni environment
    jint ret = jvm->GetEnv((void**) &env, JNI_VERSION_1_4);
    
    switch (ret) {
        case JNI_OK:
            // Success!
            return env;
            
        case JNI_EDETACHED:
            // Thread not attached
            
            // TODO : If calling AttachCurrentThread() on a native thread
            // must call DetachCurrentThread() in future.
            // see: http://developer.android.com/guide/practices/design/jni.html
            
            if (jvm->AttachCurrentThread(&env, NULL) < 0) {
                LOGD("Failed to get the environment using AttachCurrentThread()");
                return NULL;
            } else {
                // Success : Attached and obtained JNIEnv!
                return env;
            }
            
        case JNI_EVERSION:
            // Cannot recover from this error
            LOGD("JNI interface version 1.4 not supported");
        default:
            LOGD("Failed to get the environment using GetEnv()");
            return NULL;
    }
}

// get class and make it a global reference, release it at endJni().
static jclass getClassID(JNIEnv *pEnv)
{
    jclass ret = pEnv->FindClass(CLASS_NAME);
    if (! ret)
    {
        LOGD("Failed to find class of %s", CLASS_NAME);
    }
    return ret;
}

static bool getStaticMethodInfo(JniMethodInfo &methodinfo, const char *methodName, const char *paramCode)
{
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    bool bRet = false;
    do
    {
        pEnv = getJNIEnv();
        if (! pEnv)
        {
            break;
        }
        jclass classID = getClassID(pEnv);
        
        methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
        if (! methodID)
        {
            LOGD("Failed to find static method id of %s", methodName);
            break;
        }
        methodinfo.classID = classID;
        methodinfo.env = pEnv;
        methodinfo.methodID = methodID;
        
        bRet = true;
    } while (0);
    
    return bRet;
}

int SystemFunctionManager::getDisplayRotation() {
    // void playBackgroundMusic(String,boolean)
    JniMethodInfo methodInfo;
    if (!getStaticMethodInfo(methodInfo, "getDisplayRotation",
                             "()I")) {
        return -1;
    }
    jint iRet = methodInfo.env->CallIntMethod(methodInfo.classID,
                                              methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    return (int) iRet;
}

