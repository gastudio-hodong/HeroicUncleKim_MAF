//
//  UILayoutPetCapability.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutPetCapability_h
#define UILayoutPetCapability_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayoutPetCapability : public cocos2d::ui::Layout
{
public:
    static UILayoutPetCapability* create(E_PET ePet);

    UILayoutPetCapability(void);
    virtual ~UILayoutPetCapability(void);
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
    void uiLevelUP();
    void uiInfo();
    void uiInfoOther();
    
    // draw
    void drawLevelUP();
    void drawInfo();
    void drawInfoOther();
    
    // click
    
    // callback
     
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    E_PET _ePet;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUILevelUP;
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIInfoOther;
    
    
};
#endif /* UILayoutPetCapability_h */
