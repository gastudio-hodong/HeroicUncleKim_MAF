//
//  PopupToastAutoSave.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/08/18.
//

#ifndef PopupToastAutoSave_h
#define PopupToastAutoSave_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupToastAutoSave : public PopupBase
{
public:
    static PopupToastAutoSave* create();
    
    PopupToastAutoSave(void);
    virtual ~PopupToastAutoSave(void);
    virtual bool init() override;
    virtual void update(float dt) override;
    
public:
    void initUi();
    
private:
    float _timer;
};
#endif /* PopupToastAutoSave_hpp */
