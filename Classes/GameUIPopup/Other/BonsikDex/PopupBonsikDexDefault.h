//
//  PopupBonsikDexDefault.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupBonsikDexDefault_h
#define PopupBonsikDexDefault_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBonsikDexDefault : public PopupBase
{
public :
    enum class E_TYPE {
        EQUIP = 1,
        MONSTER = 2,
        LOOTS = 3,
    };
    
public:
    static PopupBonsikDexDefault* create(E_TYPE eType = E_TYPE::EQUIP);
    
    PopupBonsikDexDefault(void);
    virtual ~PopupBonsikDexDefault(void);
    virtual bool init(E_TYPE eType) ;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiMenu();
    
    //
    void drawMenu();
    
    // callback
    void callbackLootInfo(bool bResult);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickMenu(cocos2d::Ref* sender);
     
    
private:
    //
    PopupBonsikDexDefault::E_TYPE _eType;

    // widget
    cocos2d::Layer* _layerContainerMenu;
    
    cocos2d::Node* _layerContainerNow;
    cocos2d::Node* _layerContainerDexEquip;
    cocos2d::Node* _layerContainerDexMonster;
    cocos2d::Node* _layerContainerDexLoots;
};

#endif /* PopupBonsikDexDefault_h */
