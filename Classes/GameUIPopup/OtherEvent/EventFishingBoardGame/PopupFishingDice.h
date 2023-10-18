//
//  PopupFishingDice.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#ifndef PopupFishingDice_hpp
#define PopupFishingDice_hpp

#include "GameUIPopup/Base/PopupBase.h"

class CellFishingQuest;
class PopupFishingDice : public PopupBase
{

public:
    static PopupFishingDice* create(int nDice);
        
    PopupFishingDice(void);
    virtual ~PopupFishingDice(void);
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
    void onClickFishing(cocos2d::Ref* sender);

private:
    int _nDice;
    
    std::function<void(void)> _callbackClose;
};

#endif /* PopupFishingDice_hpp */
