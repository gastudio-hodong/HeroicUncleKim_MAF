//
//  PopupWeaponSpiritDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/23.
//

#ifndef PopupWeaponSpiritDefault_hpp
#define PopupWeaponSpiritDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritParent.h"

class PopupWeaponSpiritDefault : public PopupBase
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        
        RANK_UP = 1,
        LEVEL_UP,
        SMELTING
    };
    
    static PopupWeaponSpiritDefault* create(InfoWeaponSpirit* infoSpirit);
    
    PopupWeaponSpiritDefault(void);
    virtual ~PopupWeaponSpiritDefault(void);
    bool init(InfoWeaponSpirit* infoSpirit);
    
    
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    void uiReload();
    void uiLoadLayer();
    
    void onClickClose(cocos2d::Ref* sender);
    void onClickAnotherSpirit(cocos2d::Ref* sender);
    void onClickEquip(cocos2d::Ref* sender);
    void onClickTab(cocos2d::Ref* sender);
    void onClickBuff(cocos2d::Ref* sender);
    
    void callbackEquip(bool bResult, int nResult);
    
private:
    PopupWeaponSpiritDefault::E_TYPE _eType;
    InfoWeaponSpirit* _infoSpirit;
    
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    
    cocos2d::Label* _lbSpiritInfo;
    
    UILayerSpiritParent* _lyContainerNow;
    UILayerSpiritParent* _lyContainerRankUp;
    UILayerSpiritParent* _lyContainerSmelting;
    UILayerSpiritParent* _lyContainerLevelUp;
};


#endif /* PopupWeaponSpiritDefault_hpp */
