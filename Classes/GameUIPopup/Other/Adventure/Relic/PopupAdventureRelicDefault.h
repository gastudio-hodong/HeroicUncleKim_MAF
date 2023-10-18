//
//  PopupAdventureRelicMain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#ifndef PopupAdventureRelicMain_hpp
#define PopupAdventureRelicMain_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupAdventureRelicDefault : public PopupBase, public IRefresh
{
public:
    enum class E_TYPE
    {
        LIST = 1,
        CRAFTING,
        SHOP
    };
    static PopupAdventureRelicDefault* create(PopupAdventureRelicDefault::E_TYPE eType = PopupAdventureRelicDefault::E_TYPE::CRAFTING);
    
    PopupAdventureRelicDefault(void);
    virtual ~PopupAdventureRelicDefault(void);
    bool init(PopupAdventureRelicDefault::E_TYPE eType);
    
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    virtual void show() override;
    
    void showButtonForceTutorial();
    void showTutorial();
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    
    void uiReload();
    void uiLoadLayer();
    
    void onClickClose(Ref* sender);
    void onClickTab(Ref* sender);
    
    void setCloseCondition(bool value);
    void callbackHide();
    void callbackSetTab(PopupAdventureRelicDefault::E_TYPE value);
private:
    bool _isCloseCondition;
    PopupAdventureRelicDefault::E_TYPE _eType;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyContentArea;
    
    cocos2d::ui::Button* _btnTutorial;
    
    cocos2d::Layer* _lyContainerNow;
    cocos2d::Layer* _lyContainerList;
    cocos2d::Layer* _lyContainerCrafting;
    cocos2d::Layer* _lyContainerShop;
};


#endif /* PopupAdventureRelicMain_hpp */
