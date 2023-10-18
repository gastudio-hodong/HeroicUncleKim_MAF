//
//  PopupEventVoting.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/28.
//

#ifndef PopupEventVoting_hpp
#define PopupEventVoting_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventVoting : public PopupBase
{
public:
    static PopupEventVoting* create(const int nIdx);
    
    PopupEventVoting();
    virtual ~PopupEventVoting(void);
    virtual bool init(const int nIdx);
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiTop();
    void uiVote();
    
    // ui : draw
    void drawVote();
    
    void onClickClose(Ref* sender);
    void onClickIncrease(Ref* sender);
    void onClickVote(Ref* sender);
    
    //callback
private:
    bool _bInitUI;
    int _nIdx;
    int _nCount;
    
    cocos2d::ui::Layout* _uiTop;
    cocos2d::ui::Layout* _uiVote;
};

#endif /* PopupEventVoting_hpp */
