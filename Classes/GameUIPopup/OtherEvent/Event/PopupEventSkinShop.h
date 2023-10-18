//
//  PopupEventSkinShop.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupEventSkinShop_h
#define PopupEventSkinShop_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSkinShop : public PopupBase
{
public:
    static PopupEventSkinShop* create();
    
    PopupEventSkinShop(void);
    virtual ~PopupEventSkinShop(void);
    virtual bool init();
    
    // init
    void initVar();
    void initUI();
    
    void initUIContents();
    
    void onClickButton(Ref* sender);
};

#endif /* PopupEventSkinShop_hpp */
