//
//  PopupTicket.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupTicket_h
#define PopupTicket_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupTicket : public PopupBase
{
public:
    static PopupTicket* create();
    
    PopupTicket(void);
    virtual ~PopupTicket(void);
    virtual bool init();
    
public:
    // set
    void setCallbackUse(const std::function<void(void)>& callback);
    void setCallbackBuy(const std::function<void(void)>& callback);
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiList();
    
    // draw
    void drawInfo();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickUse(cocos2d::Ref* sender);
    void onClickUseOk(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    
    // callback
    void callbackInAppPurchaseTicket(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackTicketUse(bool bResult, int nResult);
    
    
private:
    int _nTicket;
    
    std::function<void(void)> _onCallbackUse;
    std::function<void(void)> _onCallbackBuy;
    
    // widget
    cocos2d::Layer* _layerContainerTop;
    cocos2d::Layer* _layerContainerList;
    
    cocos2d::ui::CText* _uiTextPTicket;
    cocos2d::ui::ImageView* _uiRewardContainer;
};

#endif /* PopupTicket_h */
