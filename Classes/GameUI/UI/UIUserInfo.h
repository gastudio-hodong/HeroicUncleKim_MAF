//
//  UIUserInfo.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UIUserInfo_h
#define UIUserInfo_h

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

class UIUserInfo : public cocos2d::LayerColor, public IRefresh
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
    static UIUserInfo* create(E_PLACE eType);
    
    UIUserInfo(void);
    virtual ~UIUserInfo(void);
    virtual bool init(E_PLACE eType);

    // override : refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    

protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    
    // draw
    void drawContainer();
    
    // set, get
    
    // schedule
    
    // callback
    void callbackRank(bool Result);
    
    // click
    void onClickStat(cocos2d::Ref* sender);
    void onClickRank(cocos2d::Ref* sender);
    
    // event
    void onExpand();
    
private:
    E_PLACE _eType;

    // widget
    cocos2d::ui::Layout* _uiContainer;
  
    cocos2d::ui::ImageView* _uiImgSkinBG;
    cocos2d::ui::CText* _uiTextRank;
};

#endif /* UIUserInfo_h */
