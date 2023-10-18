//
//  PopupPetEquipUseMaxWarning.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/05/24.
//

#ifndef PopupPetEquipUseMaxWarning_hpp
#define PopupPetEquipUseMaxWarning_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoProduct.h"

class PopupPetEquipUseMaxWarning : public PopupBase
{
public:
    static PopupPetEquipUseMaxWarning* create(int nUse, int nType);
        
    PopupPetEquipUseMaxWarning();
    virtual ~PopupPetEquipUseMaxWarning(void);
    virtual bool init(int nUse, int nType);
    virtual void onEnter() override;
        // init
    void initVar();
    void initUI();
    
    void onClick(cocos2d::Ref* sender);
    
    void onHide();
    
    void setCallback(std::function<void(int, int)> pCallback);
    
private:
    int _nUse;
    int _nType;
    
    bool _bCheck;
    
    std::function<void(int, int)> _callback;
};

#endif /* PopupPetEquipUseMaxWarning_hpp */
