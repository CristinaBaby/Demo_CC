//
//  DialogCallback.h
//  
//
//  Created by liuhailong1 on 13-10-24.
//
//

#ifndef __DIALOG_CALLBACK__
#define __DIALOG_CALLBACK__


class DialogCallback{
public:
    virtual void onNegativeClick(void*) = 0;
    virtual void onPositiveClick(void*) = 0;
};


#endif 
