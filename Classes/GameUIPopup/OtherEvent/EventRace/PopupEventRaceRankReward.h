//
//  PopupEventRaceRankReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/16.
//

#ifndef PopupEventRaceRankReward_hpp
#define PopupEventRaceRankReward_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoItems.h"

class RankInfo;
class InfoEventRaceRankReward;
class PopupEventRaceRankReward : public PopupBase
{
public:
    static PopupEventRaceRankReward* create(Vector<InfoItems*> list);
    
    PopupEventRaceRankReward();
    virtual ~PopupEventRaceRankReward(void);
    virtual bool init(Vector<InfoItems*> list);
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    void onClickClose(Ref* sender);
    
    //callback
    void callbackInfo(bool bResult, std::vector<RankInfo*>, std::vector<RankInfo*>);
private:
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyMiddle;
    cocos2d::Layer* _lyBottom;
    Vector<InfoItems*> _listReward;
};


#endif /* PopupEventRaceRankReward_hpp */
