//
//  DefenseManaAMPManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#ifndef DefenseManaAMPManager_hpp
#define DefenseManaAMPManager_hpp


#include "Libs/maf/MafSingleton.h"
#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseManaAmpStone;
class InfoDefenseManaAMP;

class DefenseManaAMPManager : public MafSingleton<DefenseManaAMPManager>
{
private:
    DefenseManaAMPManager(void);
    virtual ~DefenseManaAMPManager(void);
    friend class MafSingleton;
public:
    virtual bool init();

public:
    //load data
    void loadData();
    void setLoadManaAMPStoneData();
    void setLoadManaAMPStoneCost();
    void setLoadManaAMPOpenFloor();

    // set, get

    Vector<InfoDefenseManaAMP*> getManaAmpList() const;
    Vector<InfoDefenseManaAmpStone*> getManaAmpStoneList() const;
    int getMaxManaAMPCount();
    std::vector<int> getAMPOpenFloorList() const;

    InfoDefenseManaAMP* getManaAmpData(int idx) const;
    InfoDefenseManaAmpStone* getManaAmpStoneData(int idx) const;
    
    bool isReddotCondition() const;
    
    void updateManaAMP(const rapidjson::Document& jsonParser);
    std::string calculatorAMPPower(std::string value, bool isOrginal = false);

    //network : request
    void requestInfo(bool isShowLoading = true, const std::function<void(bool, int)>& callback = nullptr, bool isIntro = false);
    void requestChoiceStone(int idx, E_DF_AMP_STONE_Type type, bool isShowLoading = true);
    void requestResetAMP(int idx, bool isShowLoading = true);

protected:
    // network : respons
    void responseInfo(bool isShowLoading, bool isIntro, cocos2d::network::HttpResponse* response,std::string &data);
    void responseChoiceStone(int idx, E_DF_AMP_STONE_Type type, bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseResetAMP(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);

private:
    std::vector<int> _listAMPOpenFloor;
    Vector<InfoDefenseManaAMP*> _listInfoManaAmp;
    Vector<InfoDefenseManaAmpStone*> _listInfoManaAmpStome;

    std::function<void(bool, int)> _callbackInfo;
    
public:

    MafDelegate<void(bool, int)> _onServerInfo;
    MafDelegate<void(bool, int, bool, E_DF_AMP_STONE_Type)> _onServerEquipStone;
    MafDelegate<void(bool, int)> _onServerUpdateAMP;
};

#endif /* DefenseManaAMPManager_hpp */
