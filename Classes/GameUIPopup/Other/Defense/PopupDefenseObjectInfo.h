//
//  PopupDefenseObjectInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/04.
//

#ifndef PopupDefenseObjectInfo_hpp
#define PopupDefenseObjectInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigDefault.h"


class InfoDefenseWeapon;
class InfoDefenseCharacter;
class PopupDefenseObjectInfo : public PopupBase
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        
        RANK_UP = 1,
        LEVEL_UP,
        SMELTING
    };
    
    static PopupDefenseObjectInfo* create(E_DF_Trench_SettingType type, const int idx);
    
    PopupDefenseObjectInfo(void);
    virtual ~PopupDefenseObjectInfo(void);
    bool init(E_DF_Trench_SettingType type, const int idx);
    
    
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    void uiReload();
    void uiLoadLayer();
    
    void onClickClose(cocos2d::Ref* sender);
    void onClickAnother(cocos2d::Ref* sender);
    void onClickTab(cocos2d::Ref* sender);
    
protected:
    void callbackServer(bool bResult, int nResult);
    
private:
    PopupDefenseObjectInfo::E_TYPE _tabType;
    E_DF_Trench_SettingType _type;
    int _idx;
    
    InfoDefenseWeapon* _weaponData;
    InfoDefenseCharacter* _characterData;
    
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    
    cocos2d::Layer* _lyContainerNow;
    cocos2d::Layer* _lyLevelUp;
    cocos2d::Layer* _lyRankUp;
    cocos2d::Layer* _lySmeltingUp;
    
    MafDelegate<void(int)> _onEventChangeObject;
};


#endif /* PopupDefenseObjectInfo_hpp */
