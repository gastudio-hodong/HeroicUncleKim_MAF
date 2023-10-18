//
//  PopupBoardReward.hpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/21.
//

#ifndef PopupBoardReward_h
#define PopupBoardReward_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUIPopup/OtherEvent/EventBoardGame/PopupBoardGame.h"

class InfoItems;
class PopupBoardReward : public PopupBase
{

public:
    static PopupBoardReward* create(std::string strReward, bool bFinish);
        
    PopupBoardReward(void);
    virtual ~PopupBoardReward(void);
    virtual bool init(std::string strReward, bool bFinish);
    
private:
        // init
    void initVar();
    void initUI();
    
    //
    void uiReward();
        
        // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    std::string _strReward;
    bool _bFinish;
    
    InfoItems* _objItems;
};

#endif /* PopupBoardReward_h */
