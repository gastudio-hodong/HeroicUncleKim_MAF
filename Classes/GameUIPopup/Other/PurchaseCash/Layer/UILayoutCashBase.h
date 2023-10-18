//
//  UILayoutCashBase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutCashBase_h
#define UILayoutCashBase_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayoutCashBase : public cocos2d::ui::Layout
{
public:
    UILayoutCashBase(void);
    virtual ~UILayoutCashBase(void) {};
    virtual bool init() override;
    
public:
    virtual void onDraw() = 0;
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    
    // ui draw
    
    // callback
    void callbackInfo(bool bResult);
    void callbackDailyInfo(bool bResult);
    void callbackRemainingInfo(bool bResult);
    
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackPurchaseDailyReceive(bool bResult, int nIdx, std::string strItems);

    //
    void onPurchase(int idx);
    void onPurchaseDailyReceive(int idx);
    
    

private:
    //
    
    //
    
    
};
#endif /* UILayoutCashBase_h */
