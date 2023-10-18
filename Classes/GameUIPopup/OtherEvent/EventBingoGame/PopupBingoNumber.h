//
//  PopupBingoNumber.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/27.
//

#ifndef PopupBingoNumber_hpp
#define PopupBingoNumber_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoItems;
class PopupBingoNumber : public PopupBase
{
public:
    static PopupBingoNumber* create(int nNum, cocos2d::Vector<InfoItems*> listReward);
    
    PopupBingoNumber(void);
    virtual ~PopupBingoNumber(void);
    virtual bool init(int nNum, cocos2d::Vector<InfoItems*> listReward);
    virtual void update(float dt) override;
    
public:
    void initVar();
    void initUi();
    
    void uiContent();
    
    void onHide();
    void onClickClose(Ref* pSender);
    
    void setCallback(const std::function<void(void)>& pCallback);
    
private:
    int _nNum;
    float _timer;
    cocos2d::Vector<InfoItems*> _listReward;
    cocos2d::Label* _lbGuide;
    
    std::function<void(void)> _callbackClose;
};

#endif /* PopupBingoNumber_hpp */
