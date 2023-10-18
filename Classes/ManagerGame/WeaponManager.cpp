//
//  WeaponManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "WeaponManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

WeaponManager::WeaponManager(void) :
_ePlace(E_PLACE::DG_NORMAL),

_uLevel(1),

_uEquip(1),
_uEquipVisual(1),
_uEquipHigh(1),
_uEquipReborn(1),
_uEquipPermanent(1),

_equipAttackIdx(0),
_equipAttacklevel(0),
_equipAttackHonor(0),
_equipAttackValue("0")
{
    
}

WeaponManager::~WeaponManager(void)
{
    
}

bool WeaponManager::init()
{
    
    return true;
}

#pragma mark - update
void WeaponManager::update(float dt)
{
     
}

#pragma mark - data
void WeaponManager::saveData()
{
    std::stringstream str;
    
    //
    str.str("");
    str << MafUtils::toString(_uEquip.value());
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP, MafAes256::Encrypt(str.str()));
    
    
    str.str("");
    str << MafUtils::toString(_uEquipVisual.value());
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP_VISUAL, MafAes256::Encrypt(str.str()));
    
    
    str.str("");
    str << MafUtils::toString(_uEquipHigh.value());
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP_HIGH, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(_uEquipReborn.value());
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP_REBORN, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(_uEquipPermanent.value());
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP_PERMANENT, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(_uLevel.value());
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP_LEVEL, MafAes256::Encrypt(str.str()));
    
    // refine upgrade level
    str.str("");
    for( const auto& obj : _listEquipReinforce )
    {
        if ( str.str().length() != 0 )
        {
            str << ",";
        }
        str << MafUtils::format("%d_%d", obj.first, obj.second);
    }
    UserDefault::getInstance()->setStringForKey(KEY_EQUIP_REINFORCE, MafAes256::Encrypt(str.str()));
}

void WeaponManager::loadData()
{
    std::string str = "";
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP);
    if ( !str.empty() )
    {
        _uEquip = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uEquip = 1;
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP_VISUAL);
    if ( !str.empty() )
    {
        _uEquipVisual = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uEquipVisual = 1;
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP_HIGH);
    if ( !str.empty() )
    {
        _uEquipHigh = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uEquipHigh = 1;
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP_REBORN);
    if ( !str.empty() )
    {
        _uEquipReborn = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uEquipReborn = 1;
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP_PERMANENT);
    if ( !str.empty() )
    {
        _uEquipPermanent = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uEquipPermanent = 1;
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP_LEVEL);
    if ( !str.empty() )
    {
        _uLevel = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uLevel = 1;
    }
    
    // reinforce level
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_EQUIP_REINFORCE);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        std::istringstream stream(str);
        std::string field = "";
        while ( std::getline(stream, field, ',') )
        {
            auto offset = field.find("_");
            if(offset != std::string::npos)
            {
                std::string str1 = field.substr(0,offset);
                std::string str2 = field.substr(offset+1);
                
                if ( MafUtils::isNumeric(str1) == false || MafUtils::isNumeric(str2) == false )
                {
                    continue;
                }
                
                setReinforce(atoi(str1.c_str()), atoi(str2.c_str()));
            }
        }
    }
}

#pragma mark - load
void WeaponManager::setLoad()
{
    setLoadWeapon();
}

void WeaponManager::setLoadWeapon()
{
    _listInfoWeapon.clear();
    
    //
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::WEAPON);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
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
  
        int idx = jsonValue["idx"].GetInt();
        
        std::string nameKey = jsonValue["name_key"].GetString();
        std::string descKey = jsonValue["comment_key"].GetString();
        
        std::string cost = jsonValue["buy_cost"].GetString();
        
        int legned = jsonValue["islegend"].GetInt();
        
        std::string enhanceCost = jsonValue["enhance_cost"].GetString();
        int enhanceMax = jsonValue["enhance_max_count"].GetInt();
        
        auto objInfo = InfoWeapon::create();
        objInfo->setIdx(idx);
        objInfo->setNameKey(nameKey);
        objInfo->setCommentKey(descKey);
        objInfo->setLegend(legned);
        objInfo->setCost(cost);
        objInfo->setEnhanceMax(enhanceMax);
        objInfo->setEnhanceCost(enhanceCost);
        
        _listInfoWeapon.pushBack(objInfo);
    }
}

#pragma mark - info
cocos2d::Vector<InfoWeapon*> WeaponManager::getListWeaponAll()
{
    return _listInfoWeapon;
}

InfoWeapon* WeaponManager::getInfoWeapon(int idx)
{
    for ( auto& objWeapon : _listInfoWeapon )
    {
        if ( objWeapon->getIdx() == idx )
            return objWeapon;
    }
    
    return nullptr;
}

#pragma mark - get, set
std::string WeaponManager::getImagePath(int idx)
{
    return MafUtils::format("Assets/icon_weapon/icon_weapon_%03d.png", idx);
}

std::string WeaponManager::getBuyPrice(int idx, int level)
{
    auto price = getBuyPriceoriginal(idx, level);
    
    auto per  = UserInfoManager::getInstance()->getBuySaleRate(_ePlace);
    if ( per < 100 )
    {
        price = MafUtils::bigMulNum(price, MafUtils::toString(per));
        price.pop_back();
        price.pop_back();
    }
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_3) )
    {
        price = MafUtils::bigMulNum(price, MafUtils::toString(5));
        price.pop_back(); // 50%는 반이니까 다시 빼줄필요없다
    }
    
    return price;
}

std::string WeaponManager::getBuyPriceoriginal(int idx, int level)
{
    std::string price;
    
    int equipStart = 1;
    
    std::string needMoney = "100";
    
    auto find = _listInfoWeaponPrice.find(idx);
    if ( find != _listInfoWeaponPrice.end() )
    {
        return find->second.lv[level];
    }
    else
    {
        find = _listInfoWeaponPrice.find(idx - 1);
        if ( find != _listInfoWeaponPrice.end() )
        {
            //전층의 데이터가 있으면 그 다음층 값만 추가로 더해주면 된다.
            equipStart = idx - 1;
            needMoney = find->second.lv[0]; //첫번째 무기값
        }
        
        for ( int i = equipStart; i< idx; i++ )
        {
            if (i < E_WEAPON::WEAPON_68)
                needMoney = MafUtils::bigMulNum(needMoney, "6"); //6배
            else if (i < E_WEAPON::WEAPON_76_MINE)
                needMoney = MafUtils::bigMulNum(needMoney, "36");
            else if (i < E_WEAPON::WEAPON_82_L)
                needMoney.append("00");
            else if (i < E_WEAPON::WEAPON_89)
                needMoney = MafUtils::bigMulNum(needMoney, "5000");
            else if (i < E_WEAPON::WEAPON_92_L)
                needMoney = MafUtils::bigMulNum(needMoney, "50000");
            else if (i < E_WEAPON::WEAPON_99)
                needMoney.append("0000");
            else
                needMoney.append("00000");
        }
    }
    
    WeaponPrice listPrice;
    listPrice.lv[0] = needMoney;
    
    std::string moneyPer = "5";
    for ( int i = 1; i <= 5; i++ )
    {
        std::string temp = MafUtils::bigMulNum(needMoney, moneyPer); //50%
        temp.pop_back();
        
        if ( i > 1 )
            temp = MafUtils::bigMulNum(temp, MafUtils::toString(i));
        
        listPrice.lv[i] = MafUtils::bigAddNum(needMoney, temp);
    }
    
    //
    _listInfoWeaponPrice.insert( std::pair<int, WeaponPrice>(idx, listPrice));
    return listPrice.lv[level];
}

int WeaponManager::getCount()
{
    return (int)_listInfoWeapon.size();
}

int WeaponManager::getLegendExistCount()
{
    int count = 0;
    
    for ( int i = E_WEAPON::WEAPON_77_L; i <= getEquipHigh(); i++ )
    {
        auto obj = getInfoWeapon(i);
        if ( obj != nullptr && obj->getLegend() > 0 )
        {
            count++;
        }
    }
    
    return count;
}

std::string WeaponManager::getAttack(int idx, int level, int honor/* = 0*/)
{
    if (_equipAttackValue.length() != 0)
    {
        if (_equipAttackIdx == idx && _equipAttacklevel == level && _equipAttackHonor == honor)
        {
            return _equipAttackValue;
        }
    }
    
    std::string attack = "50";
    std::string attackPer = "2";
    
    for ( int i = E_WEAPON::WEAPON_1; i < idx; i++ )
    {
        if (i < E_WEAPON::WEAPON_68) {
            attack = MafUtils::bigMulNum(attack, "3");
        }else if (i < E_WEAPON::WEAPON_76_MINE) {
            attack = MafUtils::bigMulNum(attack, "15");
        }else if (i < E_WEAPON::WEAPON_82_L) {
            attack = MafUtils::bigMulNum(attack, "20");
        }else if (i < E_WEAPON::WEAPON_89) {
            attack = MafUtils::bigMulNum(attack, "20");
        }else if (i < E_WEAPON::WEAPON_92_L) {
            attack = MafUtils::bigMulNum(attack, "50");
        }else if (i < E_WEAPON::WEAPON_99) {
            attack = MafUtils::bigMulNum(attack, "20");
        }else {
            attack = MafUtils::bigMulNum(attack, "50");
        }
    }

    if ( level > 0 )
    {
        int tmpLevel = level + honor;
        
        std::string temp = MafUtils::bigMulNum(attack, attackPer);
        temp = MafUtils::bigMulNum(temp, MafUtils::toString(tmpLevel-1));
        temp.pop_back();
        
        attack = MafUtils::bigAddNum(attack, temp);
    }

    _equipAttackIdx = idx;
    _equipAttacklevel = level;
    _equipAttackHonor = honor;
    
    _equipAttackValue = attack;
    
    return attack;
}


#pragma mark - get, set : level
int WeaponManager::getLevel(int idx)
{
    int level = 0;
    
    if( idx < getEquip() )
    {
        level = C_MAX_EQUIP;
    }
    else if( idx == getEquip() )
    {
        level = _uLevel.valueInt();
    }
    
    return level;
}

int WeaponManager::getLevelMax()
{
    return C_MAX_EQUIP;
}

void WeaponManager::setLevel(int level)
{
    _uLevel = level;
}

#pragma mark - get, set : equip
int WeaponManager::getEquip()
{
    int idx = _uEquip.valueInt() > 0 ? _uEquip.valueInt() : 1;
    return idx;
}

void WeaponManager::setEquip(int idx)
{
    _uEquip = idx;
    
    if ( getEquipHigh() < _uEquip )
    {
        setEquipHigh(idx);
    }
}

int WeaponManager::getEquipVisual()
{
    int idx = _uEquipVisual.valueInt() > 0 ? _uEquipVisual.valueInt() : 1;
    return idx;
}

void WeaponManager::setEquipVisual(int idx)
{
    _uEquipVisual = idx;
}

int WeaponManager::getEquipHigh()
{
    int idx = _uEquipHigh.valueInt() > 0 ? _uEquipHigh.valueInt() : 1;
    return idx;
}

void WeaponManager::setEquipHigh(int idx)
{
    _uEquipHigh = idx;
}

int WeaponManager::getEquipReborn()
{
    int idx = _uEquipReborn.valueInt() > 0 ? _uEquipReborn.valueInt() : 1;
    return idx;
}

void WeaponManager::setEquipReborn(int idx)
{
    _uEquipReborn = idx;
}

int WeaponManager::getEquipPermanent()
{
    int idx = _uEquipPermanent.valueInt() > 0 ? _uEquipPermanent.valueInt() : 1;
    return idx;
}

void WeaponManager::setEquipPermanent(int idx)
{
    _uEquipPermanent = idx;
}

int WeaponManager::getReinforce(int idx)
{
    int level = 0;
    
    auto iter = _listEquipReinforce.find(idx);
    if ( iter != _listEquipReinforce.end() )
    {
        level = _listEquipReinforce[idx];
    }
    
    return level;
}
void WeaponManager::setReinforce(int idx, int level)
{
    _listEquipReinforce[idx] = level;
}

double WeaponManager::getReinforceEffect()
{
    double value = 1;
    
    for ( auto& objWeapon : _listInfoWeapon )
    {
        double effect = 1.0 + getReinforceEffect(objWeapon->getIdx());
        value *= effect;
    }
    
    value -= 1;
    return value;
}
double WeaponManager::getReinforceEffect(int idx, int reinforce/* = -1*/)
{
    if ( reinforce == -1 )
    {
        reinforce = getReinforce(idx);
    }

    return 0.01 * ((int)(idx*0.1) + 1) * reinforce;
}

#pragma mark - get, set : dex
double WeaponManager::getDexEffect()
{
    double value = 1;
    int countPermanent = WeaponManager::getInstance()->getEquipPermanent();
   
    value = pow(1.02, countPermanent); // 추후 무기 이외 효과 추가시 직접 숫자 적용 아닌 함수 제작 필요
    
    // bonus
    double bonus = 1.0;
    bonus += getDexBonusEffect();

    value *= bonus;
    value -= 1;
    return value;
}

#pragma mark - get, set : dex bonus
double WeaponManager::getDexBonusEffect()
{
    int levelDexBonusNow = getDexBonusLevelNow();
    
    double effect = 0;
    if ( levelDexBonusNow >= 1 )      effect += getDexBonusInfoEffect(1);
    if ( levelDexBonusNow >= 2 )      effect += getDexBonusInfoEffect(2);
    if ( levelDexBonusNow >= 3 )      effect += getDexBonusInfoEffect(3);
    if ( levelDexBonusNow >= 4 )      effect += getDexBonusInfoEffect(4);
    
    return effect;
}

int WeaponManager::getDexBonusLevelMax()
{
    return 4;
}

int WeaponManager::getDexBonusLevelNow()
{
    int levelDexBonusNow = 0;
    int levelDexBonusMax = getDexBonusLevelMax();
    
    int countPermanent = WeaponManager::getInstance()->getEquipPermanent();
    

    for ( int i = levelDexBonusMax; i > 0; i-- )
    {
        if ( countPermanent >= getDexBonusInfoNeedCount(i) )
        {
            levelDexBonusNow = i;
            break;
        }
    }
    
    return levelDexBonusNow;
}

int WeaponManager::getDexBonusInfoNeedCount(int levelDexBonus)
{
    int count = 0;
    switch (levelDexBonus) {
        case 1:     count = 30; break;
        case 2:     count = 60; break;
        case 3:     count = 90; break;
        case 4:     count = 120; break;
            
        default:
            break;
    }
    
    return count;
}

double WeaponManager::getDexBonusInfoEffect(int levelDexBonus)
{
    double effect = 0.0;
    switch (levelDexBonus) {
        case 1:     effect = 1.5; break;
        case 2:     effect = 2.0; break;
        case 3:     effect = 2.5; break;
        case 4:     effect = 3.0; break;
            
        default:
            break;
    }
    
    return effect;
}


#pragma mark - event
int WeaponManager::onReset(int floor)
{
    //
    setEquipReborn(getEquip());
    
    //
    if ( floor != -1 )
    {
        if ( UserInfoStackManager::getInstance()->getReviveSpecial() > 0 )
        {
            if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_30))
            {//음흑의 불꽃 : 강화, 초강화 환생시 퀘스트, 무기 초기화 되지 않음
                saveData();
                return 0;
            }
        }
    }
    
    setEquip(E_WEAPON::WEAPON_1);
    setEquipVisual(E_WEAPON::WEAPON_1);
    setLevel(1);
    if (floor >= 1001)
    { //5000층이상유저
        setEquip(E_WEAPON::WEAPON_58);
        setEquipVisual(E_WEAPON::WEAPON_58);
    }
    
    if ( getEquipPermanent() > getEquip() )
    {
        setEquip(getEquipPermanent());
        setEquipVisual(getEquipPermanent());
    }
    
    if ( getEquip() != E_WEAPON::WEAPON_1 )
    {
        setLevel(C_MAX_EQUIP);
    }
    
    //
    if ( floor != -1 )
    {
        saveData();
    }
    
    return 0;
}

int WeaponManager::onWeaponBuy(int idx)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    int level = getLevel(idx);
    int levelMax = getLevelMax();
    bool bLevelMax = level >= levelMax ? true : false;
    
    if ( bLevelMax == true )
    {
        return 1;
    }
    
    auto itemNeed = WeaponManager::getInstance()->getBuyPrice(idx, level);
    auto itemNow = ItemsMoneyManager::getInstance()->getGold();
    if ( MafUtils::compareStr(itemNow, itemNeed) == false )
    {
        return 2;
    }
    
    //
    itemNeed.insert(0, "-");
    ItemsMoneyManager::getInstance()->setGold(itemNeed);
    
    //
    level++;
    if ( level >= levelMax )
    {
        level = levelMax;
    }
    
    if ( level == 1 )
    {
        setEquip(idx);
        setEquipVisual(idx);
        setLevel(level);
    }
    else
    {
        setLevel(level);
    }
    
    //
    saveData();
    
    //
    ValueMap listData;
    listData["idx"] = Value(idx);
    listData["level"] = Value(level);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_BUY, listData);
    
    //
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EQUIP_CRAFT);
    
    //
    UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_EQUIP, false);
    
    if ( level == 1 )
    {
        if ( idx == E_WEAPON::WEAPON_5 )
            MafAnalyticsManager::LogEvent(kAnalEventWeaponBuy5, kRepeatTrue);
        else if ( idx == E_WEAPON::WEAPON_10 )
            MafAnalyticsManager::LogEvent(kAnalEventWeaponBuy10, kRepeatTrue);
        else if ( idx == E_WEAPON::WEAPON_15 )
            MafAnalyticsManager::LogEvent(kAnalEventWeaponBuy15, kRepeatTrue);
        else if ( idx == E_WEAPON::WEAPON_20 )
            MafAnalyticsManager::LogEvent(kAnalEventWeaponBuy20, kRepeatTrue);
        else if ( idx == E_WEAPON::WEAPON_25 )
            MafAnalyticsManager::LogEvent(kAnalEventWeaponBuy25, kRepeatTrue);
        else if ( idx == E_WEAPON::WEAPON_30 )
            MafAnalyticsManager::LogEvent(kAnalEventWeaponBuy30, kRepeatTrue);
        
        if ( idx == E_WEAPON::WEAPON_40 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_ALLWEAPON, 1, 1);
        }
    }
    
    return 0;
}

int WeaponManager::onWeaponBuyLegend(int idx)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    auto objWeapon = getInfoWeapon(idx);

    auto listCost = objWeapon->getCost();
    
    int weaponEquip = WeaponManager::getInstance()->getEquip();
    int weaponEquipLevel = getLevel(weaponEquip);
    int weaponEquipLevelMax = getLevelMax();
    if ( weaponEquipLevel < weaponEquipLevelMax )
    {
        return 1;
    }

    if ( weaponEquip + 1 != idx )
    {
        return 1;
    }
    
    // cost mine
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto cost = listCost.at(i);
        
        auto itemNeed = cost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 2;
        }
    }
    
    
    //
    setEquipHigh(idx);
    
    //
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto item = listCost.at(i);
        ItemsManager::getInstance()->subItem(item);
    }

    //
    saveData();
    MineralManager::getInstance()->saveData();

    //
    ValueMap listData;
    listData["idx"] = Value(idx);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_BUY_LEGEND, listData);

    //
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EQUIP_CRAFT);
    
    return 0;
}

int WeaponManager::onWeaponBuyPermanent(int idx)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    auto itemGoldNeed = WeaponManager::getInstance()->getBuyPrice(idx+BUY_WEAPON_PERMANENT_GOLD_GRADE, 0);
    auto itemGoldNow = ItemsMoneyManager::getInstance()->getGold();
    
    auto itemGemNeed = BUY_WEAPON_PERMANENT_GEM;
    auto itemGemNow = ItemsMoneyManager::getInstance()->getGem();
    
    if ( MafUtils::compareStr(itemGoldNow, itemGoldNeed) == false )
    {
        return 2;
    }
    
    if ( itemGemNow < itemGemNeed )
    {
        return 2;
    }
    
    int weaponEquipPermanent = WeaponManager::getInstance()->getEquipPermanent() + 1;
    if ( idx != weaponEquipPermanent )
    {
        return 1;
    }
   
    //
    itemGoldNeed.insert(0, "-");
    ItemsMoneyManager::getInstance()->setGold(itemGoldNeed);
    ItemsMoneyManager::getInstance()->setGem(-itemGemNeed);

    //
    setEquipPermanent(idx);
    
    //
    saveData();
    
    //
    ValueMap listData;
    listData["idx"] = Value(idx);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_BUY_PERMANENT, listData);
    
    //
    if ( idx >= E_WEAPON::WEAPON_50 )
        MafGooglePlay::UnlockAchievement(ACHIEVE_50_WOOD, 1, 1);
    if ( idx >= E_WEAPON::WEAPON_100_L )
        MafGooglePlay::UnlockAchievement(ACHIEVE_1000_WOOD, 1, 1);
    
    MafAnalyticsManager::LogEvent(kAnalEventWeaponPermanent, kRepeatTrue);
    
    return 0;
}

int WeaponManager::onWeaponChange(int idx)
{
    /*
     0 : 성공
     1 : 조건 안됨
     */
    
    int weaponEquipVisual = WeaponManager::getInstance()->getEquipVisual();
    if ( idx == weaponEquipVisual )
    {
        return 1;
    }
    
    int weaponEquip = WeaponManager::getInstance()->getEquip();
    if ( idx > weaponEquip )
    {
        return 1;
    }
    
    //
    setEquipVisual(idx);
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_INFO);
    
    return 0;
}

int WeaponManager::onWeaponReinforce(int idx)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    auto objWeapon = getInfoWeapon(idx);

    auto listCost = objWeapon->getEnhanceCost();
    
    int weaponEquipPermanent = WeaponManager::getInstance()->getEquipPermanent();
    int weaponEquipReinforceLevelNow = getReinforce(idx);
    int weaponEquipReinforceLevelMax = objWeapon->getEnhanceMax();
    if ( weaponEquipReinforceLevelNow >= weaponEquipReinforceLevelMax )
    {
        return 1;
    }

    if ( weaponEquipPermanent < idx )
    {
        return 1;
    }
    
    // cost mine
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto cost = listCost.at(i);
        
        auto itemNeed = cost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 2;
        }
    }
    
    
    //
    setReinforce(idx, weaponEquipReinforceLevelNow + 1);
    
    //
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto item = listCost.at(i);
        ItemsManager::getInstance()->subItem(item);
    }

    //
    saveData();

    //
    ValueMap listData;
    listData["idx"] = Value(idx);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_BUY_REINFORCE, listData);
    
    return 0;
}

void WeaponManager::onWeaponBuyMax()
{
    PopupLoading::show();
    
    // thread
    auto t = std::thread([this](){
        
        bool bBreak = false;
        bool bLevel = false;
        
        int weaponEquip = getEquip();
        int weaponEquipFinish = weaponEquip;
        
        int weaponLevel = getLevel(weaponEquip);
        int weaponLevelFinish = weaponLevel;
        
        std::string currencyGold= ItemsMoneyManager::getInstance()->getGold();
        std::string currencyGoldNeed = "0";
        
        for ( int i = (int)weaponEquip; i <= getCount(); i++ )
        {
            auto objWeapon = getInfoWeapon(i);
            if ( objWeapon->getLegend() > 0 && objWeapon->getIdx() > getEquipHigh() )
            {
                break;
            }
            
            for ( int j = weaponLevel; j < C_MAX_EQUIP; j++ )
            {
                
                auto currencyGoldNeedTemp = getBuyPrice(i, j);
                currencyGoldNeedTemp = MafUtils::bigAddNum(currencyGoldNeedTemp, currencyGoldNeed);
                if( MafUtils::compareStr(currencyGold, currencyGoldNeedTemp) == true )
                {
                    currencyGoldNeed = currencyGoldNeedTemp;
                    
                    weaponEquipFinish = i;
                    weaponLevelFinish = j + 1;
                    bLevel = true;
                    
                    if ( weaponEquipFinish == E_WEAPON::WEAPON_40 && weaponLevelFinish == 1 )
                    {
                        MafGooglePlay::UnlockAchievement(ACHIEVE_ALLWEAPON, 1, 1);
                    }
                }
                else
                {
                    bBreak = true;
                    break;
                }
            }
            
            weaponLevel = 0;
            if ( bBreak == true )
            {
                break;
            }
        }
        
        //
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

            PopupLoading::hide();
            
            if ( bLevel == false )
            {
                return;
            }
            
            std::string currencyGoldNeedTemp = currencyGoldNeed;
            currencyGoldNeedTemp.insert(0, "-");
            ItemsMoneyManager::getInstance()->setGold(currencyGoldNeedTemp);
            
            setEquip(weaponEquipFinish);
            setEquipVisual(weaponEquipFinish);
            setLevel(weaponLevelFinish);
            
            //
            saveData();
            
            //
            RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_BUY);
            
            //
            RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EQUIP_CRAFT);
            
            //
            UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_EQUIP, false);
             
        });
    });
    t.detach();
}
