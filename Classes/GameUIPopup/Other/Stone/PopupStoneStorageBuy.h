//
//  PopupStoneStorageBuy.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupStoneStorageBuy_hpp
#define PopupStoneStorageBuy_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupStoneStorageBuy : public PopupBase
{
public:
    CREATE_FUNC(PopupStoneStorageBuy);
    
    PopupStoneStorageBuy();
    virtual ~PopupStoneStorageBuy(void);
    virtual bool init() override;
    virtual void onEnter() override;
   
public:
    // set, get
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
     
private:
    bool _bInitUI;
    
    //
    int _count;
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsCount;
    cocos2d::ui::Layout* _uiContentsButton;
    
};

#endif /* PopupStoneStorageBuy_hpp */
