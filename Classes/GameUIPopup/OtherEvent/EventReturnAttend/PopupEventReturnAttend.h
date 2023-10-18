//
//  PopupEventReturnAttend.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/13.
//

#ifndef PopupEventReturnAttend_hpp
#define PopupEventReturnAttend_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Event/InfoEventReturnAttendReward.h"

class PopupEventReturnAttend : public PopupBase
{
public:
    static PopupEventReturnAttend* create();
    
    PopupEventReturnAttend(void);
    virtual ~PopupEventReturnAttend(void);
    virtual void onEnter() override;
    virtual bool init() override;
    
public:
    void initVar();
    void initUI();
    
    void uiRefresh();
    void uiTopBanner();
    void uiPopupTable();
    void uiPopupBottom();
    //click
    void onClickClose(Ref* const sender);
    
    //network
    void requestInfo();
    void responseInfo(cocos2d::network::HttpResponse* const response, const std::string &data);
    
    //callback
    void callbackInfo(const bool bResult, const int nResult);
    void callbackReward(const bool bResult, const int nResult);
private:
    bool _isRewardCondition;
    int _nTodayReward;
    int _nEndTime;
    cocos2d::Layer* _lyTopBanner;
    cocos2d::Layer* _lyReward;
    cocos2d::Layer* _lyPopupBottomUI;
    
    cocos2d::Vector<InfoEventReturnAttendReward*> _listInfoReward;
};

#endif /* PopupEventReturnAttend_hpp */
