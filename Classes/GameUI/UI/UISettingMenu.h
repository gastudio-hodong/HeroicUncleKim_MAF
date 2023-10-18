//
//  UISettingMenu.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UISettingMenu_h
#define UISettingMenu_h

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

class UISettingMenu : public cocos2d::LayerColor
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        ATTEND_EVENT,
        ATTEND,
        MAIL,
        SAVE,
        STORY,
        SLEEP,
        COLLECT,
        FRIEND,
        NAVER,
        SETTING
    };
    
public:
    static UISettingMenu* create(E_PLACE eType);
    
    UISettingMenu(void);
    virtual ~UISettingMenu(void);
    virtual bool init(E_PLACE eType);
    
public:
    // event
    void onExpand();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    
    // draw
    void drawExpand();
    
    // set, get
    std::string getMenuName(E_TYPE eType);
    std::string getMenuPath(E_TYPE eType);
    
    // schedule
    
    // callback
    
    // click
    void onClickExpand(cocos2d::Ref* sender);
    void onClickMenu(cocos2d::Ref* sender);
    
    // event
    
    
private:
    E_PLACE _eType;
    
    bool _bExpand;
    cocos2d::Size _sizeExpand;
    
    int _drawCol;
    int _drawRow;
    
    int _zorder;
    
    std::vector<E_TYPE> _listMenu;
    
    

    
    // widget
    cocos2d::ui::Layout* _uiContainer;
};

#endif /* UISettingMenu_h */
