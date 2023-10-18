//
//  ShopItemManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef ShopItemManager_h
#define ShopItemManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Event/InfoEvent7th.h"
#include "GameObject/Event/InfoEvent7thPerson.h"

class ShopItemManager : public MafSingleton<ShopItemManager>
{
public:
    ShopItemManager(void);
    virtual ~ShopItemManager(void);
    virtual bool init();
    
public:
    // load
    
    // update
    void update(float dt);
    
    // get, set
    std::string getPathIcon(E_SHOP eType);
    std::string getName(E_SHOP eType);
    std::string getDesc(E_SHOP eType);
    
    int getBuyPrice(E_SHOP eType);
 
    
    // event
    int onBuy(E_SHOP eType);
    int onBuyGold(E_SHOP eType);
    
    // network : request
    
protected:
    // network : respons
    
private: 
    double _timeUpdate;
    
    // callback
    
    
};


#endif /* ShopItemManager_hpp */
