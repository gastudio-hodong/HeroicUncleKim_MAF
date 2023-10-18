//
//  PopupAdsSkip.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupAdsSkip_h
#define PopupAdsSkip_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupAdsSkip : public PopupBase
{
public:
    static PopupAdsSkip* create();
    
    PopupAdsSkip(void);
    virtual ~PopupAdsSkip(void);
    virtual bool init() override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContent();
    
    // ui draw
    void drawContent();
    
    // callback
    void callbackBuyPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPurchase(cocos2d::Ref* sender);
    
private:
     
    
    // widget
    cocos2d::ui::Layout* _uiContainerContent;
    
    cocos2d::ui::CText* _uiTextDesc;
    cocos2d::ui::CText* _uiTextActive;
    cocos2d::ui::CText* _uiTextActivePlus1;
    cocos2d::ui::CText* _uiTextActivePlus2;
    
 
};
#endif /* PopupAdsSkip_h */
