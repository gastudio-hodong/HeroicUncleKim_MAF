//
//  PopupFishingBoardReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#ifndef PopupFishingBoardReward_hpp
#define PopupFishingBoardReward_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupFishingBoardGame.h"

class InfoItems;
class PopupFishingBoardReward : public PopupBase
{

public:
    static PopupFishingBoardReward* create(std::string strReward, bool bFinish);
        
    PopupFishingBoardReward(void);
    virtual ~PopupFishingBoardReward(void);
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

#endif /* PopupFishingBoardReward_hpp */
