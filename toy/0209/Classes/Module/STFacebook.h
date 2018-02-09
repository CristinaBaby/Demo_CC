//
//  STFacebook.h
//  SweetDiscovery
//
//  Created by zhangguangzong1 on 9/3/13.
//
//

#ifndef SweetDiscovery_STFacebook_h
#define SweetDiscovery_STFacebook_h

class STFacebook {
    
public:
    STFacebook();
    ~STFacebook();
public:
    // in Android OS, the below methods need communicate with jni,
    // so the relative source file is placed in jni directory under
    // the proj.androd
    virtual void postContent(const char * caption, const char* shareContent);
    
};


#endif
