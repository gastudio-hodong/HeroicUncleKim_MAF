//
//  PopupEventRaceTitle.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/04.
//

#ifndef PopupEventRaceTitle_hpp
#define PopupEventRaceTitle_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "ui/CocosGUI.h"

class PopupEventRaceTitle : public PopupBase
{
public:
    static PopupEventRaceTitle* create();
    
    PopupEventRaceTitle();
    virtual ~PopupEventRaceTitle(void);
    virtual bool init() override;
protected:
    void initVar();
    void initUI();
    
    void onClickStart(Ref* sender);
    void onClickClose(Ref* sender);
    
private:
    
};


#endif /* PopupEventRaceTitle_hpp */
