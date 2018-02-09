/*
 * ST_SystemFunction_Android.h
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#ifndef ST_SYSTEMFUNCTION_ANDROID_H_
#define ST_SYSTEMFUNCTION_ANDROID_H_

#include "JNIHelper.h"
#include <string>
using namespace std;
class SystemFunctionManager
{
public:
	SystemFunctionManager() :
			ClassSF(0),
			stSFJava(0),
			MethodSendMailByIntent(0),
			MethodPopAlertDialog(0),
			MethodMakeToast(0),
			MethodShowMoreGame(0),
			MethodGo2MarketDetail(0),
			MethodRating(0),
			MethodContactUs(0),
			MethodSendEmailAndAssetPic(0),
			MethodSendEmailAndFilePic(0),
			MethodGetSdCardPath(0),
			MethodCheckNetworkAvailable(0),
			MethodIsTabletAvailable(0),
			MethodDensityScale(0),
			MethodRateUs(0),
            MethodDismissRateus(0),
            MethodNewsBlast(0),
            MethodRefreshDCIM(0),
            MethodEndSession(0)
           // MethodCreateDialog(0),
           // MethodcreateDialog(0)

	{
	}
	~SystemFunctionManager();

	static SystemFunctionManager* getInstance();

	static void destroy();

	bool init(JNIEnv * pEnv, jobject pJava);

public:
	void sendEmail(const char* subject, const char* body);
	void popAlertDialog(const char* message);
	void makeToast(const char* message);
	void showMoreGame();
	void go2MarketDetail(std::string packagename);
	void rating();
	void contactUs();
	void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
	void sendEmailAndFilePic(std::string subject, std::string message,std::string fileName);
	std::string getSdCardPath();
	bool checkNetworkAvailable();
	bool isTabletAvailable();
	float densityScale();
	void rateUs();
	void dismissRateUs();
    void doNewsBlast();
    //void createDialog(std::string meaasge, std::string positive,std::string negative,const char*,const char*);
    /*
     * 向服务器发送后台统计数据，在APP出的时候发送
     */
	void endSession();
    void refreshDCIM(std::string sFileDir);
    int getDisplayRotation();

  //  void createDialog(std::string meaasge, std::string positive,std::string negative,Ref*,CallFunction);
  //  void createDialog(std::string meaasge, std::string positive);
private:
	/*>>>>>>>> below declaration is used by jni <<<<<<<<*/
	// Cached Classes.
	jclass ClassSF;
	// Cached java object
	jobject stSFJava;
	// Cached active Methods.
	jmethodID MethodSendMailByIntent;
	jmethodID MethodPopAlertDialog;
	jmethodID MethodMakeToast;
	jmethodID MethodShowMoreGame;
	jmethodID MethodRating;
	jmethodID MethodGo2MarketDetail;
	jmethodID MethodContactUs;
	jmethodID MethodSendEmailAndAssetPic;
	jmethodID MethodSendEmailAndFilePic;
	jmethodID MethodGetSdCardPath;
	jmethodID MethodCheckNetworkAvailable;
	jmethodID MethodIsTabletAvailable;
	jmethodID MethodDensityScale;
	jmethodID MethodRateUs;
	jmethodID MethodDismissRateus;
    jmethodID MethodNewsBlast;
    jmethodID MethodEndSession;
    jmethodID MethodRefreshDCIM;
   // jmethodID MethodCreateDialog;
   // jmethodID MethodCreateDialog1;
};

#endif /* ST_SYSTEMFUNCTION_ANDROID_H_ */
