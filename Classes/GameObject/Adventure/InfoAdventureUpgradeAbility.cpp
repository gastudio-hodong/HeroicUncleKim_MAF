//
//  InfoAdventureProperty.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//
#include "InfoAdventureUpgradeAbility.h"

#include "GameObject/InfoItems.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

#pragma mark -
InfoAdventureUpgradeAbility::InfoAdventureUpgradeAbility():
_nLv(0)
,_eType(E_ADVENTURE_PROPERTY_TYPE::NONE)
,_dEffect(0)
{
    _listCost.clear();
}

InfoAdventureUpgradeAbility::~InfoAdventureUpgradeAbility()
{
    _listCost.clear();
}

bool InfoAdventureUpgradeAbility::init()
{
    return true;
}

const int InfoAdventureUpgradeAbility::getLv() const
{
    return _nLv;
}
void InfoAdventureUpgradeAbility::setLv(const int value)
{
    _nLv = value;
}

const double InfoAdventureUpgradeAbility::getEffect() 
{
    return _dEffect;
}
void InfoAdventureUpgradeAbility::setEffect(const double value)
{
    _dEffect = value;
}

InfoAdventurePropertyType* const InfoAdventureUpgradeAbility::getInfoType()
{
    return AdventureManager::getInstance()->getPropertyType(_eType);
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventureUpgradeAbility::getType() const
{
    return _eType;
}
void InfoAdventureUpgradeAbility::setType(const E_ADVENTURE_PROPERTY_TYPE value)
{
    _eType = value;
}

const cocos2d::Vector<InfoItems*>& InfoAdventureUpgradeAbility::getListCost() const
{
    return _listCost;
}
InfoItems* const InfoAdventureUpgradeAbility::getInfoItems(const int nIdx) const
{
    InfoItems* result = nullptr;
    
    for(auto obj : _listCost)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
void InfoAdventureUpgradeAbility::setCost(const cocos2d::Vector<InfoItems*> value)
{
    _listCost = value;
}

const bool InfoAdventureUpgradeAbility::isPartyOption()
{
    bool result = false;
    
    auto obj = AdventureManager::getInstance()->getPropertyType(_eType);
    
    if(obj != nullptr)
        result = obj->isPartyOption();
    
    return result;
}

const bool InfoAdventureUpgradeAbility::isUpgradeConditionMine() const
{
    bool result = true;
    
    for ( int i = 0; i < C_COUNT_MINE_MINERAL; i++ )
    {
        int nCostIdx = (int)E_ITEMS::MINE_STONE_IRON + i;
        
        if ( MineralManager::getInstance()->getCount((E_MINERAL) (i + 1)) < getUpgradeCost(nCostIdx))
        {
            E_ITEMS eItem = E_ITEMS::MINE_STONE_IRON;
            switch ((E_MINERAL)(i + 1)) {
                case E_MINERAL::MINERAL_1:  eItem = E_ITEMS::MINE_STONE_IRON;        break;
                case E_MINERAL::MINERAL_2:  eItem = E_ITEMS::MINE_STONE_COPPER;        break;
                case E_MINERAL::MINERAL_3:  eItem = E_ITEMS::MINE_STONE_SILVER;        break;
                case E_MINERAL::MINERAL_4:  eItem = E_ITEMS::MINE_PIECE_IRON;        break;
                case E_MINERAL::MINERAL_5:  eItem = E_ITEMS::MINE_PIECE_COPPER;        break;
                case E_MINERAL::MINERAL_6:  eItem = E_ITEMS::MINE_PIECE_SILVER;        break;
                case E_MINERAL::MINERAL_7_SAPPHIRE:  eItem = E_ITEMS::MINE_SAPPHIRE;        break;
                case E_MINERAL::MINERAL_8_OPAL:  eItem = E_ITEMS::MINE_OPAL;        break;
                case E_MINERAL::MINERAL_9_AQUA:  eItem = E_ITEMS::MINE_AQUAMARINE;        break;
                case E_MINERAL::MINERAL_10_EMERALD: eItem = E_ITEMS::MINE_EMERALD;        break;
                case E_MINERAL::MINERAL_11_AMETHYST: eItem = E_ITEMS::MINE_AMETHYST;        break;
                default:
                    break;
            }
            
            auto popup = PopupCurrencyDetail::create(eItem);
            popup->show();
            return false;
        }
    }
    
    return result;
}

const bool InfoAdventureUpgradeAbility::isUpgradeConditionCurrency(E_ITEMS eType) const
{
    bool result = true;
    
    int nCurrentCurrency = 0;
    
    switch (eType) {
        case E_ITEMS::GEM:
        {
            nCurrentCurrency = ItemsMoneyManager::getInstance()->getGem();
        }
            break;
        case E_ITEMS::POINT:
        {
            nCurrentCurrency = ItemsMoneyManager::getInstance()->getPoint();
        }
            break;
        default:
            break;
    }
    
    if ( nCurrentCurrency < getUpgradeCost((int)eType))
    {
        auto popup = PopupCurrencyDetail::create(eType);
        popup->show();
        result = false;
    }
    
    return result;
}

const bool InfoAdventureUpgradeAbility::isUpgradeCondition() const
{
    return isUpgradeConditionMine() && isUpgradeConditionCurrency(E_ITEMS::GEM) && isUpgradeConditionCurrency(E_ITEMS::POINT);
}

const int InfoAdventureUpgradeAbility::getUpgradeCost(const int nIdx) const
{
    int result = 0;
    
    auto objCost = getInfoItems(nIdx);
    
    if(objCost != nullptr)
        result = std::atoi(objCost->getCount().c_str());
    
    return result;
}

const bool InfoAdventureUpgradeAbility::tryUpgradePayment()
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    bool result = true;
    
    if(!isUpgradeCondition())
        return false;
    
    // 재화 감소
    ItemsMoneyManager::getInstance()->setPoint(-getUpgradeCost((int)E_ITEMS::POINT));
    ItemsMoneyManager::getInstance()->setGem(-getUpgradeCost((int)E_ITEMS::GEM));
    
    for (int i = 0; i < C_COUNT_MINE_MINERAL; i++)
    {
        const int nCostIdx = (int)E_ITEMS::MINE_STONE_IRON + i;
        int needCount = getUpgradeCost(nCostIdx);
        
        const int count = MineralManager::getInstance()->getCount((E_MINERAL)(i + 1)) - needCount;
        MineralManager::getInstance()->setCount((E_MINERAL)(i + 1), count);
    }
    
    //
    MineralManager::getInstance()->saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    
    return result;
}
