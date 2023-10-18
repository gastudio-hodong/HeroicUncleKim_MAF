//
//  PopupPetEquipBoxBuyCount.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 31/07/2019.
//

#ifndef PopupPetEquipBoxBuyCount_hpp
#define PopupPetEquipBoxBuyCount_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupPetEquipBoxBuyCount : public PopupBase
{
public:
    static PopupPetEquipBoxBuyCount* create(E_PET_EQUIP_BUY eType, bool bEvent);
    
    PopupPetEquipBoxBuyCount(void);
    virtual ~PopupPetEquipBoxBuyCount(void);
    virtual bool init(E_PET_EQUIP_BUY eType, bool bEvent);
    virtual void onEnter() override;
    
public:
    void setCallback(const std::function<void(E_PET_EQUIP_BUY, int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiButtons();

    // draw
    void drawInfo();
    void drawButtons();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickArrow(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    
    // callback
    
private:
    bool _bInitUI;

    //
    E_PET_EQUIP_BUY _eType;
    bool _bEvent;
    
    int _countNow;
    int _countMax;
    
    //
    std::function<void(E_PET_EQUIP_BUY, int)> _callbackResult;

    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerButtons;
};


#endif /* PopupPetEquipBoxBuyCount_hpp */
