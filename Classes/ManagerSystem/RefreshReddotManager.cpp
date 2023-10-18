#include "RefreshReddotManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "GameObject/InfoWeapon.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/PetEquipBoxManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

RefreshReddotManager::RefreshReddotManager(void)
{
    
}

RefreshReddotManager::~RefreshReddotManager(void)
{
    
}

bool RefreshReddotManager::init()
{
    return true;
}

#pragma mark - RefreshReddotManager : other function
void RefreshReddotManager::addUI(E_REFRESH_REDDOT eType, cocos2d::Node* node)
{
    //
    auto listener = cocos2d::EventListenerCustom::create(enumToString(eType), [=](cocos2d::EventCustom *event){
        
        bool bVisible = false;
        auto userData = static_cast<cocos2d::ValueMap*>(event->getUserData());
        if ( userData != nullptr )
        {
            cocos2d::ValueMap listData = *userData;
            bVisible = listData["visable"].asBool();
        }
        
        auto target = dynamic_cast<Node*>(event->getCurrentTarget());
        if ( target != nullptr )
        {
            target->setVisible(bVisible);
        }
    });
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node);
    
    //
    node->setVisible(isReddotVisible(eType));
}

void RefreshReddotManager::refreshUI(E_REFRESH_REDDOT eType)
{
    ValueMap listData;
    listData["visable"] = Value(isReddotVisible(eType));
    
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(enumToString(eType), &listData);
}

void RefreshReddotManager::setValue(E_REFRESH_REDDOT eType, bool bValue)
{
    std::string key = "reddot_";
    key.append(enumToString(eType));
    
    UserDefault::getInstance()->setBoolForKey(key.c_str(), bValue);
}

#pragma mark - RefreshReddotManager : visible
bool RefreshReddotManager::isReddotVisible(E_REFRESH_REDDOT eType)
{
    bool bResult = false;
    
    switch (eType) {
        case E_REFRESH_REDDOT::EQUIP_CRAFT:
        {
            E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
            bResult = checkEquipCraft(ePlace);
            break;
        }
        case E_REFRESH_REDDOT::PET_EVOLUTION_CHICK:
        case E_REFRESH_REDDOT::PET_EVOLUTION_HAMSTER:
        case E_REFRESH_REDDOT::PET_EVOLUTION_DRAGON:
        case E_REFRESH_REDDOT::PET_EVOLUTION_PRINCESS:
        case E_REFRESH_REDDOT::PET_EVOLUTION_PURIRING:
        case E_REFRESH_REDDOT::PET_EVOLUTION_HONG:
        case E_REFRESH_REDDOT::PET_EVOLUTION_MOLE:
        case E_REFRESH_REDDOT::PET_EVOLUTION_HANDSOME:
        {
            E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
            E_PET ePet = E_PET::PET_NONE;
            switch (eType) {
                case E_REFRESH_REDDOT::PET_EVOLUTION_CHICK:     ePet = E_PET::CHICK_1;      break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_HAMSTER:   ePet = E_PET::HAMSTER_2;    break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_DRAGON:    ePet = E_PET::DRAGON_3;     break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_PRINCESS:  ePet = E_PET::PRINCESS_4;   break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_PURIRING:  ePet = E_PET::PURIRING_5;   break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_HONG:      ePet = E_PET::HONG_6;       break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_MOLE:      ePet = E_PET::MOLE_7;       break;
                case E_REFRESH_REDDOT::PET_EVOLUTION_HANDSOME:  ePet = E_PET::HANDSOME_8;   break;
                    
                default:
                    break;
            }
            
            bResult = checkPetEvolution(ePlace, ePet);
            break;
        }
        case E_REFRESH_REDDOT::PET_EVOLUTION_ALL:
        {
            bResult = checkPetEvolutionAll();
            break;
        }
        case E_REFRESH_REDDOT::PET_REINFORCE_CHICK:
        case E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER:
        case E_REFRESH_REDDOT::PET_REINFORCE_DRAGON:
        case E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS:
        case E_REFRESH_REDDOT::PET_REINFORCE_PURIRING:
        case E_REFRESH_REDDOT::PET_REINFORCE_HONG:
        case E_REFRESH_REDDOT::PET_REINFORCE_MOLE:
        case E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME:
        {
            E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
            E_PET ePet = E_PET::PET_NONE;
            switch (eType) {
                case E_REFRESH_REDDOT::PET_REINFORCE_CHICK:     ePet = E_PET::CHICK_1;      break;
                case E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER:   ePet = E_PET::HAMSTER_2;    break;
                case E_REFRESH_REDDOT::PET_REINFORCE_DRAGON:    ePet = E_PET::DRAGON_3;     break;
                case E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS:  ePet = E_PET::PRINCESS_4;   break;
                case E_REFRESH_REDDOT::PET_REINFORCE_PURIRING:  ePet = E_PET::PURIRING_5;   break;
                case E_REFRESH_REDDOT::PET_REINFORCE_HONG:      ePet = E_PET::HONG_6;       break;
                case E_REFRESH_REDDOT::PET_REINFORCE_MOLE:      ePet = E_PET::MOLE_7;       break;
                case E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME:  ePet = E_PET::HANDSOME_8;   break;
                    
                default:
                    break;
            }
            
            bResult = checkPetReinforce(ePlace, ePet);
            break;
        }
        case E_REFRESH_REDDOT::PET_REINFORCE_ALL:
        {
            bResult = checkPetReinforceAll();
            break;
        }
        case E_REFRESH_REDDOT::PET_EQUIP_CHICK:
        case E_REFRESH_REDDOT::PET_EQUIP_HAMSTER:
        case E_REFRESH_REDDOT::PET_EQUIP_DRAGON:
        case E_REFRESH_REDDOT::PET_EQUIP_PRINCESS:
        case E_REFRESH_REDDOT::PET_EQUIP_PURIRING:
        case E_REFRESH_REDDOT::PET_EQUIP_HONG:
        case E_REFRESH_REDDOT::PET_EQUIP_MOLE:
        case E_REFRESH_REDDOT::PET_EQUIP_HANDSOME:
        {
            E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
            E_PET ePet = E_PET::PET_NONE;
            switch (eType) {
                case E_REFRESH_REDDOT::PET_EQUIP_CHICK:     ePet = E_PET::CHICK_1;      break;
                case E_REFRESH_REDDOT::PET_EQUIP_HAMSTER:   ePet = E_PET::HAMSTER_2;    break;
                case E_REFRESH_REDDOT::PET_EQUIP_DRAGON:    ePet = E_PET::DRAGON_3;     break;
                case E_REFRESH_REDDOT::PET_EQUIP_PRINCESS:  ePet = E_PET::PRINCESS_4;   break;
                case E_REFRESH_REDDOT::PET_EQUIP_PURIRING:  ePet = E_PET::PURIRING_5;   break;
                case E_REFRESH_REDDOT::PET_EQUIP_HONG:      ePet = E_PET::HONG_6;       break;
                case E_REFRESH_REDDOT::PET_EQUIP_MOLE:      ePet = E_PET::MOLE_7;       break;
                case E_REFRESH_REDDOT::PET_EQUIP_HANDSOME:  ePet = E_PET::HANDSOME_8;   break;
                    
                default:
                    break;
            }
            
            bResult = checkPetEquip(ePlace, ePet);
            break;
        }
        case E_REFRESH_REDDOT::PET_EQUIP_ALL:
        {
            bResult = checkPetEquipAll();
            break;
        }
        case E_REFRESH_REDDOT::DUNGEONPASS_REWARD:
        {
            bResult = checkDungeonPassReward();
            break;
        }
        case E_REFRESH_REDDOT::PET_EQUIP_BOX:
        {
            bResult = checkPetEquipBox();
            break;
        }
        case E_REFRESH_REDDOT::PURCHASE_BANNER:
        case E_REFRESH_REDDOT::EVENT_RACE_MISSION:
        case E_REFRESH_REDDOT::FLASHMAN_MISSION_COMPLETE:
        case E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP:
        case E_REFRESH_REDDOT::DEX_LOOTS_LEVELUP:
        case E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ALL:
        case E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_FOOD:
        case E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ADVENTURE:
        {
            std::string key = "reddot_";
            key.append(enumToString(eType));
            
            bResult = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
            break;
        }
            
        default:
            break;
    }
    return bResult;
}

bool RefreshReddotManager::checkEquipCraft(E_PLACE ePlace)
{
    bool bResult = false;
    
    int weaponEquip = WeaponManager::getInstance()->getEquip();
    int weaponEquipHigh = WeaponManager::getInstance()->getEquipHigh();
    int weaponLevel = WeaponManager::getInstance()->getLevel(weaponEquip);
    
    if ( weaponLevel < C_MAX_EQUIP )
    {
        return bResult;
    }
    
    if ( weaponEquip >= WeaponManager::getInstance()->getCount() )
    {
        return bResult;
    }
    
    auto objEquip = WeaponManager::getInstance()->getInfoWeapon(weaponEquip + 1);
    if ( objEquip->getLegend() == 0 )
    {
        return bResult;
    }
    
    if(weaponEquip < weaponEquipHigh)
        return bResult;
        
    //
    bResult = true;
    
    //
    auto listCost = objEquip->getCost();
    
    for ( int i = 0; i < listCost.size(); i++ )
    {
        auto cost = listCost.at(i);
        
        auto itemNeed = cost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            bResult = false;
            break;
        }
    }
    
    return bResult;
}

bool RefreshReddotManager::checkPetReinforce(E_PLACE ePlace, E_PET ePet)
{
    bool bResult = false;
    if ( ePet == E_PET::PET_NONE )
    {
        return bResult;
    }
    
    if ( PetNewManager::getInstance()->getPet(ePet) == false )
    {
        return bResult;
    }
    
    int nReinforceLevel = PetNewManager::getInstance()->getPetReinforce(ePet);
    if ( nReinforceLevel >= C_MAX_PET_GRADE )
    {
        return bResult;
    }
    
    //
    bResult = true;
    
    //
    std::vector<int> listMine = PetNewManager::getInstance()->getPetReinforceUpgradePrice(ePet, nReinforceLevel);
    for ( int i = 0; i < listMine.size(); i++ )
    {
        E_MINERAL eMineral = (E_MINERAL)i;
        
        int needCount = listMine.at(i);
        int nowCount = MineralManager::getInstance()->getCount(eMineral);
        
        if ( needCount > 0 )
        {
            if ( needCount > nowCount )
            {
                bResult = false;
                break;
            }
        }
    }
    
    return bResult;
}

bool RefreshReddotManager::checkPetReinforceAll()
{
    bool bResult = false;
    
    E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    E_PET listPet[] = {E_PET::CHICK_1, E_PET::HAMSTER_2, E_PET::DRAGON_3, E_PET::PRINCESS_4, E_PET::PURIRING_5, E_PET::HONG_6, E_PET::MOLE_7, E_PET::HANDSOME_8};
    for ( int i = 0; i < sizeof(listPet) / sizeof(E_PET); i++ )
    {
        E_PET ePet = listPet[i];
        
        if ( checkPetReinforce(ePlace, ePet) == true )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

bool RefreshReddotManager::checkPetEvolution(E_PLACE ePlace, E_PET ePet)
{
    bool bResult = false;
    if ( ePet == E_PET::PET_NONE )
    {
        return bResult;
    }
    
    if ( PetNewManager::getInstance()->getPet(ePet) == false )
    {
        return bResult;
    }
    
    int nLevelNow = PetNewManager::getInstance()->getPetEvolution(ePet);
    int nLevelReward = PetNewManager::getInstance()->getPetEvolutionReward(ePet);
    
    if ( nLevelNow < 1 )
    {
        return bResult;
    }
    
    if ( nLevelNow <= nLevelReward )
    {
        return bResult;
    }
    
    //
    bResult = true;
    
    return bResult;
}

bool RefreshReddotManager::checkPetEvolutionAll()
{
    bool bResult = false;
    
    E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    E_PET listPet[] = {E_PET::CHICK_1, E_PET::HAMSTER_2, E_PET::DRAGON_3, E_PET::PRINCESS_4, E_PET::PURIRING_5, E_PET::HONG_6, E_PET::MOLE_7, E_PET::HANDSOME_8};
    for ( int i = 0; i < sizeof(listPet) / sizeof(E_PET); i++ )
    {
        E_PET ePet = listPet[i];
        
        if ( checkPetEvolution(ePlace, ePet) == true )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

bool RefreshReddotManager::checkPetEquip(E_PLACE ePlace, E_PET ePet)
{
    bool bResult = false;
    if ( ePet == E_PET::PET_NONE )
    {
        return bResult;
    }
    
    auto listPetEquip = PetNewManager::getInstance()->getListPetEquip(ePet);
    for ( auto& objPetEquip : listPetEquip )
    {
        int levelNow = PetNewManager::getInstance()->getPetEquipLevel(objPetEquip->getIdx());
        int levelMax = PetNewManager::getInstance()->getPetEquipLevelMax();
        if ( levelNow <= 0 || levelNow >= levelMax )
        {
            continue;
        }
        
        int itemNeed = PetNewManager::getInstance()->getPetEquipAmountNeed(levelNow);
        int itemNow = PetNewManager::getInstance()->getPetEquipAmount(objPetEquip->getIdx());
        if ( itemNow >= itemNeed )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

bool RefreshReddotManager::checkPetEquipAll()
{
    bool bResult = false;
    
    E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    E_PET listPet[] = {E_PET::CHICK_1, E_PET::HAMSTER_2, E_PET::DRAGON_3, E_PET::PRINCESS_4, E_PET::PURIRING_5, E_PET::HONG_6, E_PET::MOLE_7, E_PET::HANDSOME_8};
    for ( int i = 0; i < sizeof(listPet) / sizeof(E_PET); i++ )
    {
        E_PET ePet = listPet[i];
        
        if ( checkPetEquip(ePlace, ePet) == true )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

bool RefreshReddotManager::checkDungeonPassReward()
{
    bool bResult = false;
    
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( objPass != nullptr && objPass->isReward() )
    {
        bResult = true;
    }
        
    return bResult;
}

bool RefreshReddotManager::checkPetEquipBox()
{
    bool bResult = false;
    
    int petEquipBox = PetEquipBoxManager::getInstance()->getEquipBoxCount();
    int petEquipEventBox = PetEquipBoxManager::getInstance()->getEquipBoxEventCount();
    int petEquipCashBox = PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount();
    
    if(EventManager::getInstance()->isActivatedEvent(16) == true)
    {
        if(petEquipEventBox > 0)
            bResult = true;
    }
    else
    {
        if(petEquipBox > 0)
            bResult = true;
    }
    
    if(petEquipCashBox > 0)
        bResult = true;
        
    return bResult;
}
