//
//  RefreshManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef RefreshManager_h
#define RefreshManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigEnum.h"

 class RefreshManager : public MafSingleton<RefreshManager>
{    
public:
    RefreshManager(void);
    virtual ~RefreshManager(void);
    virtual bool init();
     
public:
    void addUI(E_REFRESH eType, cocos2d::Node* node);
    void refreshUI(E_REFRESH eType, cocos2d::ValueMap listData = cocos2d::ValueMap());
    void refreshCurrencyUI(E_ITEMS eType, std::string strCount, bool bPet = false);
};

#endif /* RefreshManager_h */
