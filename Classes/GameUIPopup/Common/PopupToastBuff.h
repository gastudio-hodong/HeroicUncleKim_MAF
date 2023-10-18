//
//  PopupToastBuff.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupToastBuff_h
#define PopupToastBuff_h

#include "Common/ConfigDefault.h"

enum class E_TOAST_BUFF {
    NONE = 0,
    
    BUFF_ATK,
    BUFF_SPD,
    BUFF_COIN,
    BUFF_ALL
};

class PopupToastBuff : public cocos2d::Layer
{
public:
    static PopupToastBuff* create(E_TOAST_BUFF eType);
    
    PopupToastBuff(void);
    virtual ~PopupToastBuff(void);
    virtual bool init(E_TOAST_BUFF eType);
    
public:
    static void show(E_TOAST_BUFF eType);
    
protected:
    
    // init
    void initVar();
    void initUi();
     
    
private:
    //
    static int _nCountVisible;
    
    //
    E_TOAST_BUFF _eType;
    
    //
    cocos2d::Layer* _layerContainerBottom;
    
};

#endif /* PopupToastBuff_h */
