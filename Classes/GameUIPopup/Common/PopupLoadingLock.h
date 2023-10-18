//
//  PopupLoadingLock.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupLoadingLock_h
#define PopupLoadingLock_h

#include "Common/ConfigDefault.h"

class PopupLoadingLock : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(PopupLoadingLock);
    
    PopupLoadingLock(void);
    virtual ~PopupLoadingLock(void);
    virtual bool init(void);
    
public:
    static void show();
    static void hide(bool bAll = false);
    static bool isShow();
    
protected:
    // set, get
    void setAddTouch();
    void setDelTouch();
    
private:
    static int _nCountVisible;
    
};

#endif /* PopupLoadingLock_h */
