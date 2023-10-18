//
//  PopupDice.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/15.
//

#ifndef PopupDice_h
#define PopupDice_h

#include "GameUIPopup/Base/PopupBase.h"

class CellFishingQuest;
class PopupDice : public PopupBase
{

public:
    static PopupDice* create(int nDice);
        
    PopupDice(void);
    virtual ~PopupDice(void);
    virtual bool init(int nDice);
    
public:
    void setCallbackClose(std::function<void(void)> callback);
    
private:
    // init
    void initVar();
    void initUI();

    // ui
    void uiDice();
    void uiError();
    
    void redrawUI();
    void showDice();
    void showUILackPoint();

    // click
    void onClickClose(cocos2d::Ref* sender);

private:
    int _nDice;
    
    std::function<void(void)> _callbackClose;
};

#endif /* PopupDice_h */
