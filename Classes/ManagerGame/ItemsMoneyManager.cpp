//
//  ItemsMoneyManager.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 10. 20..
//
//

#include "ItemsMoneyManager.h"

#include "Common/ConfigKey.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ServerMissionManager.h"

USING_NS_CC;

static ItemsMoneyManager *s_UserInfoMoney = nullptr;
ItemsMoneyManager* ItemsMoneyManager::getInstance()
{
    if(s_UserInfoMoney==nullptr)
    {
        s_UserInfoMoney = new ItemsMoneyManager();
    }
    return s_UserInfoMoney;
}

void ItemsMoneyManager::destroyInstance()
{
    if ( s_UserInfoMoney != nullptr )
    {
        delete s_UserInfoMoney;
        s_UserInfoMoney = nullptr;
    }
}

ItemsMoneyManager::ItemsMoneyManager():
_nEventRaceSkinTicket(0)
{
    _uGold = "0";
    _uKey = "0";
    
    _uGem = "";
    _uPoint = "";
    _uCube = "";
    _uCostumeCoupon = "";
    _uMonsterSoulPiece = "";
    
    
    _uDevilKey="0";
    
    _uFriendship = "0";
    
    _uEventTicket = "0";
}

ItemsMoneyManager::~ItemsMoneyManager()
{
    s_UserInfoMoney = nullptr;
    
}

void ItemsMoneyManager::resetNormal(){
    _uGold = "100";
}

void ItemsMoneyManager::resetDevil(){
    _uDevilGold = "100";
}

#pragma mark-
void ItemsMoneyManager::initGold()
{
    ItemsMoneyManager::getInstance()->setGoldZero(false);
    
    std::string uGoldTemp = UserDefault::getInstance()->getStringForKey(KEY_GOLD);
    if ( uGoldTemp.length() != 0 )
    {
        _uGold = MafAes256::Decrypt(uGoldTemp);
        _uGoldLength = UserDefault::getInstance()->getStringForKey(KEY_LENGTH_GOLD, MafAes256::Encrypt("0"));
        
        if ( _uGold.length() > getGoldLength() )
        {
            _uGoldLength = MafAes256::Encrypt(MafUtils::toString(_uGold.length()));
            UserDefault::getInstance()->setStringForKey(KEY_LENGTH_GOLD, _uGoldLength);
        }
    }
}

void ItemsMoneyManager::setGoldZero(bool bSave)
{
    _uGold = "100";
    
    // save
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(_uGold);
        UserDefault::getInstance()->setStringForKey(KEY_GOLD, enData.c_str());
    }

    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void ItemsMoneyManager::setGold(std::string plus, bool bSave, bool bRefresh)
{
    std::string uGoldPrev = _uGold;
    if ( plus[0] == '-' )
    {
        _uGold = MafUtils::bigSubNum(_uGold, plus,false);
    }
    else
    {
        _uGold = MafUtils::bigAddNum(_uGold, plus, false);
    }
    
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(_uGold);
        UserDefault::getInstance()->setStringForKey(KEY_GOLD, enData.c_str());
        
        //
        if ( _uGold.length() > uGoldPrev.length() )
        {
            int nLengthPrev = atoi(MafAes256::Decrypt(_uGoldLength).c_str());
            if ( _uGold.length() > nLengthPrev )
            {
                _uGoldLength = MafAes256::Encrypt(MafUtils::toString(_uGold.length()));
                UserDefault::getInstance()->setStringForKey(KEY_LENGTH_GOLD, _uGoldLength);
            }
        }
    }
    
    //
    if ( bRefresh == true )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    }
}

std::string ItemsMoneyManager::getGold()
{
    return _uGold;
}

int ItemsMoneyManager::getGoldLength()
{
    return atoi(MafAes256::Decrypt(_uGoldLength).c_str());
}

#pragma mark-
void ItemsMoneyManager::initKey()
{
    setKeyZero();

    std::string uKeyTemp = UserDefault::getInstance()->getStringForKey(KEY_KEY);
    if ( uKeyTemp.length() != 0 )
    {
        _uKey = MafAes256::Decrypt(uKeyTemp);
        _uKeyLength = UserDefault::getInstance()->getStringForKey(KEY_LENGTH_KEY, MafAes256::Encrypt("0"));
        
        if ( _uKey.length() > getKeyLength() )
        {
            _uKeyLength = MafAes256::Encrypt(MafUtils::toString(_uKey.length()));
            UserDefault::getInstance()->setStringForKey(KEY_LENGTH_KEY, _uKeyLength);
        }
    }
}

void ItemsMoneyManager::setKeyZero()
{
    _uKey = "0";
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void ItemsMoneyManager::setKey(std::string plus,bool bSave)
{
    std::string uKeyPrev = _uKey;
    if ( plus[0] == '-' )
    {
        _uKey = MafUtils::bigSubNum(_uKey, plus,false);
    }
    else
    {
        auto totalKey = SaveManager::getHistoryData(E_HISTORY_DATA::K_TOTAL_KEY);
        if ( _uKey.compare("0") != 0 || totalKey.compare("0") == 0 )
        {
            totalKey = MafUtils::bigAddNum(totalKey, plus, false);
            SaveManager::setHistoryData(E_HISTORY_DATA::K_TOTAL_KEY, totalKey);
        }
        
        _uKey = MafUtils::bigAddNum(_uKey, plus,false);
    }
    
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(_uKey);
        UserDefault::getInstance()->setStringForKey(KEY_KEY, enData.c_str());
        
        //
        if ( _uKey.length() > uKeyPrev.length() )
        {
            int nLengthPrev = atoi(MafAes256::Decrypt(_uKeyLength).c_str());
            if ( _uKey.length() > nLengthPrev )
            {
                _uKeyLength = MafAes256::Encrypt(MafUtils::toString(_uKey.length()));
                UserDefault::getInstance()->setStringForKey(KEY_LENGTH_KEY, _uKeyLength);
            }
        }
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void ItemsMoneyManager::setKey(double plus,bool bSave)
{
    setKey(MafUtils::doubleToString(plus),bSave);
}

std::string ItemsMoneyManager::getKey()
{
//    std::string sKey;
//    sKey = _userDefault->getStringForKey(KEY_KEY);
//    if(!sKey.empty()){
//        sKey = MafAes256::Decrypt(sKey);
//    }
//    else{
//        return "0";
//    }
//    return sKey;
    return _uKey;
};

int ItemsMoneyManager::getKeyLength()
{
    return atoi(MafAes256::Decrypt(_uKeyLength).c_str());
}

#pragma mark-
void ItemsMoneyManager::initGem()
{
    _uGem = UserDefault::getInstance()->getStringForKey(KEY_GEM);
    _uGemLength = UserDefault::getInstance()->getStringForKey(KEY_LENGTH_GEM, MafAes256::Encrypt("0"));
    
    std::string uGemTemp = MafUtils::toString(getGem());
    if ( uGemTemp.length() > getGemLength() )
    {
        _uGemLength = MafAes256::Encrypt(MafUtils::toString(uGemTemp.length()));
        UserDefault::getInstance()->setStringForKey(KEY_LENGTH_GEM, _uGemLength);
    }
}

void ItemsMoneyManager::setGem(int plus, bool bSave)
{
    int nGem = getGem() + plus;
    if ( nGem < 0 )
    {
        return;
    }
    
    std::string uGemTemp = MafUtils::toString(nGem);
    _uGem = MafAes256::Encrypt(uGemTemp);
    if ( bSave == true )
    {
        UserDefault::getInstance()->setStringForKey(KEY_GEM, _uGem.c_str());
        
        //
        if ( plus > 0 )
        {
            int gem = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_TOTAL_GEM);
            SaveManager::setHistoryData(E_HISTORY_DATA::K_TOTAL_GEM, (gem+(int)plus));
        }
        else
        {
            ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::USE_GEM, -plus);
        }
        
        //
        int nLengthPrev = atoi(MafAes256::Decrypt(_uGemLength).c_str());
        if ( uGemTemp.length() > nLengthPrev )
        {
            _uGemLength = MafAes256::Encrypt(MafUtils::toString(uGemTemp.length()));
            UserDefault::getInstance()->setStringForKey(KEY_LENGTH_GEM, _uGemLength);
        }
                
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    }
}

int ItemsMoneyManager::getGem()
{
    int nGem = 0;
    if ( _uGem.length() != 0 )
    {
        nGem = atoi(MafAes256::Decrypt(_uGem).c_str());
        if ( nGem < 0 )
            nGem = 0;
    }
    
    return nGem;
}

int ItemsMoneyManager::getGemLength()
{
    return atoi(MafAes256::Decrypt(_uGemLength).c_str());
}

#pragma mark-
void ItemsMoneyManager::initPoint()
{
    _uPoint = UserDefault::getInstance()->getStringForKey(KEY_POINT);
}

void ItemsMoneyManager::setPoint(int plus, bool bSave)
{
    int nPoint = getPoint() + plus;
    if ( nPoint < 0 )
    {
        return;
    }
    
    _uPoint = MafAes256::Encrypt(MafUtils::toString(nPoint));
    if ( bSave == true )
    {
        UserDefault::getInstance()->setStringForKey(KEY_POINT, _uPoint.c_str());
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    }
}

int ItemsMoneyManager::getPoint()
{
    int nPoint = 0;
    if ( _uPoint.length() != 0 )
    {
        nPoint = atoi(MafAes256::Decrypt(_uPoint).c_str());
        if ( nPoint < 0 )
            nPoint = 0;
    }
    
    return nPoint;
};

#pragma mark-
void ItemsMoneyManager::initCube()
{
    _uCube = UserDefault::getInstance()->getStringForKey(KEY_CUBE);
}

void ItemsMoneyManager::setCube(int plus, bool bSave)
{
    int nCube = getCube() + plus;
    if ( nCube < 0 )
    {
        return;
    }
    
    _uCube = MafAes256::Encrypt(MafUtils::toString(nCube));
    if ( bSave == true )
    {
        UserDefault::getInstance()->setStringForKey(KEY_CUBE, _uCube.c_str());
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

int ItemsMoneyManager::getCube()
{
    int nCube = 0;
    if ( _uCube.length() != 0 )
    {
        nCube = atoi(MafAes256::Decrypt(_uCube).c_str());
        if ( nCube < 0 )
            nCube = 0;
    }
    
    return nCube;
}

#pragma mark-
void ItemsMoneyManager::initCostumeCoupon()
{
    _uCostumeCoupon = UserDefault::getInstance()->getStringForKey(KEY_COSTUME_COUPON);
}

void ItemsMoneyManager::setCostumeCoupon(int nCount)
{
    _uCostumeCoupon = MafAes256::Encrypt(MafUtils::toString(nCount));
    
    UserDefault::getInstance()->setStringForKey(KEY_COSTUME_COUPON, _uCostumeCoupon.c_str());

    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

int ItemsMoneyManager::getCostumeCoupon()
{
    int nCostumeCoupon = 0;
    if ( _uCostumeCoupon.length() != 0 )
    {
        nCostumeCoupon = atoi(MafAes256::Decrypt(_uCostumeCoupon).c_str());
        if ( nCostumeCoupon < 0 )
            nCostumeCoupon = 0;
    }
    
    return nCostumeCoupon;
}

#pragma mark-
void ItemsMoneyManager::initMonsterSoulPiece()
{
    _uMonsterSoulPiece = UserDefault::getInstance()->getStringForKey(KEY_MONSTER_SOUL_PIECE);
}

void ItemsMoneyManager::setMonsterSoulPiece(int nCount)
{
    int nMonsterSoulPiece = getMonsterSoulPiece() + nCount;
    if ( nMonsterSoulPiece < 0 )
    {
        nMonsterSoulPiece = 0;
    }
    
    _uMonsterSoulPiece = MafAes256::Encrypt(MafUtils::toString(nMonsterSoulPiece));
    UserDefault::getInstance()->setStringForKey(KEY_MONSTER_SOUL_PIECE, _uMonsterSoulPiece.c_str());

    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

int ItemsMoneyManager::getMonsterSoulPiece()
{
    int nMonsterSoul = 0;
    if ( _uMonsterSoulPiece.length() != 0 )
    {
        nMonsterSoul = atoi(MafAes256::Decrypt(_uMonsterSoulPiece).c_str());
        if ( nMonsterSoul < 0 )
            nMonsterSoul = 0;
    }
    return nMonsterSoul;
}

#pragma mark-
void ItemsMoneyManager::initMythril()
{
    _uMythril = UserDefault::getInstance()->getStringForKey(KEY_MYTHRIL);
}

void ItemsMoneyManager::setMythril(int plus, bool bSave)
{
    int nMythril = getMythril() + plus;
    if ( nMythril < 0 )
    {
        return;
    }
    
    _uMythril = MafAes256::Encrypt(MafUtils::toString(nMythril));
    if ( bSave == true )
    {
        UserDefault::getInstance()->setStringForKey(KEY_MYTHRIL, _uMythril.c_str());
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

int ItemsMoneyManager::getMythril()
{
    int nMythril = 0;
    if ( _uMythril.length() != 0 )
    {
        nMythril = atoi(MafAes256::Decrypt(_uMythril).c_str());
        if ( nMythril < 0 )
            nMythril = 0;
    }
    
    return nMythril;
}

#pragma mark- DEVIL
void ItemsMoneyManager::setDevilGoldZero(bool bSave)
{
    _uDevilGold = "0";
    
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(_uDevilGold);
        UserDefault::getInstance()->setStringForKey(KEY_D_GOLD, enData.c_str());
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void ItemsMoneyManager::setDevilGold(std::string plus, bool bSave, bool bRefresh)
{
    if ( plus[0] == '-' )
    {
        _uDevilGold = MafUtils::bigSubNum(_uDevilGold, plus,false);
    }
    else
    {
        _uDevilGold = MafUtils::bigAddNum(_uDevilGold, plus,false);
    }
    
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(_uDevilGold);
        UserDefault::getInstance()->setStringForKey(KEY_D_GOLD, enData.c_str());
    }

    //
    if ( bRefresh == true )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    }
}
std::string ItemsMoneyManager::getDevilGold()
{
    return _uDevilGold;
}

void ItemsMoneyManager::setDevilKeyZero()
{
    _uDevilKey = "0";
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void ItemsMoneyManager::setDevilKey(double plus,bool bSave)
{
    setDevilKey(MafUtils::doubleToString(plus),bSave);
}

void ItemsMoneyManager::setDevilKey(std::string plus,bool bSave)
{
    if ( plus[0] == '-' )
    {
        _uDevilKey = MafUtils::bigSubNum(_uDevilKey, plus,false);
    }
    else
    {
        _uDevilKey = MafUtils::bigAddNum(_uDevilKey, plus,false);
    }
    
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(_uDevilKey);
        UserDefault::getInstance()->setStringForKey(KEY_D_KEY, enData.c_str());
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

std::string ItemsMoneyManager::getDevilKey()
{
    return _uDevilKey;
}









