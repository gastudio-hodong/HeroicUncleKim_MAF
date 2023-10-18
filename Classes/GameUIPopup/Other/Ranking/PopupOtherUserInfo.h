//
//  PopupOtherUserInfo.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/18.
//

#ifndef PopupOtherUserInfo_h
#define PopupOtherUserInfo_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoBadge.h"
#include "GameObject/Raid/InfoWeaponSpirit.h"

struct tagArtiInfo
{
    tagArtiInfo():
    _type(E_ARTIFACT_TYPE::ARTI_TYPE_NONE),
    _level(0),
    _generation(0),
    _strImgPath("")
    {
    }
    ~tagArtiInfo()
    {
    }
    E_ARTIFACT_TYPE _type;
    int _level;
    int _generation;
    std::string _strImgPath;
};

class InfoDefenseManaAMP;
class PopupOtherUserInfo : public PopupBase
{
public:
    static PopupOtherUserInfo* create(int userIdx, std::string platform, std::string nick);
    
    PopupOtherUserInfo();
    
    virtual ~PopupOtherUserInfo();
    
    virtual bool init(int userIdx, std::string platform, std::string nick);
    
    // init
    void initVar();
    void initUI();
    void initTopUI();
    void initArtifactUI();
    void initBadgeUI();
    void initButtonUI();
    
    // network after
    void afterInit();
    void afterInitTopUI();
    void afterInitBottomUI();
    
    //network
    void requestUserInfo();
    void responsUserInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    // callback
    void callbackFriednAddRequest(bool bResult, int nResult);
    
    //utils
    void onClick(Ref* sender);
    void onClickReport(Ref* sender);
    void onClickFriendAdd(Ref* sender);
    void onClickManaAMP(Ref* sender);
    
    tagArtiInfo* getArtiInfoFromType(E_ARTIFACT_TYPE type);
    
    void setEnchantStoneLv(E_STONE eType, int nLv);
    int getEnchantStoneLv(E_STONE eType);
    
    void setBuildingLv(E_TOWN eType, int nLv);
    int getBuildingLv(E_TOWN eType);
private:
    
    int _userIdx;
    int _highFloor;
    int _totalFloor;
    int _skinIdx;
    int _weaponIdx;
    int _totalCostumeNum;
    int _defenseHighFloor;
    std::map<E_COSTUME, E_COSTUME_IDX> _mapCostume;
    std::vector<tagArtiInfo*> _vecArtifact;
    cocos2d::Vector<InfoBadgeHave*> _vecBadge;
    std::string _platform;
    std::string _nick;
    std::string _startDay;
    
    InfoWeaponSpirit* _infoSpirit;
    
    cocos2d::Label* _lbHighFloor;
    cocos2d::Label* _lbTotalFloor;
    cocos2d::Label* _lbStarDay;
    cocos2d::Label* _lbTotalCostume;
    
    Layer* _lyUserInfoArea;
    Layer* _lyArtifactArea;
    Layer* _lyBadgeArea;
    Layer* _lyButtonArea;
    
    bool _isInfinite;
    int _nInfiniteGrade;
    std::vector<int> _vecInfiniteArtiLv;
    std::vector<std::pair<E_STONE, int>> _vecEnchantStoneLv;
    std::vector<std::pair<E_TOWN, int>> _vecBuildingLv;
    int _nBongTowerFloor;
    
    Vector<InfoDefenseManaAMP*> _listManaAMP;
    std::vector<cocos2d::Sprite*> _vecSprArtiBox;
};

#endif /* PopupOtherUserInfo_h */
