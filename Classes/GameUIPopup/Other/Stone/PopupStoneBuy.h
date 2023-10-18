//
//  PopupStoneBuy.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupStoneBuy_hpp
#define PopupStoneBuy_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupStoneBuy : public PopupBase
{
public:
    CREATE_FUNC(PopupStoneBuy);
    
    PopupStoneBuy();
    virtual ~PopupStoneBuy(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setStone(E_STONE eStone);
    void setCountChange(int change);
    
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiInfo();
    void uiCount();
    void uiButton();
    
    // ui : draw
    void drawInfo();
    void drawCount();
    void drawButton();
    
    //
    
    //
    void onClickClose(Ref* sender);
    void onClickCountArrow(Ref* sender);
    void onClickBuy(Ref* sender);
    
    //callback
    void callbackBuy(bool bResult, int nResult);
     
private:
    bool _bInitUI;
    
    //
    E_STONE _eStone;
    int _count;
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsCount;
    cocos2d::ui::Layout* _uiContentsButton;
    
    cocos2d::ui::CText* _uiTitle;
};

#endif /* PopupStoneBuy_hpp */
