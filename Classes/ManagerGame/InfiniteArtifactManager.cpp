//
//  InfiniteArtifactManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/26.
//


#include "InfiniteArtifactManager.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUIPopup/Other/ArtifactInfinite/PopupUpgradeInfiniteArtifact.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

#pragma mark - InfiniteBasicInfo

InfiniteBasicInfo::InfiniteBasicInfo(int grade, int maxLv, int point, int requireCoin, double increaseA, int increaseB):
_grade(grade),
_maxLv(maxLv),
_point(point),
_requireCoin(requireCoin),
_increaseA(increaseA),
_increaseB(increaseB)
{
    
}

InfiniteBasicInfo::~InfiniteBasicInfo()
{
    
}

int InfiniteBasicInfo::getGrade()
{
    return _grade;
}

int InfiniteBasicInfo::getDemandPoint()
{
    return _point;
}
int InfiniteBasicInfo::getMaxLv()
{
    return _maxLv;
}

int InfiniteBasicInfo::getDemandCoin()
{
    return _requireCoin;
}

double InfiniteBasicInfo::getIncreaseA()
{
    return _increaseA;
}

int InfiniteBasicInfo::getIncreaseB()
{
    return _increaseB;
}



#pragma mark - InfiniteArtifactInfo
InfiniteArtifactInfo::InfiniteArtifactInfo(INFINITE_ARTIFACT_TYPE type, InfiniteBasicInfo* basicInfo):
_basicInfo(basicInfo),
_type(type),
_isCurrent(false),
_isMax(false),
_currentLv(0),
_persent(0)
{
    
}

InfiniteArtifactInfo::~InfiniteArtifactInfo()
{
    
}

#pragma mark - InfoGet
int InfiniteArtifactInfo::getGrade()
{
    return _basicInfo->getGrade();
}

INFINITE_ARTIFACT_TYPE InfiniteArtifactInfo::getType()
{
    return _type;
}


int InfiniteArtifactInfo::getDemandPoint()
{
    return _basicInfo->getDemandPoint();
}

int InfiniteArtifactInfo::getCurLv()
{
    int result = _currentLv;
    
    if(_isMax)
        result = getMaxLv();
    
    return result;
}

int InfiniteArtifactInfo::getMaxLv()
{
    return _basicInfo->getMaxLv();
}

int InfiniteArtifactInfo::getDemandCoin()
{
    return _basicInfo->getDemandCoin();
}

double InfiniteArtifactInfo::getIncreaseA()
{
    return _basicInfo->getIncreaseA();
}

int InfiniteArtifactInfo::getIncreaseB()
{
    return _basicInfo->getIncreaseB();
}

bool InfiniteArtifactInfo::isCurrent()
{
    return _isCurrent;
}

double InfiniteArtifactInfo::getEfficiency(int plus)
{
    if(_isMax)
        return _persent;//저장할 방법을 찾자
    
    double attack = 0.0f;
    double value = 1.0;
    double up = 1.0;
    int evol = 50;
    int evol2 = 5000;
            
    for (int i = 1; i <= _currentLv + plus; i++)
    {
        if (i > 1)
        {
            if ((i - 1) % evol2 == 0)
                up *= 2;
            if ((i - 1) % evol == 0)
                value += up;
        }
        attack += value;
    }
        
    return attack;
}

void InfiniteArtifactInfo::setMaxEfficiency()
{
    double attack = 0.0f;
    double value = 1.0;
    double up = 1.0;
    int evol = 50;
    int evol2 = 5000;
            
    for (int i = 1; i <= getMaxLv(); i++)
    {
        if (i > 1)
        {
            if ((i - 1) % evol2 == 0)
                up *= 2;
            if ((i - 1) % evol == 0)
                value += up;
        }
        attack += value;
    }
        
    _persent = attack;
}


std::string InfiniteArtifactInfo::getName()
{
    std::string key = "";
    if ( _basicInfo->getGrade() > 7 )
    {
        switch(_type)
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK :       key = "t_arti_d1_unlimit_title_1_up1"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI :       key = "t_arti_d1_unlimit_title_2_up1"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST :     key = "t_arti_d1_unlimit_title_3_up1"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL :      key = "t_arti_d1_unlimit_title_4_up1"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS :     key = "t_arti_d1_unlimit_title_5_up1"; break;
                
            default:
                break;
        }
    }
    else
    {
        switch(_type)
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK :       key = "t_arti_d1_unlimit_title_1"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI :       key = "t_arti_d1_unlimit_title_2"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST :     key = "t_arti_d1_unlimit_title_3"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL :      key = "t_arti_d1_unlimit_title_4"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS :     key = "t_arti_d1_unlimit_title_5"; break;
                
            default:
                break;
        }
    }
    
    return GAME_TEXT(key);
}

std::string InfiniteArtifactInfo::getDesc()
{
    std::string key = "";
    switch(_type)
    {
        case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK :       key = "t_arti_d1_unlimit_desc_1"; break;
        case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI :       key = "t_arti_d1_unlimit_desc_2"; break;
        case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST :     key = "t_arti_d1_unlimit_desc_3"; break;
        case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL :      key = "t_arti_d1_unlimit_desc_4"; break;
        case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS :     key = "t_arti_d1_unlimit_desc_5"; break;
            
        default:
            break;
    }
    
    return GAME_TEXT(key);
}

std::string InfiniteArtifactInfo::getIconPath()
{
    std::string path = "";
    if ( _basicInfo->getGrade() > 7 )
    {
        switch(_type)
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK :       path = "Assets/icon_artifact/artifact_113.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI :       path = "Assets/icon_artifact/artifact_114.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST :     path = "Assets/icon_artifact/artifact_115.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL :      path = "Assets/icon_artifact/artifact_116.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS :     path = "Assets/icon_artifact/artifact_117.png"; break;

            default:
                break;
        }
    }
    else
    {
        switch(_type)
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK :       path = "Assets/icon_artifact/artifact_104.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI :       path = "Assets/icon_artifact/artifact_105.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST :     path = "Assets/icon_artifact/artifact_106.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL :      path = "Assets/icon_artifact/artifact_107.png"; break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS :     path = "Assets/icon_artifact/artifact_108.png"; break;

            default:
                break;
        }
    }
    
    return path;
}

#pragma mark - InfoSet
void InfiniteArtifactInfo::setCurLv(int value)
{
    _currentLv = value;
    if(_currentLv >= getMaxLv())
    {
        _currentLv = getMaxLv();
        setMaxEfficiency();
        _isMax = true;
    }
}

void InfiniteArtifactInfo::setIsCurrent(bool active)
{
    _isCurrent = active;
}

#pragma mark - InfiniteArtifactManager
InfiniteArtifactManager::InfiniteArtifactManager():
_curGrade(0),
_maxGrade(0),
_openCondition(false),
_isActive(false),
_isFirst(true),
_isInfiniteArtiView(false),
_curAtkLv(0),
_curCriLv(0),
_curQuestLv(0),
_curKillLv(0),
_curCrossLv(0),
_callbackArtiBuy(nullptr)
{
    
}

InfiniteArtifactManager::~InfiniteArtifactManager()
{
    for(auto obj : _vecInfiniteArtifactInfo)
        delete obj;
    for(auto obj : _vecInfiniteArtifactBasicInfo)
        delete obj;
    
    _vecInfiniteArtifactInfo.clear();
    _vecInfiniteArtifactBasicInfo.clear();
}

#pragma mark - init

bool InfiniteArtifactManager::init()
{
    bool result = true;
    
    
    // clear
    _vecInfiniteArtifactInfo.clear();
    _vecInfiniteArtifactBasicInfo.clear();
    
    
    initInfoData();
    loadInfiniteArtiData();
    createArtifactInfo();
    saveInfiniteArtiData();
    
    return result;
}

void InfiniteArtifactManager::initInfoData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::INFINITE_ARTIFACT_INFO);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }

        int grade = jsonValue["grade"].GetInt();
        int maxLv = jsonValue["max"].GetInt();
        int point = jsonValue["point"].GetInt();
        int requireCoin = jsonValue["require_coin"].GetInt();
        double increaseA = jsonValue["increase_A"].GetDouble();
        int increaseB = jsonValue["increase_B"].GetInt();
        
        _vecInfiniteArtifactBasicInfo.push_back(new InfiniteBasicInfo(grade, maxLv, point, requireCoin, increaseA, increaseB));
    }
    _maxGrade = (int)_vecInfiniteArtifactBasicInfo.size();
}

void InfiniteArtifactManager::createArtifactInfo()
{
    for(int i = 1; i <= _curGrade; ++i)
    {
        auto basicInfo = getInfiniteBasicInfoFromGrade(i);
        if(basicInfo == nullptr)
            continue;
        for(int j = 1; j <= INFINITE_ARTIFACT_KIND; ++j)
        {
            auto info = new InfiniteArtifactInfo((INFINITE_ARTIFACT_TYPE)j, basicInfo);
            
            if(_curGrade == i)
                info->setIsCurrent(true);
           else if(_curGrade > info->getGrade())
                info->setCurLv(info->getMaxLv());
            
            _vecInfiniteArtifactInfo.push_back(info);
        }
    }
    
    auto info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
    if(info != nullptr) info->setCurLv(_curAtkLv);
    
    info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
    if(info != nullptr) info->setCurLv(_curCriLv);
    
    info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
    if(info != nullptr) info->setCurLv(_curQuestLv);
    
    info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
    if(info != nullptr) info->setCurLv(_curKillLv);
    
    info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
    if(info != nullptr) info->setCurLv(_curCrossLv);
}

#pragma mark - get

std::vector<InfiniteArtifactInfo*> InfiniteArtifactManager::getInfiniteArtiInfoAll()
{
    return _vecInfiniteArtifactInfo;
}

std::vector<InfiniteArtifactInfo*> InfiniteArtifactManager::getInfiniteArtiInfoFromGrade(int grade)
{
    
    std::vector<InfiniteArtifactInfo*> _vecResult;
    _vecResult.clear();
    
    for(auto info : _vecInfiniteArtifactInfo)
    {
        if(info->getGrade() == grade)
            _vecResult.push_back(info);
    }
    
    return _vecResult;
}

std::vector<InfiniteArtifactInfo*> InfiniteArtifactManager::getInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE type)
{
    
    std::vector<InfiniteArtifactInfo*> _vecResult;
    _vecResult.clear();
    
    for(auto info : _vecInfiniteArtifactInfo)
    {
        if(info->getType() == type)
            _vecResult.push_back(info);
    }
    
    return _vecResult;
}

InfiniteArtifactInfo* InfiniteArtifactManager::getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE type)
{
    InfiniteArtifactInfo* result = nullptr;
    
    for(auto info : _vecInfiniteArtifactInfo)
    {
        if(_curGrade > _maxGrade)
        {
            if(info->getGrade() == _maxGrade && info->getType() == type)
                result = info;
        }
        else
        {
            if(info->getGrade() == _curGrade && info->getType() == type)
                result = info;
        }
    }
    
    return result;
}

InfiniteBasicInfo* InfiniteArtifactManager::getInfiniteBasicInfoFromGrade(int grade)
{
    InfiniteBasicInfo* result = nullptr;
    
    for(auto info : _vecInfiniteArtifactBasicInfo)
    {
        if(info->getGrade() == grade)
            result = info;
    }
    
    return result;
}

std::string InfiniteArtifactManager::getInfiniteArtifactData()
{
    std::string strResult = "";
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_INFINITE_ARTI, "");
    strData = MafAes256::Decrypt(strData);
        
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
            
        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonParser.Accept(jsonWriter);
            
        strResult = jsonBuffer.GetString();
    }
    else
    {
        strResult = "";
    }
        
        
    saveInfiniteArtiData();
    
    return strResult;
}

std::string InfiniteArtifactManager::getInfiniteArtiCalculation(std::string value, INFINITE_ARTIFACT_TYPE type)
{
    auto vecArti = getInfiniteArtiInfoFromType(type);
    
    for(auto info : vecArti)
    {
        double addPer = info->getEfficiency();
        if(addPer>0){
            auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(addPer));
            tt.pop_back();
            tt.pop_back();
            value = MafUtils::bigAddNum(value, tt);
        }
    }
    
    return value;
}

//std::string InfiniteArtifactManager::getInfiniteArtiUpgradeKey(int grade, int level, INFINITE_ARTIFACT_TYPE type)
//{
//    std::string key = "0";
//    switch (type)
//    {
//        case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS:
//            key = MafUtils::doubleToString(1250000000000000000000000000000000000000000000000000000000000000000000000000000000000.0);
//            break;
//
//        default:
//            key = MafUtils::doubleToString(25000000000000000000000000000000000000000000000000000000000000000000000000000000000.0);
//            break;
//    }
//
//    //최초 요구량 15차 기본 요구량 1만배
//    key = MafUtils::bigMulNum(key, "50000");
//
//    //이후 단계별 기본 요구량 계산 단계당 10만배
//    for(int i = 1; i < grade; ++i)
//    {
//        key = MafUtils::bigMulNum(key, "100000");
//    }
//
//    key = MafUtils::bigMulNum(key, MafUtils::doubleToString((double)std::pow(level, 1.5f)));
//
//    return key;
//}

double InfiniteArtifactManager::getInfiniteArtiUpgradeKey(int grade, double increaseA, int increaseB, int level, INFINITE_ARTIFACT_TYPE type, bool isDouble)
{
    double key = 0.0f;
    if ( grade > 7 )
    {
        switch (type)
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS:
                key = 50.0;
                increaseB += 2;
                break;
                
            default:
                key = 25.0;
                break;
        }
        
        key *= 5;
        
        key *= std::pow(10, increaseB);
        
        key *= std::pow(level, increaseA);
    }
    else
    {
        switch (type)
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS:
                key = 1250000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
                break;
                
            default:
                key = 25000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
                break;
        }
        
        //최초 요구량 15차 기본 요구량 1만배
        key *= 50000;
        
        //이후 단계별 기본 요구량 계산 단계당 10만배
        for(int i = 1; i < grade; ++i)
        {
            key *= 100000;
        }
        
        key *= std::pow(level, 1.5f);
    }
    
    
    return key;
}


int InfiniteArtifactManager::getCurGrade()
{
    int result = _curGrade;
    
    if(_curGrade > _maxGrade)
        result = _maxGrade;
    
    return result;
}

int InfiniteArtifactManager::getMaxGrade()
{
    return _maxGrade;
}

bool InfiniteArtifactManager::isOpenCondition()
{
    if ( _openCondition == true )
    {
        return _openCondition;
    }
        
    
    auto user = UserInfoManager::getInstance();
    auto data = DataManager::getInstance();
    
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
    if(user->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_84_EVOL15) >= data->GetArtifactInfo(ARTI_84_EVOL15)->getLevelMax() &&
       user->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_85_EVOL15) >= data->GetArtifactInfo(ARTI_85_EVOL15)->getLevelMax() &&
       user->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_86_EVOL15) >= data->GetArtifactInfo(ARTI_86_EVOL15)->getLevelMax() &&
       user->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_87_EVOL15) >= data->GetArtifactInfo(ARTI_87_EVOL15)->getLevelMax() &&
       user->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_88_CROSS_EVOL15) >= data->GetArtifactInfo(ARTI_88_CROSS_EVOL15)->getLevelMax()
       )
    {
        _openCondition = true;
    }
    return _openCondition;
}

bool InfiniteArtifactManager::isActive()
{
    return _isActive;
}

bool InfiniteArtifactManager::isUpgradeCondition()
{
    bool result = false;
    
    int nMaxArti = 0;
    
    auto vecCurArti = getInfiniteArtiInfoFromGrade(_curGrade);
    
    for(auto info : vecCurArti)
    {
        if(info->getCurLv() >= info->getMaxLv())
            nMaxArti++;
    }
    if(nMaxArti >= INFINITE_ARTIFACT_KIND)
        result = true;
    
    
    return result;
}

bool InfiniteArtifactManager::isView()
{
    return _isInfiniteArtiView;
}
bool InfiniteArtifactManager::isUpgradeConditionToRiviveKeyArti()
{
    bool result = false;
    
    auto nCount = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY);
    
    auto nCurLevel = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT);
    if ( nCurLevel < nCount / 5 )
    {
        result = true;
    }
    
    return result;
}
void InfiniteArtifactManager::setView(bool active)
{
    _isInfiniteArtiView = active;
}
bool InfiniteArtifactManager::isFirst()
{
    return _isFirst;
}
void InfiniteArtifactManager::setFirstUpgrade(bool active)
{
    _isFirst = active;
}
#pragma mark - save load

void InfiniteArtifactManager::loadInfiniteArtiData()
{
    
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_INFINITE_ARTI, "");
    strData = MafAes256::Decrypt(strData);
    if(strData.compare("null") == 0)
    {
        strData = "";
    }

    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*) strData.c_str());

    if(jsonParser.HasParseError() == false)
    {
        _curGrade = jsonParser["cur_grade"].GetInt();
        _curAtkLv = jsonParser["atk_lv"].GetInt();
        _curCriLv = jsonParser["cri_lv"].GetInt();
        _curQuestLv = jsonParser["quest_lv"].GetInt();
        _curKillLv = jsonParser["kill_lv"].GetInt();
        _curCrossLv = jsonParser["cross_lv"].GetInt();
        _isActive = jsonParser["isActive"].GetBool();
        if(_isActive)
            _isFirst = false;
        
    }

}

void InfiniteArtifactManager::saveInfiniteArtiData()
{
    
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_INFINITE_ARTI, "");
    strData = MafAes256::Decrypt(strData);
    if(strData.compare("null") == 0)
    {
        strData = "";
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*) strData.c_str());
    
    if(jsonParser.HasParseError())
    {
        jsonParser.SetObject();
        //현재 단계
        rapidjson::Value vCurGrade;
        vCurGrade.SetInt(_curGrade);
        jsonParser.AddMember("cur_grade", vCurGrade, jsonParser.GetAllocator());
        
        //무한 보물 활성화 여부
        rapidjson::Value vActive;
        vActive.SetBool(_isActive);
        jsonParser.AddMember("isActive", vActive, jsonParser.GetAllocator());
        
        //공격력 보물
        rapidjson::Value vAtk;
        //크리티컬 보물
        rapidjson::Value vCri;
        //퀘스트 보물
        rapidjson::Value vQuest;
        //킬 보물
        rapidjson::Value vKill;
        //십자가 보물
        rapidjson::Value vCross;
        if(_curGrade > _maxGrade)
        {
            vAtk.SetInt(_curAtkLv);
            vCri.SetInt(_curCriLv);
            vQuest.SetInt(_curQuestLv);
            vKill.SetInt(_curKillLv);
            vCross.SetInt(_curCrossLv);
        }
        else
        {
            //공격력 보물
            auto info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
            
            if(info != nullptr) vAtk.SetInt(info->getCurLv());
            else vAtk.SetInt(0);
            
            //크리티컬 보물
            info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
                   
            if(info != nullptr) vCri.SetInt(info->getCurLv());
            else vCri.SetInt(0);
            
            //퀘스트 보물
            info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
            
            if(info != nullptr) vQuest.SetInt(info->getCurLv());
            else vQuest.SetInt(0);
            
            //킬 보물
            info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
            
            if(info != nullptr) vKill.SetInt(info->getCurLv());
            else vKill.SetInt(0);
            
            //십자가 보물
            info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
            
            if(info != nullptr) vCross.SetInt(info->getCurLv());
            else vCross.SetInt(0);
            
        }

        jsonParser.AddMember("atk_lv", vAtk, jsonParser.GetAllocator());
        jsonParser.AddMember("cri_lv", vCri, jsonParser.GetAllocator());
        jsonParser.AddMember("quest_lv", vQuest, jsonParser.GetAllocator());
        jsonParser.AddMember("kill_lv", vKill, jsonParser.GetAllocator());
        jsonParser.AddMember("cross_lv", vCross, jsonParser.GetAllocator());
        
    }
    else
    {
        if(jsonParser.HasMember("cur_grade"))//현재 단계
        {
            jsonParser["cur_grade"].SetInt(_curGrade);
        }
        else
        {
            rapidjson::Value vCurGrade;
            vCurGrade.SetInt(_curGrade);
            jsonParser.AddMember("cur_grade", vCurGrade, jsonParser.GetAllocator());
        }
        
        if(jsonParser.HasMember("isActive"))//무한 보물 활성화 여부
        {
            jsonParser["isActive"].SetBool(_isActive);
        }
        else
        {
            rapidjson::Value vActive;
            vActive.SetBool(_isActive);
            jsonParser.AddMember("isActive", vActive, jsonParser.GetAllocator());
        }
        
        
        
        int atkLv = 0;
        auto info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
        if(info != nullptr) atkLv = info->getCurLv();
        
        int criLv = 0;
        info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
        if(info != nullptr) criLv = info->getCurLv();
        
        int questLv = 0;
        info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
        if(info != nullptr) questLv = info->getCurLv();
        
        int killLv = 0;
        info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
        if(info != nullptr) killLv = info->getCurLv();
        
        int crossLv = 0;
        info = getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
        if(info != nullptr) crossLv = info->getCurLv();
        
        if(_curGrade > _maxGrade)
        {
            atkLv = _curAtkLv;
            criLv = _curCriLv;
            questLv = _curQuestLv;
            killLv = _curKillLv;
            crossLv = _curCrossLv;
        }
        
        if(jsonParser.HasMember("atk_lv"))//공격력 보물
        {
            jsonParser["atk_lv"].SetInt(atkLv);
        }
        else
        {
            rapidjson::Value value;
            value.SetInt(atkLv);
            jsonParser.AddMember("atk_lv", value, jsonParser.GetAllocator());
        }
        
        if(jsonParser.HasMember("cri_lv"))// 크리티컬 보물
        {
            jsonParser["cri_lv"].SetInt(criLv);
        }
        else
        {
            rapidjson::Value value;
            value.SetInt(criLv);
            jsonParser.AddMember("cri_lv", value, jsonParser.GetAllocator());
        }
        
        if(jsonParser.HasMember("quest_lv"))// 퀘스트 보물
        {
            jsonParser["quest_lv"].SetInt(questLv);
        }
        else
        {
            rapidjson::Value value;
            value.SetInt(questLv);
            jsonParser.AddMember("quest_lv", value, jsonParser.GetAllocator());
        }
        
        if(jsonParser.HasMember("kill_lv"))// 킬 보물
        {
            jsonParser["kill_lv"].SetInt(killLv);
        }
        else
        {
            rapidjson::Value value;
            value.SetInt(killLv);
            jsonParser.AddMember("kill_lv", value, jsonParser.GetAllocator());
        }
        
        if(jsonParser.HasMember("cross_lv"))// 십자가 보물
        {
            jsonParser["cross_lv"].SetInt(crossLv);
        }
        else
        {
            rapidjson::Value value;
            value.SetInt(crossLv);
            jsonParser.AddMember("cross_lv", value, jsonParser.GetAllocator());
        }
        
    }
    
    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
    jsonParser.Accept(jsonWriter);
    
    strData = jsonBuffer.GetString();
    

    strData = MafAes256::Encrypt(strData);
    UserDefault::getInstance()->setStringForKey(KEY_INFINITE_ARTI, strData);
}

#pragma mark - network

void InfiniteArtifactManager::requestInfiniteArtiBuy(const std::function<void(bool)>& pCallback)
{
    PopupLoading::show();
       
    std::string url = "/v2/artifact/transcendence/buy";
       
       _callbackArtiBuy = pCallback;
       auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
       req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
       req->setData("_grade", _curGrade + 1);
       
       MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
           std::string dd = data;
           responseInfiniteArtiBuy(response, dd);
       };
       req->send(cb);
}

void InfiniteArtifactManager::responseInfiniteArtiBuy(cocos2d::network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        _callbackArtiBuy = nullptr;
        return;
    }
    
    int bResult = jsonParser["_result"].GetInt();
    
    if (bResult == 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_17"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(_callbackArtiBuy != nullptr)
            _callbackArtiBuy(false);
        
        _callbackArtiBuy = nullptr;
        
        return;
    }
    else
    {
        _isActive = true;
        _isInfiniteArtiView = true;
        if(bResult == 4)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_restoration"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        
        auto serverGradeList = jsonParser["_artifact_info_list"].GetArray();
        for(int i = 0; i < serverGradeList.Size(); ++i)
        {
            int nGrade = serverGradeList[i]["_grade"].GetInt();
            if(_curGrade < nGrade)
                _curGrade = nGrade;
        }
        
        if(_curGrade == 1)
        {
            auto popup = PopupFirstUpgradeInfiniteArtifact::create();
            popup->show();
        }
        addNewGrade(_curGrade);
    }

    int nCoin = jsonParser["_total_count"].GetInt();
    
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
    
    renewalArtifact();
    
    if(_callbackArtiBuy != nullptr)
        _callbackArtiBuy(true);
    
    _callbackArtiBuy = nullptr;
    saveInfiniteArtiData();
}

#pragma mark - util
void InfiniteArtifactManager::renewalArtifact()
{
    for(auto info : _vecInfiniteArtifactInfo)
    {
        if(info->getGrade() != _curGrade && info->isCurrent())
        {
            info->setIsCurrent(false);
        }
    }
}

void InfiniteArtifactManager::addNewGrade(int grade)
{
    for(int i = 1; i <= grade; ++i)
    {
        auto basicInfo = getInfiniteBasicInfoFromGrade(i);
        if(basicInfo == nullptr)
            continue;
        auto vecInfo = getInfiniteArtiInfoFromGrade(basicInfo->getGrade());
        
        if(vecInfo.size() != 0)
            continue;
        
        for(int i = 1; i <= INFINITE_ARTIFACT_KIND; ++i)
        {
            addNewInfiniteArtifact((INFINITE_ARTIFACT_TYPE)i, basicInfo);
        }
    }
}

void InfiniteArtifactManager::addNewInfiniteArtifact(INFINITE_ARTIFACT_TYPE type, InfiniteBasicInfo* basicInfo)
{
    auto info = new InfiniteArtifactInfo(type, basicInfo);
    if(_curGrade == info->getGrade())
        info->setIsCurrent(true);
    else if(_curGrade > info->getGrade())
        info->setCurLv(info->getMaxLv());
    
    _vecInfiniteArtifactInfo.push_back(info);
}

void InfiniteArtifactManager::setInfiniteArtifactLevel(InfiniteArtifactInfo* info, int lv)
{
    if(info == nullptr)
        return;
    
    info->setCurLv(lv);
    
    if(_curGrade <= _maxGrade)
    {
        switch (info->getType())
        {
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK: _curAtkLv = info->getCurLv(); break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI: _curCriLv = info->getCurLv(); break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST: _curQuestLv = info->getCurLv(); break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL: _curKillLv = info->getCurLv(); break;
            case INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS: _curCrossLv = info->getCurLv(); break;
                
            default:
                break;
        }
    }
    
    saveInfiniteArtiData();
    
    TimingPackageManager::getInstance()->invokeEventTimingPackage();
}
