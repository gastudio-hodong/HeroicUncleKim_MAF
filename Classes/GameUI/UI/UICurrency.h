//
//  UICurrency.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UICurrency_h
#define UICurrency_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class InfoItems;
class UICurrency : public cocos2d::ui::Layout, public IRefresh
{
public:
    static UICurrency* create(std::vector<E_ITEMS> list, cocos2d::Size size);
    static UICurrency* create(cocos2d::Vector<InfoItems*> list, cocos2d::Size size);
    
    UICurrency(void);
    virtual ~UICurrency(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    // set, get
    void setList(std::vector<E_ITEMS> list);
    void setList(cocos2d::Vector<InfoItems*> list);
    
    void setEffect(bool bShow);
    void setUIPosition(Vec2 position);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    
    // draw
    void drawCurrency();
    void drawCurrencyCount();

    // click
    void onClickCurrencyIcon(cocos2d::Ref* sender);
    void onClickCurrency(cocos2d::Ref* sender);
    

private:
    bool _bInitUI;
    
    //
    cocos2d::Vector<InfoItems*> _listData;
    
    bool _bEffect;
    bool _bKeyLongShow;
    
    cocos2d::Vec2 _uiPosition;
    
    
    // widget
    cocos2d::ui::Layout* _layerContainerCurrency;
};

#endif /* UICurrency_h */
