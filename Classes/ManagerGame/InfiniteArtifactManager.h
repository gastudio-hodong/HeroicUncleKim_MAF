//
//  InfiniteArtifactManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/26.
//

#ifndef InfiniteArtifactManager_h
#define InfiniteArtifactManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

static const int INFINITE_ARTIFACT_KIND = 5;



struct InfiniteBasicInfo
{
private:
    int _grade;
    int _maxLv;
    
    int _point;
    int _requireCoin;
    
    double _increaseA;
    int _increaseB;
    
public:
    InfiniteBasicInfo(int grade, int maxLv, int point, int requireCoin, double increaseA, int increaseB);
    ~InfiniteBasicInfo();
    
    int getGrade();
    int getMaxLv();
    int getDemandPoint();
    int getDemandCoin();
    double getIncreaseA();
    int getIncreaseB();
};

class InfiniteArtifactInfo
{
private:
    InfiniteBasicInfo* _basicInfo;
    INFINITE_ARTIFACT_TYPE _type;
    int _currentLv;
    bool _isCurrent;
    bool _isMax;
    double _persent;
    
public:
    InfiniteArtifactInfo(INFINITE_ARTIFACT_TYPE type, InfiniteBasicInfo* basicInfo);
    ~InfiniteArtifactInfo();
    void init();
    
    //get
    int getGrade();
    INFINITE_ARTIFACT_TYPE getType();
    int getDemandPoint();
    int getCurLv();
    int getMaxLv();
    int getDemandCoin();
    double getIncreaseA();
    int getIncreaseB();
    
    bool isCurrent();
    
    double getEfficiency(int plus = 0);
    void setMaxEfficiency();
    
    std::string getName();
    std::string getDesc();
    std::string getIconPath();
    
    
    //set
    /*!
     * @breif 직접 사용 X 매니저 -> setInfiniteArtifactLevel 함수 사용
     */
    void setCurLv(int value);
    void setIsCurrent(bool active);
};


class InfiniteArtifactManager : public MafSingleton<InfiniteArtifactManager>
{
public:
    InfiniteArtifactManager();
    virtual ~InfiniteArtifactManager(void);
    bool init();
    
    void initInfoData();
    void createArtifactInfo();

    
public:
//    static std::string getInfiniteArtiUpgradeKey(int grade, int level, INFINITE_ARTIFACT_TYPE type);
    
    static double getInfiniteArtiUpgradeKey(int grade, double increaseA, int increaseB, int level, INFINITE_ARTIFACT_TYPE type, bool isDouble);
    
    //get set
    std::vector<InfiniteArtifactInfo*> getInfiniteArtiInfoAll();
    std::vector<InfiniteArtifactInfo*> getInfiniteArtiInfoFromGrade(int grade);
    std::vector<InfiniteArtifactInfo*> getInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE type);
    
    InfiniteArtifactInfo* getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE type);
    
    InfiniteBasicInfo* getInfiniteBasicInfoFromGrade(int grade);
    
    std::string getInfiniteArtifactData();
    
    std::string getInfiniteArtiCalculation(std::string value, INFINITE_ARTIFACT_TYPE type);
    
    

    int getCurGrade();
    int getMaxGrade();
    bool isOpenCondition();
    bool isActive();
    bool isFirst();
    bool isUpgradeCondition();
    bool isView();
    bool isUpgradeConditionToRiviveKeyArti();
    void setView(bool active);
    void setFirstUpgrade(bool active);
    
    //save load
    void loadInfiniteArtiData();
    void saveInfiniteArtiData();
    
    //network
    void requestInfiniteArtiInfoList(const std::function<void(bool)>& pCallback);
    void responseInfiniteArtiInfoList(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestInfiniteArtiBuy(const std::function<void(bool)>& pCallback);
    void responseInfiniteArtiBuy(cocos2d::network::HttpResponse* response,std::string &data);
    
    //util
    void renewalArtifact();
    void addNewGrade(int grade);
    void addNewInfiniteArtifact(INFINITE_ARTIFACT_TYPE type, InfiniteBasicInfo* basicInfo);
    
    void setInfiniteArtifactLevel(InfiniteArtifactInfo* info, int lv);
        
private:
        
    std::vector<InfiniteArtifactInfo*> _vecInfiniteArtifactInfo;
    std::vector<InfiniteBasicInfo*> _vecInfiniteArtifactBasicInfo;
        
    int _curGrade;
    int _maxGrade;
    bool _openCondition;
    bool _isActive;
    bool _isFirst;
    bool _isInfiniteArtiView;
    
    int _curAtkLv;
    int _curCriLv;
    int _curQuestLv;
    int _curKillLv;
    int _curCrossLv;
    std::function<void(bool)> _callbackArtiBuy;
};

#endif /* InfiniteArtifactManager_h */
