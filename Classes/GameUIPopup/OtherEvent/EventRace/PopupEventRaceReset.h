//
//  PopupEventRaceReset.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupEventRaceReset_h
#define PopupEventRaceReset_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupEventRaceReset : public PopupBase
{
public:
    static PopupEventRaceReset* create();
    
    PopupEventRaceReset(void);
    virtual ~PopupEventRaceReset(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiButtons();
    
    //
    void drawInfo();;
    void drawButtons();
    
    // get, set
    int getResetCount();
    int getResetCountMax();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickResetPayment(cocos2d::Ref* sender);
    void onClickReset(cocos2d::Ref* sender);
    
    // callback
    void callbackReset(bool bResult, int nResult);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
    
private:
    bool _bInitUI;
    
    //

    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerTab;
    cocos2d::ui::Layout* _layerContainerButtons;
    
    cocos2d::ui::Button* _uiBtnResetPayment;
    cocos2d::ui::Button* _uiBtnReset;
};

#endif /* PopupEventRaceReset_h */
