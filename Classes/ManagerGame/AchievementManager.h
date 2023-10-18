//
//  AchievementManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef AchievementManager_h
#define AchievementManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoItems.h"

class ObjAchievement : public cocos2d::Ref
{
public:
    CREATE_FUNC(ObjAchievement);

    ObjAchievement();
    virtual ~ObjAchievement();
    virtual bool init();

public:
    int getIdx();
    void setIdx(int value);
    
    int getType();
    void setType(int value);
    
    E_ACHIEVE_NORMAR_TYPE getNormalType();
    void setNormalType(E_ACHIEVE_NORMAR_TYPE value);
    
    E_NORMAL_ACHIEVE_MISSION getNormalMission();
    void setNormalMission(E_NORMAL_ACHIEVE_MISSION value);
    
    int getPrecedeIdx();
    void setPrecedeIdx(int value);
    
    InfoItems* getItem();
    void setRewardType(int value);
    void setTypeParams(int value);
    void setRewardCount(int value);
    
    E_SHORTCUTS getShortcuts();
    void setShortcuts(E_SHORTCUTS value);
    
    std::string getText();
    std::string getTextKey();
    void setTextKey(std::string value);
    
    int getCount();
    void setCount(int value);
    
    int getClearCount();
    void setClearCount(int value);
    
    int getCountMax();
    void setCountMax(int value);
    
    InfoItems* getReward();
    void setRewardIdx(int nIdx);
    void setRewardCount(std::string strCount);
    void setRewardTypeParams(int nTypeParams);

    int getGradeClearCount();
    int getGradeCurrentCount();
    
    int getGrade();
    void setGrade(int value);
    void addGrade();
    
    int getMaxGrade();
    
    bool isComplete();
    void setComplete(bool value);
  
    bool isCurrent();
    void setCurrent(bool value);
    
    bool isRewardCondition();
    void setRewardCondition(bool value);
    
    E_ARTIFACT_TYPE getArtifactType();
    void setArtifactType(E_ARTIFACT_TYPE value);
    
    int getArtifactGrade();
    void setArtifactGrade(int value);
    
    E_ARTIFACT getArtifactIdx();
    void setArtifactIdx(E_ARTIFACT value);

protected:
    int _nIdx;
    int _nType;
    E_ACHIEVE_NORMAR_TYPE _eNormalType;
    E_NORMAL_ACHIEVE_MISSION _eNormalMission;
    int _nPrecedeIdx;
    
    E_ITEMS _eRewardType;
    int _nTypeParams;
    int _nRewardCount;
    
    E_SHORTCUTS _eShortcuts;
    
    std::string _strTextKey;
    
    int _nCurrentCount;
    int _nClearCount;
    int _nCountMax;
    

    int _nRewardIdx;
    std::string _strRewardCount;
    int _nRewardTypeParams;

    int _nGrade;
    
    bool _bComplete;
    bool _bCurrent;
    bool _bRewardCondition;
    
    //artifact
    E_ARTIFACT_TYPE _eArtiType;
    int _nArtiGrade;
    E_ARTIFACT _eArtifact;
    
};

class AchievementManager : public MafSingleton<AchievementManager>
{    
public:
    AchievementManager(void);
    virtual ~AchievementManager(void);
    virtual bool init();
    
    void setLoadPrison();
    void setLoadNormal();
    
    void saveNormalAchieveData();
    void loadNormalAchieveData();
    void updateNormalAchieveCurrent();
    void updateNormalAchieveIsReward();
    void updateNormalAchieveIsReward(ObjAchievement* obj);
public:
    //지하감옥
    cocos2d::Vector<ObjAchievement*> getList(int nType);
    void setListConvert(int nType, std::string strList);
    
    int getCompleteRate(int nType); // 업적 달성률
    bool isReward(int nType); // 받을 완료보상 체크
    bool isCompleteReward(int nType); // 받을 완료보상 체크
    
    //일반
    void initNormalAchieveCount();
    void initNormalAchieveCount(ObjAchievement* obj);
    cocos2d::Vector<ObjAchievement*> getAchievementNormalListAll();
    cocos2d::Vector<ObjAchievement*> getAchievementNormalListFromType(E_ACHIEVE_NORMAR_TYPE value, bool isCleanUp = false);
    
    int getAchieveNormalSize();
    bool isRewardCondition();
    bool isRewardCondition(E_ACHIEVE_NORMAR_TYPE type);
    void setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION mission, int value, bool isPlus = false);
    
    ObjAchievement* getObjAchieveNormal(int idx);
    ObjAchievement* getObjAchieveNormal(E_NORMAL_ACHIEVE_MISSION mission);
    ObjAchievement* getObjAchieveNormal(E_ARTIFACT arti);
    
    void achievementNormalReward(ObjAchievement* achieve);
    void achievementNormalRewardAll(E_ACHIEVE_NORMAR_TYPE type);
    void setCallbackRedraw(const std::function<void(void)>& pCallback);
    
    
    // network
    void requestInfo(int nType, const std::function<void(bool)>& pCallback);
    void requestUpdate(int nType, int nKind, int nCount);
    void requestReward(int nType, int nIdx, const std::function<void(bool, int)>& pCallback);
    void requestComplete(int nType, const std::function<void(bool, int)>& pCallback);
    
protected:
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    void responseComplete(cocos2d::network::HttpResponse* response,std::string &data);
    
    //normal achieve Utills
    void achieveNormalRewardAllCleanUp(cocos2d::Vector<InfoItems*> rewardList);
    InfoItems* getItemToIdxFromNormalRewardAll(int idx);
    
private:
    cocos2d::Vector<ObjAchievement*> _listAchievement;
    std::map<int, bool> _listAchievementComplete;
    
    cocos2d::Vector<ObjAchievement*> _listAchievementNormal;
    
    cocos2d::Vector<InfoItems*> _vecNormalAchieveItemAll;
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool, int)> _callbackReward;
    std::function<void(bool, int)> _callbackComplete;
    
    std::function<void(void)> _callbackRedraw;
};

#endif /* AchievementManager_h */
