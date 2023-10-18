//
//  PopupPetEquipBox.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 31/07/2019.
//

#ifndef PopupPetEquipBox_hpp
#define PopupPetEquipBox_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class InfoPetEquip;
class PopupPetEquipBox : public PopupBase, public IRefresh
{
public:
    static PopupPetEquipBox* create();
    
    PopupPetEquipBox(void);
    virtual ~PopupPetEquipBox(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    //
    void onBoxBuyAll();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiBuy();
    void uiEvent();

    // draw
    void drawInfo();
    void drawBuy();
    void drawEvent();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickHelp(cocos2d::Ref* sender);
    void onClickHelpEvent(cocos2d::Ref* sender);
    void onClickBox(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    void onClickFree(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool bResult);
    void callbackFree(bool bResult, int nResult, int eType);
    void callbackBoxBuy(bool bResult, int nResult, int eType);
    
private:
    bool _bInitUI;

    //
    E_PET_EQUIP_BUY _boxBuyType;
    int _boxBuyCount;

    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerBuy;
    cocos2d::ui::Layout* _uiContainerEvent;
    
    cocos2d::ui::ImageView* _uiBox;
    cocos2d::ui::ImageView* _uiBoxBoost;
    cocos2d::ui::CText* _uiBoxSpecialCount;
    cocos2d::ui::CText* _uiBoxNormalCount;
    cocos2d::ui::Layout* _uiBoxButtons;
};


#endif /* PopupPetEquipBox_hpp */
