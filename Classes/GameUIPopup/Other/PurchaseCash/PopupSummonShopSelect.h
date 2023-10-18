//
//  PopupSummonShopSelect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/01.
//

#ifndef PopupSummonShopSelect_hpp
#define PopupSummonShopSelect_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoDefenseManaAMP;
class PopupSummonShopSelect : public PopupBase
{
public:
    static PopupSummonShopSelect* create();
    
    PopupSummonShopSelect();
    virtual ~PopupSummonShopSelect(void);
    virtual bool init();
protected:
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMain();
    
    void onClickClose(Ref* sender);
    void onClickDefenseShop(Ref* sender);
    void onClickRaidShop(Ref* sender);
    
private:
    int _number;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyMain;
    
public:
};



#endif /* PopupSummonShopSelect_hpp */
