//
//  PopupSpiritBuffInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/25.
//

#ifndef PopupSpiritBuffInfo_hpp
#define PopupSpiritBuffInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritParent.h"

class PopupSpiritBuffInfo : public PopupBase
{
public:
    static PopupSpiritBuffInfo* create(int nIdx);
    
    PopupSpiritBuffInfo(void);
    virtual ~PopupSpiritBuffInfo(void);
    bool init(int nIdx);
    
    
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiBottom();
    
    void onClickClose(Ref* sender);
private:
    int _nIdx;
    Layer* _lyTopUI;
    Layer* _lyBottomUI;
};
#endif /* PopupSpiritBuffInfo_hpp */
