//
//  PopupManaAMPReset.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/24.
//

#ifndef PopupManaAMPReset_hpp
#define PopupManaAMPReset_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupManaAMPReset : public PopupBase
{
public:
    static PopupManaAMPReset* create(int number);
    
    PopupManaAMPReset();
    virtual ~PopupManaAMPReset(void);
    virtual bool init(int number);
protected:
    void initVar();
    void initUI();
    
    void uiTop();
    void uiBottom();
    
    void onClickClose(Ref* sender);
    void onClickReset(Ref* sender);
    
private:
    int _number;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyBottom;
    
public:
    MafDelegate<void(void)> _onReset;
};


#endif /* PopupManaAMPReset_hpp */
