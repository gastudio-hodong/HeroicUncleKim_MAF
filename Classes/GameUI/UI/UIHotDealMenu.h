//
//  UIHotDealMenu.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UIHotDealMenu_h
#define UIHotDealMenu_h

#include "GameUIPopup/Other/TimingPackage/BaseTimingPackageManagement.h"

class UIHotDealMenu : public BaseTimingPackageManagement
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        ADS,
        ATTEND,
        MYHOME,
        SHOP_ITEM,
        MINE,
        TOWN,
    };
    
public:
    static UIHotDealMenu* create(E_PLACE eType);
    
    UIHotDealMenu(void);
    virtual ~UIHotDealMenu(void);
    virtual bool init(E_PLACE eType);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // draw
    void drawContainer() override;
    void drawExpand() override;
    
    // set, get
    void setMenu() override;
    
    // schedule
    
    // callback
    void callbackReload();
    
    // click
    void onClickExpand(cocos2d::Ref* sender);
    
    // event
    void onExpand() override;
    
private:
    E_PLACE _eType;
    
    bool _bExpand;
    double _widthExpand;
    int _zorder;
    
    cocos2d::Vector<cocos2d::Node*> _listMenu;

    // widget
    cocos2d::ui::Layout* _uiContainer;
};

#endif /* UIHotDealMenu_h */
