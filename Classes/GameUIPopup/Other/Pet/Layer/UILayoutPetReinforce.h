//
//  UILayoutPetReinforce.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutPetReinforce_h
#define UILayoutPetReinforce_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayoutPetReinforce : public cocos2d::ui::Layout
{
public:
    static UILayoutPetReinforce* create(E_PET ePet);

    UILayoutPetReinforce(void);
    virtual ~UILayoutPetReinforce(void);
    virtual bool init(E_PET ePet);
    virtual void onEnter() override;

public:
    // set, get, add
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiStep();
    
    // draw
    void drawInfo();
    void drawStep();
    
    // click
    void onClickMine(cocos2d::Ref* sender);
    void onClickReinforce(cocos2d::Ref* sender);
    
    // callback
    void callbackPurchaseOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    E_PET _ePet;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIStep;
    
    
    
};
#endif /* UILayoutPetReinforce_h */
