//
//  PopupLoading.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupLoading_h
#define PopupLoading_h

#include "Common/ConfigDefault.h"

class PopupLoading : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(PopupLoading);
    
    PopupLoading(void);
    virtual ~PopupLoading(void);
    virtual bool init(void);
    
public:
    static void show();
    static void hide(bool bAll = false);
    static bool isShow();
    
protected:
    // ui
    void uiLoading();
    
    // set, get
    
private:
    static int _nCountVisible;
    
};

#endif /* PopupLoading_h */
