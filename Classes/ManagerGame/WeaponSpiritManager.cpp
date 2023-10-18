//
//  WeaponSpiritManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/01.
//

#include "WeaponSpiritManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

WeaponSpiritManager::WeaponSpiritManager(void):
_nSpiritStonCount(0)
,_isServerDispatch(false)
,_nTotalMileage(0)
,_nCuttentMileageRewardIdx(0)
,_nSummonTicket_1(0)
,_nSummonTicket_11(0)
,_nSummonTicket_33(0)
,_nSpiritContractLv(0)
,_nSpiritBuffCooltime(0)
,_nSpiritBuffDurationTime(0)
,_eSpiritBuffState(WeaponSpiritManager::E_SPIRIT_BUFF_STATE::NONE)
,_infoWearSpirit(nullptr)
,_callbackInfo(nullptr)
,_callbackRankUp(nullptr)
,_callbackLevelUp(nullptr)
,_callbackSmelting(nullptr)
,_callbackWear(nullptr)
,_callbackContractLv(nullptr)
,_callbackSummon(nullptr)
,_callbackMileageReward(nullptr)
{
    _listInfoWeaponSpirit.clear();
    _mapSmeltingCost.clear();
    _listInfoMileageLv.clear();
    _listInfoMileageReward.clear();
    _listIsSpiritFirstSummon.clear();
    _listInfoWeaponSpiritBuff.clear();
    _listInfoWeaponSpiritGradeSynergy.clear();
}

WeaponSpiritManager::~WeaponSpiritManager(void)
{
    _listInfoWeaponSpirit.clear();
    _mapSmeltingCost.clear();
    _listInfoMileageLv.clear();
    _listInfoMileageReward.clear();
    _listIsSpiritFirstSummon.clear();
    _listInfoWeaponSpiritBuff.clear();
    _listInfoWeaponSpiritGradeSynergy.clear();
}

bool WeaponSpiritManager::init()
{
    setLoadSpirit();
    setLoadSmeltingCost();
    setLoadMileageLv();
    setLoadMileageReward();
    setLoadWeaponSpiritBuff();
    setLoadWeaponSpiritSynergy();
    return true;
}

#pragma mark - Load Data
void WeaponSpiritManager::setLoadSpiritFirstSummonData(int size)
{
    _listIsSpiritFirstSummon.clear();
    for(int i = 0; i < size; ++i)
    {
        _listIsSpiritFirstSummon.push_back(false);
    }
    
    std::string str;
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_SPIRIT_FIRST_SUMMON);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=0; i < _listIsSpiritFirstSummon.size(); i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                _listIsSpiritFirstSummon[i] = ret;
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                _listIsSpiritFirstSummon[i] = ret;
                break;
            }
        }
    }
    setSaveSpiritFirstSummonData();
}
void WeaponSpiritManager::setLoadSpirit()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::WEAPON_SPIRIT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    _listInfoWeaponSpirit.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        int nGrade = jsonValue["grade"].GetInt();
        int nIdxNextComb = jsonValue["next_comb_idx"].GetInt();
        double fDmgValue = jsonValue["dmg_value"].GetDouble();
        double fGoldValue = jsonValue["gold_value"].GetDouble();
        double fKeyValue = jsonValue["key_value"].GetDouble();
        int nLvUpCost = jsonValue["lv_up_cost"].GetInt();
        int nSmeltingCost = jsonValue["smelting_cost_base"].GetInt();
        
        
        //
        auto obj = InfoWeaponSpirit::create();
        obj->setIdx(nIdx);
        obj->setGrade(nGrade);
        obj->setIdxNextComb(nIdxNextComb);
        obj->setEffectDmg(fDmgValue);
        obj->setEffectGold(fGoldValue);
        obj->setEffectKey(fKeyValue);
        obj->setLvUpCost(nLvUpCost);
        obj->setSmeltingCost(nSmeltingCost);
        
        _listInfoWeaponSpirit.pushBack(obj);
    }
    setLoadSpiritFirstSummonData(_listInfoWeaponSpirit.size());
}

void WeaponSpiritManager::setLoadSmeltingCost()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::WEAPON_SPIRIT_SMELTING_COST);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _mapSmeltingCost.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nIdx = jsonValue["lv"].GetInt();
        int nCost = jsonValue["spirit_cost"].GetInt();
        
        _mapSmeltingCost.insert(std::make_pair(nIdx, nCost));
    }
}

void WeaponSpiritManager::setLoadMileageLv()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_MILEAGE_LV);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoMileageLv.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nLv = jsonValue["lv"].GetInt();
        int nValue = jsonValue["value"].GetInt();
        
        InfoRaidMileageLv* obj = InfoRaidMileageLv::create();
        obj->setLv(nLv);
        obj->setValue(nValue);
        
        _listInfoMileageLv.pushBack(obj);
    }
}

void WeaponSpiritManager::setLoadMileageReward()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_MILEAGE_REWARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoMileageReward.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nLv = jsonValue["idx"].GetInt();
        int nValue = jsonValue["value"].GetInt();
        std::string strReward = jsonValue["reward"].GetString();
        
        auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        
        InfoRaidMileageReward* obj = InfoRaidMileageReward::create();
        obj->setIdx(nLv);
        obj->setValue(nValue);
        obj->setListReward(listReward);
        
        _listInfoMileageReward.pushBack(obj);
    }
}

void WeaponSpiritManager::setLoadWeaponSpiritBuff()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::WEAPON_SPIRIT_BUFF);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoWeaponSpiritBuff.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        int nIdx = jsonValue["idx"].GetInt();
        E_SPIRIT_BUFF_TYPE eType = (E_SPIRIT_BUFF_TYPE)jsonValue["type"].GetInt();
        int nIncreases = jsonValue["increases"].GetInt();
        int nCoolTime = jsonValue["cool_time"].GetInt();
        int nDurationTime = jsonValue["duration_time"].GetInt();
        std::string strColor = jsonValue["color"].GetString();
        Color4B color = Color4B::BLACK;
        
        rapidjson::Document jsonParserColor;
        jsonParserColor.Parse<0>((char*)strColor.c_str());
        if( jsonParserColor.HasParseError() == false )
        {
            int r = jsonParserColor["r"].GetInt();
            int g = jsonParserColor["g"].GetInt();
            int b = jsonParserColor["b"].GetInt();
            color = Color4B(r,g,b,255);
        }
        
        InfoWeaponSpiritBuff* obj = InfoWeaponSpiritBuff::create();
        obj->setIdx(nIdx);
        obj->setType(eType);
        obj->setIncreases(nIncreases);
        obj->setCoolTime(nCoolTime);
        obj->setDurationTime(nDurationTime);
        obj->setEffectColor(color);
        
        _listInfoWeaponSpiritBuff.pushBack(obj);
    }
}

void WeaponSpiritManager::setLoadWeaponSpiritSynergy()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::WEAPON_SPIRIT_SYNERGY);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoWeaponSpiritGradeSynergy.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        int nGrade = jsonValue["grade"].GetInt();
        float nIncreasesLv = jsonValue["increases_lv"].GetDouble();
        float nIncreasesSmelting = jsonValue["increases_smelting"].GetDouble();
        
        InfoWeaponSpiritGradeSynergy* obj = InfoWeaponSpiritGradeSynergy::create();
        obj->setGrade(nGrade);
        obj->setIncreasesLv(nIncreasesLv);
        obj->setIncreasesSmelting(nIncreasesSmelting);
        _listInfoWeaponSpiritGradeSynergy.pushBack(obj);
    }
}
#pragma mark - Save Data
void WeaponSpiritManager::setSaveSpiritFirstSummonData()
{
    std::stringstream str;
    
    str.str("");
    for(int i=0; i<_listIsSpiritFirstSummon.size();i++)
    {
        str << _listIsSpiritFirstSummon[i] <<",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_SPIRIT_FIRST_SUMMON, MafAes256::Encrypt(str.str()));
}

#pragma mark -get
Vector<InfoWeaponSpirit*> WeaponSpiritManager::getListWeaponSpirit()
{
    return _listInfoWeaponSpirit;
}

Vector<InfoWeaponSpirit*> WeaponSpiritManager::getListWeaponSpirit(int nGrade)
{
    Vector<InfoWeaponSpirit*> result;
    result.clear();
    
    for(auto obj : _listInfoWeaponSpirit)
    {
        if(obj->getGrade() == nGrade)
            result.pushBack(obj);
    }
    
    return result;
}

InfoWeaponSpirit* WeaponSpiritManager::getInfoWeaponSpirit(int idx)
{
    InfoWeaponSpirit* result = nullptr;
    
    for(auto obj : _listInfoWeaponSpirit)
    {
        if(obj->getIdx() == idx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoWeaponSpirit* WeaponSpiritManager::getInfoWearSpirit()
{
    return _infoWearSpirit;
}

int WeaponSpiritManager::getSpiritStonCount()
{
    return _nSpiritStonCount;
}
int WeaponSpiritManager::getTotalMileage()
{
    return _nTotalMileage;
}
int WeaponSpiritManager::getSummonTicket_1()
{
    return _nSummonTicket_1;
}
int WeaponSpiritManager::getSummonTicket_11()
{
    return _nSummonTicket_11;
}
int WeaponSpiritManager::getSummonTicket_33()
{
    return _nSummonTicket_33;
}
int WeaponSpiritManager::getCurrentMileageLv()
{
    int result = 0;
    
    int tmpPoint = _nTotalMileage;
    
    for(auto obj : _listInfoMileageLv)
    {
        if(obj->getValue() > _nTotalMileage)
        {
            break;
        }
        result++;
    }
    
    return result;
}

int WeaponSpiritManager::getCurrentMileageLvPoint()
{
    int result = _nTotalMileage;
    int currentLv = getCurrentMileageLv();
    
    for(auto obj : _listInfoMileageLv)
    {
        if(obj->getLv() == currentLv)
        {
            result -= obj->getValue();
            break;
        }
    }
    
    return result;
}

InfoRaidMileageLv* WeaponSpiritManager::getInfoMileageLv(int value)
{
    InfoRaidMileageLv* result = nullptr;
    
    for(auto obj : _listInfoMileageLv)
    {
        if(obj->getLv() == value)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

Vector<InfoRaidMileageLv*> WeaponSpiritManager::getListMileageLv()
{
    return _listInfoMileageLv;
}

int WeaponSpiritManager::getCurrentMileageRewardIdx()
{
    return _nCuttentMileageRewardIdx;
}

void WeaponSpiritManager::setCurrentMileageRewardIdx(int value)
{
    _nCuttentMileageRewardIdx = value;
}

void WeaponSpiritManager::setContractLv(int value)
{
    _nSpiritContractLv = value;
}

int WeaponSpiritManager::getContractLv()
{
    return _nSpiritContractLv;
}

int WeaponSpiritManager::getCurrentMileageRewardPoint()
{
    int result = _nTotalMileage;
    
    for(auto obj : _listInfoMileageReward)
    {
        if(obj->getValue() > result || obj->getIdx() == _nCuttentMileageRewardIdx)
        {
            break;
        }
        result -= obj->getValue();
    }
    
    return result;
}

InfoRaidMileageReward* WeaponSpiritManager::getInfoMileageReward(int value)
{
    InfoRaidMileageReward* result = nullptr;
    
    for(auto obj : _listInfoMileageReward)
    {
        if(obj->getIdx() == value)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

Vector<InfoRaidMileageReward*> WeaponSpiritManager::getListMileageReward()
{
    return _listInfoMileageReward;
}

InfoWeaponSpiritBuff* WeaponSpiritManager::getInfoWeaponSpiritBuff()
{
    InfoWeaponSpiritBuff* result = nullptr;
    
    if(_infoWearSpirit == nullptr)
        return result;
    
    for(auto obj : _listInfoWeaponSpiritBuff)
    {
        if(_infoWearSpirit->getIdx() == obj->getIdx())
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoWeaponSpiritBuff* WeaponSpiritManager::getInfoWeaponSpiritBuff(int nIdx)
{
    InfoWeaponSpiritBuff* result = nullptr;
    
    for(auto obj : _listInfoWeaponSpiritBuff)
    {
        if(nIdx == obj->getIdx())
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoWeaponSpiritGradeSynergy* WeaponSpiritManager::getInfoWeaponSpiritGradeSynergy(int nGrade)
{
    InfoWeaponSpiritGradeSynergy* result = nullptr;
    
    for(auto obj : _listInfoWeaponSpiritGradeSynergy)
    {
        if(nGrade == obj->getGrade())
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

void WeaponSpiritManager::setSpiritStonCount(int value)
{
    _nSpiritStonCount = value;
}
void WeaponSpiritManager::setTotalMileage(int value)
{
    _nTotalMileage = value;
}
void WeaponSpiritManager::setSummonTicket_1(int value)
{
    _nSummonTicket_1 = value;
}
void WeaponSpiritManager::setSummonTicket_11(int value)
{
    _nSummonTicket_11 = value;
}
void WeaponSpiritManager::setSummonTicket_33(int value)
{
    _nSummonTicket_33 = value;
}
int WeaponSpiritManager::getAdsSummonCooldown()
{
    return _nAdsSummonCooltime;
}
void WeaponSpiritManager::setAdsSummonCooldown(int value)
{
    _nAdsSummonCooltime = value;
}

void WeaponSpiritManager::setSpiritBuff()
{
    auto objSettingMaxLv = NewRaidManager::getInstance()->getRaidSetting("spirit_max_lv");
    int nMaxLv = 99;
    
    if(objSettingMaxLv != nullptr)
        nMaxLv = objSettingMaxLv->getValueToInt();
    
    if(_infoWearSpirit->getGrade() >= 6 &&
       _infoWearSpirit->getCurrentLv() >= nMaxLv &&
       _infoWearSpirit->getCurrentSmelting() >= 1)
    {
        if(_eSpiritBuffState == WeaponSpiritManager::E_SPIRIT_BUFF_STATE::NONE)
        {
            _eSpiritBuffState = WeaponSpiritManager::E_SPIRIT_BUFF_STATE::COOL_TIME;
        }
    }
    else
    {
        
        _eSpiritBuffState = WeaponSpiritManager::E_SPIRIT_BUFF_STATE::NONE;
        _nSpiritBuffCooltime = 0;
        _nSpiritBuffDurationTime = 0;
    }
}
void WeaponSpiritManager::scheduleSpiritBuff()
{
    switch(_eSpiritBuffState)
    {
        case WeaponSpiritManager::E_SPIRIT_BUFF_STATE::NONE :
        {
            return;
        }break;
        case WeaponSpiritManager::E_SPIRIT_BUFF_STATE::COOL_TIME :
        {
            auto objBuff = getInfoWeaponSpiritBuff();
            if(objBuff == nullptr)
            {
                _eSpiritBuffState = WeaponSpiritManager::E_SPIRIT_BUFF_STATE::NONE;
                return;
            }
            _nSpiritBuffCooltime++;
            
            if(_nSpiritBuffCooltime >= objBuff->getCoolTime())
            {
                _eSpiritBuffState = E_SPIRIT_BUFF_STATE::DURATION_TIME;
                _nSpiritBuffDurationTime = 0;
                return;
            }
        }break;
        case WeaponSpiritManager::E_SPIRIT_BUFF_STATE::DURATION_TIME :
        {
            auto objBuff = getInfoWeaponSpiritBuff();
            if(objBuff == nullptr)
            {
                _eSpiritBuffState = WeaponSpiritManager::E_SPIRIT_BUFF_STATE::NONE;
                return;
            }
            
            _nSpiritBuffDurationTime++;
            if(_nSpiritBuffDurationTime >= objBuff->getDurationTime())
            {
                _eSpiritBuffState = E_SPIRIT_BUFF_STATE::COOL_TIME;
                _nSpiritBuffCooltime = _nSpiritBuffDurationTime;
                return;
            }
        }break;
    }
}
WeaponSpiritManager::E_SPIRIT_BUFF_STATE WeaponSpiritManager::getBuffState()
{
    return _eSpiritBuffState;
}
int WeaponSpiritManager::getAdsSummonCount()
{
    return _nAdsSummonCount;
}
void WeaponSpiritManager::setAdsSummonCount(int value)
{
    _nAdsSummonCount = value;
}

bool WeaponSpiritManager::isContractPossible(int value)
{
    bool result = false;
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_floor_" + std::to_string(value));
    
    auto objPriceSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_cost");
    
    int price = objPriceSetting->getValueToInt();
    for(int i = 0; i < value - 1; ++i)
        price *= 2;
    
    if(objSetting == nullptr)
        return false;
    
    if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= objSetting->getValueToInt()
       && price <= _nSpiritStonCount)
        result = true;
    
    return result;
}
bool WeaponSpiritManager::isReddotCondition()
{
    bool result = false;
    
    if (!NewRaidManager::getInstance()->isRaidEnterCondition())
    {
        return result;
    }
    
    for(int i = 0; i < _listIsSpiritFirstSummon.size(); ++i)
    {
        auto obj = getInfoWeaponSpirit(i + 1);
        if(obj->isHas() && !_listIsSpiritFirstSummon[i])
        {
            result = true;
            return result;
        }
    }
    
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_max_lv");
    for(int i = 0; i < objSetting->getValueToInt(); ++i)
    {
        int lv = i+1;
        if(isContractPossible(lv) && _nSpiritContractLv < lv)
        {
            result = true;
            return result;
        }
    }
    
    auto objRankUpSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_rank_up_cost");
    for(auto obj : _listInfoWeaponSpirit)
    {
        if(obj->getIdx() != _listInfoWeaponSpirit.size() && obj->getCurrentCount() / objRankUpSetting->getValueToInt() >= 1)
        {
            result = true;
            return result;
        }
    }
    
    return result;
}

bool WeaponSpiritManager::isFirstSummon(int value)
{
    bool result = false;
    
    if(_listIsSpiritFirstSummon.size() > value)
        result = _listIsSpiritFirstSummon[value];
    
    return result;
}

void WeaponSpiritManager::setFirstSummon(int value)
{
    if(_listIsSpiritFirstSummon.size() > value && getInfoWeaponSpirit(value + 1)->isHas())
        _listIsSpiritFirstSummon[value] = true;
    
    setSaveSpiritFirstSummonData();
}

bool WeaponSpiritManager::isServerDispatch()
{
    return _isServerDispatch;
}
int WeaponSpiritManager::getSmeltingSpiritCost(int nLv)
{
    int result = 0;
    auto iter = _mapSmeltingCost.find(nLv);
    
    if(iter != _mapSmeltingCost.end())
        result = iter->second;
    
    return result;
}

std::string WeaponSpiritManager::getSpiritCalculation(std::string value, WeaponSpiritManager::E_EFFECT_TYPE type, bool isOrginal)
{
    double addPer = 0;
    
    for(auto info : _listInfoWeaponSpirit)
    {
        if(!info->isHas())
            continue;
        
        double spiritPer = 0;
        switch (type) {
            case WeaponSpiritManager::E_EFFECT_TYPE::DMG: spiritPer = info->getEffectDmg(); break;
            case WeaponSpiritManager::E_EFFECT_TYPE::GOLD: spiritPer = info->getEffectGold(); break;
            case WeaponSpiritManager::E_EFFECT_TYPE::KEY: spiritPer = info->getEffectKey(); break;
        }
        addPer += spiritPer;
    }
    
    if ( type == WeaponSpiritManager::E_EFFECT_TYPE::DMG && !isOrginal )
    {
        //
        double synergy = addPer * getSpiritSynergyAll() * 0.01;
        addPer += synergy;
        
        //
        addPer = addPer * std::pow(10, getSpiritMaximumAll());;
    }
    
    if ( addPer > 0 )
    {
        auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(addPer));
        if(tt.length() > 0)
            tt.pop_back();
        
        if(tt.length() > 0)
            tt.pop_back();
        
        if(tt.length() > 0)
            value = MafUtils::bigAddNum(value, tt);
    }
    
    return value;
}
double WeaponSpiritManager::getSpiritSynergyAll()
{
    double result = 0;
    
    for(auto obj : _listInfoWeaponSpiritGradeSynergy)
    {
        result += getSpiritSynergy(obj->getGrade());
    }
    
    //모험 유물 적용
    double advtRelicBuff = 0.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicEffectBuffCalculator(E_ADVENTURE_PROPERTY_CALCULATE_TYPE::SPIRIT_COLLECT_ATK);
    double relicBonus = result * advtRelicBuff * 0.01;
    
    result += relicBonus;
    
    return result;
}

double WeaponSpiritManager::getSpiritSynergy(int nGrade)
{
    double result = 0;
    
    auto objSynergy = getInfoWeaponSpiritGradeSynergy(nGrade);
    
    if(objSynergy == nullptr)
        return result;
    
    double nIncreasesLv = 0;
    double nIncreasesSmelting = 0;
    
    auto list = getListWeaponSpirit(nGrade);
    
    for(auto obj : list)
    {
        if(!obj->isHas())
            continue;
        
        nIncreasesLv += obj->getCurrentLv();
        nIncreasesSmelting += obj->getCurrentSmelting();
    }

    result = (((nIncreasesLv * objSynergy->getIncreasesLv()) + 100) * ((nIncreasesSmelting * objSynergy->getIncreasesSmelting()) + 100) / 100 ) - 100;
    
    return result;
}

int WeaponSpiritManager::getSpiritMaximumAll()
{
    auto objSettingSmelting = NewRaidManager::getInstance()->getRaidSetting("spirit_smelting_max_lv");
    auto objSettingLevel = NewRaidManager::getInstance()->getRaidSetting("spirit_max_lv");
    
    
    int smeltingMax = objSettingSmelting->getValueToInt();
    int levelMax = objSettingLevel->getValueToInt();
    
    int count = 0;
    for ( auto& obj : _listInfoWeaponSpirit )
    {
        if ( smeltingMax <= obj->getCurrentSmelting() && levelMax <= obj->getCurrentLv() )
        {
            count++;
        }
    }
    
    return count;
}

bool WeaponSpiritManager::getSpiritMaximum(int idx)
{
    bool bResult = false;
    
    auto objSettingSmelting = NewRaidManager::getInstance()->getRaidSetting("spirit_smelting_max_lv");
    auto objSettingLevel = NewRaidManager::getInstance()->getRaidSetting("spirit_max_lv");
    
    int smeltingMax = objSettingSmelting->getValueToInt();
    int levelMax = objSettingLevel->getValueToInt();
    
    auto objInfoWeaponSpirit = getInfoWeaponSpirit(idx);
    if ( objInfoWeaponSpirit != nullptr )
    {
        if ( smeltingMax <= objInfoWeaponSpirit->getCurrentSmelting() && levelMax <= objInfoWeaponSpirit->getCurrentLv() )
        {
            bResult = true;
        }
    }
    
    return bResult;
}

int WeaponSpiritManager::getSpiritMaxGrade()
{
    int result = 0;
    
    for(auto obj : _listInfoWeaponSpirit)
    {
        if(obj->getGrade() > result)
            result = obj->getGrade();
    }
    
    return result;
}

#pragma mark - network
void WeaponSpiritManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    //
    std::string url = "/new/raid/weapon/info/%d";
    _isServerDispatch = true;
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    _isServerDispatch = false;
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
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, 0);
        }
        return;
    }
    
    _nSpiritStonCount = jsonParser["_spirit_stone"].GetInt();
    _nSpiritContractLv = jsonParser["_sprit_contract"].GetInt();
    
    _nTotalMileage   = jsonParser["_mileage"].GetInt();
    _nSummonTicket_1   = jsonParser["_draw_ticket_1"].GetInt();
    _nSummonTicket_11   = jsonParser["_draw_ticket_11"].GetInt();
    _nSummonTicket_33   = jsonParser["_draw_ticket_33"].GetInt();
    
    _nAdsSummonCount   = jsonParser["_spirit_draw_ads_count"].GetInt();
    _nAdsSummonCooltime   = jsonParser["_spirit_draw_ads_cooltime"].GetInt();
    
    _nCuttentMileageRewardIdx = jsonParser["_mileage_reward"].Size() + 1;
    
    int nDragonBall = 0;
    if(!jsonParser["_dragon_ball"].IsNull())
        nDragonBall = jsonParser["_dragon_ball"].GetInt();
    NewRaidManager::getInstance()->setDragonBall(nDragonBall);
    
    int nAdsDragonBallCount = 0;
    if(!jsonParser["_dragon_ball_ads_count"].IsNull())
        nAdsDragonBallCount = jsonParser["_dragon_ball_ads_count"].GetInt();
    NewRaidManager::getInstance()->setAdsDragonBallCount(nAdsDragonBallCount);
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    
    
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, 1);
    }
    
    _callbackInfo = nullptr;
}

void WeaponSpiritManager::requestSpiritCompose(const std::function<void(bool, int)>& pCallback, int nIdx, int nCount)
{
    //
    _callbackRankUp = pCallback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/ComposeWeaponSpirit";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", nIdx);
    req->setData("_count", nCount);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSpiritCompose(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseSpiritCompose(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackRankUp != nullptr )
        {
            _callbackRankUp(false, 0);
            _callbackRankUp = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackRankUp != nullptr )
        {
            _callbackRankUp(false, nResult);
            _callbackRankUp = nullptr;
        }
        return;
    }
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    
    if ( _callbackRankUp != nullptr )
    {
        _callbackRankUp(true, nResult);
        _callbackRankUp = nullptr;
    }
    
}

void WeaponSpiritManager::requestSpiritLevelup(const std::function<void(bool, int)>& pCallback, int nIdx)
{
    //
    _callbackLevelUp = pCallback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/ReinforcementWeaponSpirit";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSpiritLevelup(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseSpiritLevelup(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackLevelUp != nullptr )
        {
            _callbackLevelUp(false, 0);
            _callbackLevelUp = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackLevelUp != nullptr )
        {
            _callbackLevelUp(false, nResult);
            _callbackLevelUp = nullptr;
        }
        return;
    }
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::SPIRIT_STON )
            {
                _nSpiritStonCount = std::atoi(obj->getCount().c_str());
            }
        }
        
    }
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackLevelUp != nullptr )
    {
        _callbackLevelUp(true, nResult);
        _callbackLevelUp = nullptr;
    }
    
}

void WeaponSpiritManager::requestSpiritSmelting(const std::function<void(bool, int)>& pCallback, int nIdx)
{
    //
    _callbackSmelting = pCallback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/SmeltingWeaponSpirit";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSpiritSmelting(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseSpiritSmelting(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackSmelting != nullptr )
        {
            _callbackSmelting(false, 0);
            _callbackSmelting = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackSmelting != nullptr )
        {
            _callbackSmelting(false, nResult);
            _callbackSmelting = nullptr;
        }
        return;
    }
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::SPIRIT_STON )
            {
                _nSpiritStonCount = std::atoi(obj->getCount().c_str());
            }
        }
        
    }
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackSmelting != nullptr )
    {
        _callbackSmelting(true, nResult);
        _callbackSmelting = nullptr;
    }
    
}

void WeaponSpiritManager::requestSpiritWear(const std::function<void(bool, int)>& pCallback, int nIdx)
{
    //
    _callbackWear = pCallback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/WearWeaponSpirit";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSpiritWear(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseSpiritWear(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackWear != nullptr )
        {
            _callbackWear(false, 0);
            _callbackWear = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackWear != nullptr )
        {
            _callbackWear(false, nResult);
            _callbackWear = nullptr;
        }
        return;
    }
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    
    if ( _callbackWear != nullptr )
    {
        _callbackWear(true, nResult);
        _callbackWear = nullptr;
    }
    
}

void WeaponSpiritManager::requestSpiritSummon(const std::function<void(bool, int, Vector<InfoItems*>)>& callback, int nCount, bool isAds)
{
    //
    _callbackSummon = callback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/SummonWeaponSpirit";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_count", nCount);
    req->setData("_isADS", (int)isAds);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSpiritSummon(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseSpiritSummon(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackSummon != nullptr )
        {
            _callbackSummon(false, 0, listReward);
            _callbackSummon = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackSummon != nullptr )
        {
            _callbackSummon(false, nResult, listReward);
            _callbackSummon = nullptr;
        }
        return;
    }
    
    _nTotalMileage = jsonParser["_total_mileage"].GetInt();
    _nSummonTicket_1 = jsonParser["_draw_ticket_1"].GetInt();
    _nSummonTicket_11 = jsonParser["_draw_ticket_11"].GetInt();
    _nSummonTicket_33   = jsonParser["_draw_ticket_33"].GetInt();
    _nAdsSummonCount   = jsonParser["_spirit_draw_ads_count"].GetInt();
    _nAdsSummonCooltime   = jsonParser["_spirit_draw_ads_cooltime"].GetInt();
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    ItemsManager::getInstance()->addItems(strReward, false);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::SPIRIT_STON )
            {
                _nSpiritStonCount = std::atoi(obj->getCount().c_str());
            }
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                int nCount = std::atoi(obj->getCount().c_str());
                NewRaidManager::getInstance()->setDragonBall(nCount);
            }
        }
    }
    
    int nDragonBall = jsonParser["_dragon_ball"].GetInt();
    NewRaidManager::getInstance()->setDragonBall(nDragonBall);
    
    if ( _callbackSummon != nullptr )
    {
        _callbackSummon(true, nResult, listReward);
        _callbackSummon = nullptr;
    }
    
}

void WeaponSpiritManager::requestMileageReward(const std::function<void(bool, int)>& callback)
{
    //
    _callbackMileageReward = callback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/WeaponSpiritMileageReward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", _nCuttentMileageRewardIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMileageReward(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseMileageReward(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackMileageReward != nullptr )
        {
            _callbackMileageReward(false, 0);
            _callbackMileageReward = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackMileageReward != nullptr )
        {
            _callbackMileageReward(false, nResult);
            _callbackMileageReward = nullptr;
        }
        return;
    }
    
    int nMileageRewardIdx = jsonParser["_mileage_reward"].Size() + 1;
    
    _nCuttentMileageRewardIdx = nMileageRewardIdx;
    
    resetSpiritInfo();
    for ( int i = 0; i < jsonParser["_weapon_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_weapon_list"][i];
        
        for (auto obj : _listInfoWeaponSpirit)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nSmelting = jsonValue["smelting"].GetInt();
                int nLevel = jsonValue["level"].GetInt();
                int nCurrentCount = jsonValue["count"].GetInt();
                bool isWear = jsonValue["wear"].GetInt();
                
                obj->setHas(true);
                obj->setWear(isWear);
                obj->setCurrentCount(nCurrentCount);
                obj->setCurrentLv(nLevel);
                obj->setCurrentSmelting(nSmelting);
                
                if(isWear == true)
                {
                    _infoWearSpirit = obj;
                    setSpiritBuff();
                }
                
                break;
            }
        }
    }
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::SPIRIT_STON )
            {
                _nSpiritStonCount = std::atoi(obj->getCount().c_str());
            }
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                int nCount = std::atoi(obj->getCount().c_str());
                NewRaidManager::getInstance()->setDragonBall(nCount);
            }
        }
    }
    
    
    if ( _callbackMileageReward != nullptr )
    {
        _callbackMileageReward(true, nResult);
        _callbackMileageReward = nullptr;
    }
    
}

void WeaponSpiritManager::requestContract(const std::function<void(bool, int)>& callback)
{
    //
    _callbackContractLv = callback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/UpdateSpiritContract";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseContract(response,dd);
    };
    req->send(cb);
}

void WeaponSpiritManager::responseContract(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackContractLv != nullptr )
        {
            _callbackContractLv(false, 0);
            _callbackContractLv = nullptr;
        }
        return;
    }
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackContractLv != nullptr )
        {
            _callbackContractLv(false, nResult);
            _callbackContractLv = nullptr;
        }
        return;
    }
    
    _nSpiritContractLv = jsonParser["_sprit_contract"].GetInt();
    
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::SPIRIT_STON )
            {
                _nSpiritStonCount = std::atoi(obj->getCount().c_str());
            }
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                int nCount = std::atoi(obj->getCount().c_str());
                NewRaidManager::getInstance()->setDragonBall(nCount);
            }
        }
    }
    
    
    if ( _callbackContractLv != nullptr )
    {
        _callbackContractLv(true, nResult);
        _callbackContractLv = nullptr;
    }
    
}


#pragma mark - Utills
void WeaponSpiritManager::resetSpiritInfo()
{
    for(auto obj : _listInfoWeaponSpirit)
    {
        obj->setHas(false);
        obj->setWear(false);
        obj->setCurrentLv(0);
        obj->setCurrentCount(0);
        obj->setCurrentSmelting(0);
    }
}
