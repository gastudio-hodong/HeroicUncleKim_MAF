//
//  MonsterManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "MonsterManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

MonsterManager::MonsterManager(void) :
_ePlace(E_PLACE::DG_NORMAL)

{
    
}

MonsterManager::~MonsterManager(void)
{
    
}

bool MonsterManager::init()
{
    
    for ( int i = 0; i < sizeof(_uSoulCount) / sizeof(TInteger); i++ )
    {
        _uSoulCount[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_uDexLevel) / sizeof(TInteger); i++ )
    {
        _uDexLevel[i] = 0;
    }
    
    return true;
}

#pragma mark - update
void MonsterManager::update(float dt)
{
     
}

#pragma mark - data
void MonsterManager::saveData()
{
    std::stringstream str;
    
    // soul count
    str.str("");
    for ( int i = 1; i <= C_COUNT_MONSTER; i++)
    {
        str << getSoulCountNow(i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_MONSTER_SOUL, MafAes256::Encrypt(str.str()));
    
    // dex level
    str.str("");
    for ( int i = 1; i <= C_COUNT_MONSTER; i++ )
    {
        str << getDexLevelNow(i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_MONSTER_SOUL_LEVEL, MafAes256::Encrypt(str.str()));
}

void MonsterManager::loadData()
{
    std::string str = "";
    
    // soul count
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MONSTER_SOUL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_MONSTER; i++)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setSoulCount(i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setSoulCount(i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // dex level
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MONSTER_SOUL_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for ( int i = 1; i <= C_COUNT_MONSTER; i++ )
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setDexLevel(i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setDexLevel(i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    //
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, isSoulLevelComplete());
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP);
}

#pragma mark - load
void MonsterManager::setLoad()
{
    
}

#pragma mark - info

#pragma mark - get, set : soul
bool MonsterManager::isSoulLevelComplete()
{
    bool bResult = false;
    
    int dexLevelMax = getDexLevelMax();
    
    for ( int i = 1; i <= C_COUNT_MONSTER; i++ )
    {
        int idx = i;

        int dexLevelNow = getDexLevelNow(idx);
        if ( dexLevelNow >= dexLevelMax )
        {
            continue;
        }

        int soulCountMax = getDexInfoNeedCount(dexLevelNow + 1);
        int soulCountNow = getSoulCountNow(idx) - getDexInfoNeedCountTotal(dexLevelNow);
        if ( soulCountNow >= soulCountMax )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}
int MonsterManager::getSoulCountNow(int idx)
{
    int count = 0;
    if (  sizeof(_uSoulCount)/sizeof(TInteger) <= idx )
    {
        return count;
    }
    
    count = _uSoulCount[idx].valueInt();
    return count;
}
void MonsterManager::addSoulCount(int idx, int countAdd)
{
    int count = getSoulCountNow(idx) + countAdd;
    int countMax = getDexInfoNeedCountTotal();
    int countPiece = 0;
    
    if ( count > countMax )
    {
        countPiece = count - countMax;
        count = countMax;
        if ( countAdd < countPiece )
        {
            countPiece = countAdd;
        }
        ItemsMoneyManager::getInstance()->setMonsterSoulPiece(countPiece);
    }
    setSoulCount(idx, count);
}
void MonsterManager::setSoulCount(int idx, int count)
{
    if (  sizeof(_uSoulCount)/sizeof(TInteger) <= idx )
    {
        return;
    }
    
    _uSoulCount[idx] = count;
}

#pragma mark - get, set : dex
double MonsterManager::getDexEffect()
{
    auto value = 1.0;
    auto levelDexMax = getDexLevelMax();
    
    for ( int level = 1; level <= levelDexMax; level++ )
    {
        int count = getDexLevelCount(level);
        
        double effect = pow(1.0 + 0.01 * level, count);
        value *= effect;
    }
    
    // bonus
    double bonus = 1.0;
    bonus += getDexBonusEffect();

    value *= bonus;
    value -= 1;
    return value;
}

int MonsterManager::getDexLevelMax()
{
    return 5;
}

int MonsterManager::getDexLevelNow(int idx)
{
    int levelDex = 0;
    if (  sizeof(_uDexLevel) / sizeof(TInteger) <= idx )
    {
        return levelDex;
    }
    
    levelDex = _uDexLevel[idx].valueInt();
    levelDex = MIN(levelDex, getDexLevelMax());
    
    return levelDex;
}

int MonsterManager::getDexLevelCount(int level)
{
    int nResult = 0;
    for ( int i = 1; i < sizeof(_uDexLevel) / sizeof(TInteger); i++ )
    {
        if ( _uDexLevel[i] == level )
        {
            nResult++;
        }
    }
    return nResult;
}

void MonsterManager::addDexLevel(int idx)
{
    int levelDex = getDexLevelNow(idx) + 1;
    levelDex = MIN(levelDex, getDexLevelMax());
    
    setDexLevel(idx, levelDex);
}
void MonsterManager::setDexLevel(int idx, int levelDex)
{
    if (  sizeof(_uDexLevel) / sizeof(TInteger) <= idx )
    {
        return;
    }
    
    _uDexLevel[idx] = levelDex;
}

int MonsterManager::getDexInfoNeedCountTotal(int levelDexMax/* = -1*/)
{
    int count = 0;
    
    if ( levelDexMax == -1 )
        levelDexMax = getDexLevelMax();
    
    for ( int i = 1; i <= levelDexMax; i++ )
    {
        count += getDexInfoNeedCount(i);
    }
    
    return count;
}

int MonsterManager::getDexInfoNeedCount(int levelDex)
{
    int count = 0;
    switch (levelDex) {
        case 1:     count = 50; break;
        case 2:     count = 100; break;
        case 3:     count = 150; break;
        case 4:     count = 175; break;
        case 5:     count = 200; break;
            
        default:
            break;
    }
    
    return count;
}


#pragma mark - get, set : dex bonus
double MonsterManager::getDexBonusEffect()
{
    int levelDexBonusNow = getDexBonusLevelNow();
    
    double effect = 0;
    if ( levelDexBonusNow >= 1 )      effect += getDexBonusInfoEffect(1);
    if ( levelDexBonusNow >= 2 )      effect += getDexBonusInfoEffect(2);
    if ( levelDexBonusNow >= 3 )      effect += getDexBonusInfoEffect(3);
    if ( levelDexBonusNow >= 4 )      effect += getDexBonusInfoEffect(4);
    if ( levelDexBonusNow >= 5 )      effect += getDexBonusInfoEffect(5);
    
    return effect;
}

int MonsterManager::getDexBonusLevelMax()
{
    return 5;
}

int MonsterManager::getDexBonusLevelNow()
{
    int levelDexBonusNow = 0;
    int levelDexBonusMax = getDexBonusLevelMax();
    
    auto getLevelTotal = [=](int level)->int{

        int total = 0;
        for ( int i = level; i <= levelDexBonusMax; i++ )
        {
            total += getDexLevelCount(i);
        }
        
        return total;
    };
    
    for ( int i = levelDexBonusMax; i > 0; i-- )
    {
        if ( getLevelTotal(getDexBonusInfoNeedDexLevel(i)) >= getDexBonusInfoNeedCount(i) )
        {
            levelDexBonusNow = i;
            break;
        }
    }
    
    return levelDexBonusNow;
}

int MonsterManager::getDexBonusInfoNeedCount(int levelDexBonus)
{
    int count = 0;
    switch (levelDexBonus) {
        case 1:     count = 50; break;
        case 2:     count = 100; break;
        case 3:     count = 150; break;
        case 4:     count = 150; break;
        case 5:     count = 200; break;
            
        default:
            break;
    }
    
    return count;
}

int MonsterManager::getDexBonusInfoNeedDexLevel(int levelDexBonus)
{
    int levelDex = 0;
    switch (levelDexBonus) {
        case 1:     levelDex = 1; break;
        case 2:     levelDex = 2; break;
        case 3:     levelDex = 3; break;
        case 4:     levelDex = 4; break;
        case 5:     levelDex = 5; break;
            
        default:
            break;
    }
    
    return levelDex;
}

double MonsterManager::getDexBonusInfoEffect(int levelDexBonus)
{
    double effect = 0.0;
    switch (levelDexBonus) {
        case 1:     effect = 1.0; break;
        case 2:     effect = 2.0; break;
        case 3:     effect = 3.0; break;
        case 4:     effect = 4.0; break;
        case 5:     effect = 5.0; break;
            
        default:
            break;
    }
    
    return effect;
}

#pragma mark - event
