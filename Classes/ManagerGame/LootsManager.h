//
//  LootsManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/07.
//

#ifndef LootsManager_hpp
#define LootsManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoLoot.h"

class LootsManager : public MafSingleton<LootsManager>
{
public:
    LootsManager(void);
    virtual ~LootsManager(void);
    virtual bool init();
    
public:
    
    // load
    void setLoad();
    void setLoadLoots();
    
    // info
    cocos2d::Vector<InfoLoot*> getListInfoLoots();
    InfoLoot* getInfoLoot(int nIdx);
    
    
    // set, get :
    double getEffect();
    
    int getLootCountNow(int idx);
    int getLootCountMax(int grade, int level);
    
    int getLootLevelNow(int idx);
    bool isLootLevelUPPossible();
    
    E_ITEMS getGradeToItemsType(int grade);
    
    // network
    void requestLootsInfo(bool bPopupLoading, const std::function<void(bool)>& callback);
    void requestLootsLevelUp(bool bPopupLoading, int idx, int levelNow, const std::function<void(bool,int)>& callback);
    void requestLootPieceSynthesize(bool bPopupLoading, int grade, int count, const std::function<void(bool,int)>& callback);
    
    // event
    int onLootsLevelUp(int idx, const std::function<void(bool, int)>& callback);
    int onLootPieceSynthesize(int grade, int count, const std::function<void(bool, int)>& callback);
    
protected:
    // network
    void responseLootsInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseLootsLevelUp(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseLootPieceSynthesize(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    cocos2d::Vector<InfoLoot*> _listInfoLoots;
    
    //
    std::map<int, TInteger> _listLootCount;
    std::map<int, TInteger> _listLootLevel;
    
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool, int)> _callbackLevelUp;
    std::function<void(bool, int)> _callbackSynthesize;

};

#endif /* LootsManager_hpp */
