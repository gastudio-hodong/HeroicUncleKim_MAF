//
//  PopupToastPet.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupToastPet_h
#define PopupToastPet_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class PopupToastPet : public cocos2d::LayerColor
{
public:
    static PopupToastPet* create(E_PET ePet);
    
    PopupToastPet(void);
    virtual ~PopupToastPet(void);
    virtual bool init(E_PET ePet);
    
public:
    static void show(E_PET ePet);
    
protected:
    
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiPet();
    
private:
    static int _nCountVisible;
    
    E_PLACE _ePlace;
    E_PET _ePet;
    
    
    //
    cocos2d::Layer* _layerContainerPet;
    
};

#endif /* PopupToastPet_h */
