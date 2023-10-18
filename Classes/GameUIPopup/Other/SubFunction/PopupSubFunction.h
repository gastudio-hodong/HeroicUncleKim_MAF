//
//  PopupSubFunction.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupSubFunction_h
#define PopupSubFunction_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupSubFunction : public PopupBase
{
public:
    enum class E_TYPE {
        NONE = 0,
        
        MINE,
        MYHOME,
        SHOPITEM,
        TOWN,
    };
    
public:
    static PopupSubFunction* create(E_TYPE eType);
    
    PopupSubFunction(void);
    virtual ~PopupSubFunction(void);
    virtual bool init(E_TYPE eType);
    virtual void onEnter() override;

public:
    void setCurrency(std::vector<E_ITEMS> list);
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    
    // ui : draw
    
    // ui : set, get
    
    // callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    E_TYPE _eType;
    cocos2d::Size _size;
    std::string _title;
    
    // widget
    cocos2d::ui::Layout* _uiContents;
    
 
};
#endif /* PopupSubFunction_h */
