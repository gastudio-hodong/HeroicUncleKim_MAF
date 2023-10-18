//
//  ItemsManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef ItemsManager_h
#define ItemsManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoItems.h"

class ItemsManager : public MafSingleton<ItemsManager>
{
public:
    ItemsManager(void);
    virtual ~ItemsManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    
    // parameter
    int getParameterArtifact();
    void setParameterArtifact(int nData);
    
    //
    InfoItems* getConvert(std::string strItem);
    cocos2d::Vector<InfoItems*> getConvertArray(std::string strItems);
    
    //
    InfoItems* getInfoItems(int nIdx);
    InfoItems* getInfoItems(E_ITEMS eType);
    
    //
    void addItems(std::string strRewardJson, bool bToast = true);
    void addItems(cocos2d::Vector<InfoItems*> listReward, bool bToast = true);
    void addItem(int nIdx, int nTypeParams, int nCount, bool bToast = true);
    void addItem(int nIdx, int nTypeParams, std::string strCount, bool bToast = true);
    void addItem(E_ITEMS eType, int nTypeParams, int nCount, bool bToast = true);
    void addItem(E_ITEMS eType, int nTypeParams, std::string strCount, bool bToast = true);
    void addItem(InfoItems* objItem, bool bToast = true);
    
    void subItem(InfoItems* objItem);
    
    //
    void setItemsServer(std::string strItemsJson);
    void setItemsServer(E_ITEMS eType, int nTypeParams, int nCount);
    void setItemsServer(E_ITEMS eType, int nTypeParams, std::string strCount);
    void setItemsServer(InfoItems* objItems);
    
    //
    std::string getItems(E_ITEMS eType);
    
private:
    void addItemsApply(InfoItems* objReward);
    void subItemsApply(InfoItems* objReward);
    
    
    
private:
    cocos2d::Vector<InfoItems*> _listInfoItems;
    
    std::map<int, std::string> _listItems;
    
    int _nParameterArtifact;
   
};

#endif /* ItemsManager_h */
