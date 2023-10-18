//
//  PopupPetEquipBoxBuyPurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/05/24.
//

#ifndef PopupPetEquipBoxBuyPurchase_h
#define PopupPetEquipBoxBuyPurchase_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoProduct;
class PopupPetEquipBoxBuyPurchase : public PopupBase
{
public:
    static PopupPetEquipBoxBuyPurchase* create();
        
    PopupPetEquipBoxBuyPurchase();
    virtual ~PopupPetEquipBoxBuyPurchase(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
        // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiList();
    
    // draw
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPurchase(cocos2d::Ref* sender);
    
    // callback
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
    
    void buyPurchase(int idx);
    
    
private:
    bool _bInitUI;

    //
    cocos2d::Vector<InfoProduct*> _listProduct;
    
    cocos2d::Size _sizeList;

    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerList;
    
};

#endif /* PopupPetEquipBoxBuyPurchase_h */
