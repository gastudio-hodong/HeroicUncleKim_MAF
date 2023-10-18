//
//  UISubMenu.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UISubMenu_h
#define UISubMenu_h

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

class UISubMenu : public cocos2d::LayerColor
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        ADS,
        STONE,
        MYHOME,
        SHOP_ITEM,
        MINE,
        TOWN,
    };
    
public:
    static UISubMenu* create(E_PLACE eType);
    
    UISubMenu(void);
    virtual ~UISubMenu(void);
    virtual bool init(E_PLACE eType);
    virtual void onEnter() override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // draw
    void drawContainer();
    void drawExpand();
    
    
    // set, get
    std::string getMenuName(E_TYPE eType);
    std::string getMenuPath(E_TYPE eType);
    std::string getMenuLockPath(E_TYPE eType);
    
    bool isReddotMyHome();
    bool isReddotAds();
    bool isReddotMine();
    
    // schedule
    
    // callback
    
    // click
    void onClickExpand(cocos2d::Ref* sender);
    void onClickMenu(cocos2d::Ref* sender);
    void onClickMenuLock(cocos2d::Ref* sender);
    
    // event
    void onExpand();
    
private:
    E_PLACE _eType;
    
    bool _bExpand;
    cocos2d::Size _sizeExpand;
    
    int _zorder;
    
    std::vector<E_TYPE> _listMenu;

    // widget
    cocos2d::ui::Layout* _uiContainer;
};

#endif /* UISubMenu_h */
