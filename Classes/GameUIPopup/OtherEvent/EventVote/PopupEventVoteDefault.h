//
//  PopupEventVoteDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupEventVoteDefault_hpp
#define PopupEventVoteDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventVoteDefault : public PopupBase
{
public:
    static PopupEventVoteDefault* create();
    
    PopupEventVoteDefault();
    virtual ~PopupEventVoteDefault(void);
    virtual bool init() override;
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiContainerNetwork();
    void uiTop();
    
    // ui : draw
    void drawTop();
    void drawParticipation();
    void drawVote();
    
    void onClickClose(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickPrevVote(Ref* sender);
    void onClickAds(Ref* sender);
    
    //callback
    void callbackInfo(const bool bResult, const int nResult);
    void callbackVote(const bool bResult, const int nResult);
    void callbackAds(const bool bResult, const int nResult);
    
    void callBackResultAds(ADS_RESULT result);
private:
    bool _bInitUI;
    bool _bInitNetworkInfo;
    
    cocos2d::ui::Layout* _uiTop;
    cocos2d::ui::Layout* _uiParticipation;
    cocos2d::ui::Layout* _uiVote;
};

#endif /* PopupEventVoteDefault_hpp */
