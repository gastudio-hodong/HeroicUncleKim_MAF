//
//  PopupFlashmanRankReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFlashmanRankReward_h
#define PopupFlashmanRankReward_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoItems;
class PopupFlashmanRankReward : public PopupBase
{
public:
    static PopupFlashmanRankReward* create();
    
    PopupFlashmanRankReward(void);
    virtual ~PopupFlashmanRankReward(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setRank(int rank);
    void setRewrad(std::string reward);
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiButtons();
    
    // ui : draw
    void drawDefault();
    void drawButtons();
    
    // set, get
    
    // callback
    
    // schedule
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    int _rank;
    cocos2d::Vector<InfoItems*> _listReward;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
};
#endif /* PopupFlashmanRankReward_h */
