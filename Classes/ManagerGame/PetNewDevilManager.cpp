//
//  PetNewDevilManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "PetNewDevilManager.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"
#include "Common/ConfigGameDevil.h"

#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PetNewDevilManager::PetNewDevilManager(void) :
_ePlace(E_PLACE::DG_DEVIL_DOM)
{
    
}

PetNewDevilManager::~PetNewDevilManager(void)
{
    
}

bool PetNewDevilManager::init()
{
    for ( int i = 0; i < sizeof(_uPetLevelDevil) / sizeof(TInteger); i++ )
    {
        _uPetLevelDevil[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_uPetGradeDevil) / sizeof(TInteger); i++ )
    {
        _uPetGradeDevil[i] = 1;
    }
    
    return true;
}

#pragma mark - update
void PetNewDevilManager::update(float dt)
{
     
}

#pragma mark - data
void PetNewDevilManager::saveData()
{
    std::stringstream str;
    
    // level
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET_DEVIL; i++ )
    {
        int idx = (i-1)+(E_PET::D_1_PENGUIN);
        str << getPetLevel((E_PET)idx) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_D_PET_LEVEL, MafAes256::Encrypt(str.str()));
    
    // grade
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET_DEVIL; i++ )
    {
        int idx = (i-1)+(E_PET::D_1_PENGUIN);
        str << getPetGrade((E_PET)idx) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_D_PET_GRADE, MafAes256::Encrypt(str.str()));
}

void PetNewDevilManager::loadData()
{
    std::string str = "";
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_D_PET_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_PET_DEVIL; i++)
        {
            int idx = (i-1)+(E_PET::D_1_PENGUIN);
            
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setPetLevel((E_PET)idx, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setPetLevel((E_PET)idx, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_D_PET_GRADE);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_PET_DEVIL; i++)
        {
            int idx = (i-1)+(E_PET::D_1_PENGUIN);
            
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setPetGrade((E_PET)idx, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(str.c_str());
                if ( str.length() == 0 )
                    ret = 1;
                
                setPetGrade((E_PET)idx, ret);
                break;
            }
        }
    }
    
}

#pragma mark - load
void PetNewDevilManager::setLoad()
{
    
}

#pragma mark - info

#pragma mark - get, set : pet
std::string PetNewDevilManager::getPetName(E_PET ePet)
{
    std::string strName = "";
    
    switch (ePet) {
        case E_PET::D_1_PENGUIN:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_penguin"));
            break;
        }
        case E_PET::D_2_HEDGEHOG:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_hedgehog"));
            break;
        }
        case E_PET::D_3_GATEKEEPER:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_gatekeeper"));
            break;
        }
        case E_PET::D_4_FLASH:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_flash"));
            break;
        }
            
        default:
            break;
    }
    
    return strName;
}

std::string PetNewDevilManager::getPetExplain(E_PET ePet)
{
    std::string strExpain = "";
    switch (ePet) {
        case E_PET::D_1_PENGUIN:        strExpain.append(GAME_TEXT("t_pet_d2_contents_penguin"));        break;
        case E_PET::D_2_HEDGEHOG:       strExpain.append(GAME_TEXT("t_pet_d2_contents_hedgehog"));       break;
        case E_PET::D_3_GATEKEEPER:     strExpain.append(GAME_TEXT("t_pet_d2_contents_gatekeeper"));     break;
        case E_PET::D_4_FLASH:          strExpain.append(GAME_TEXT("t_pet_d2_contents_flash"));          break;
            
        default:
            break;
    }
    
    return strExpain;
}

std::string PetNewDevilManager::getPetUpgradeExplain(E_PET ePet)
{
    std::string strExpain = "";
    switch (ePet) {
        case E_PET::D_1_PENGUIN:        strExpain.append(GAME_TEXT("t_pet_d2_option_penguin"));        break;
        case E_PET::D_2_HEDGEHOG:       strExpain.append(GAME_TEXT("t_pet_d2_option_hedgehog"));       break;
        case E_PET::D_3_GATEKEEPER:     strExpain.append(GAME_TEXT("t_pet_d2_option_gatekeeper"));     break;
        case E_PET::D_4_FLASH:          strExpain.append(GAME_TEXT("t_pet_d2_option_flash"));          break;
            
        default:
            break;
    }
    
    return strExpain;
}

std::string PetNewDevilManager::getPetImagePath(E_PET ePet)
{
    std::string strPath = MafUtils::format("Assets/icon_pet/peticon_%02d_%d.png", ePet, 1);
    return strPath;
}

double PetNewDevilManager::getPetCoolTime(E_PET ePet)
{
    double nTime = 0;
    
    if ( ePet == E_PET::D_2_HEDGEHOG )     nTime = 3;
    else if ( ePet == E_PET::D_3_GATEKEEPER )   nTime = 10;
     
    return nTime;
}

double PetNewDevilManager::getPetEffect(E_PET ePet, int level)
{
    double nEffect = 0;
    
    switch (ePet) {
        case E_PET::D_1_PENGUIN:
            nEffect = 50;
            nEffect = 50 * level;
            break;
        case E_PET::D_2_HEDGEHOG:
            nEffect = 0;
            nEffect = 0.1 * level;
            break;
        case E_PET::D_3_GATEKEEPER:
            nEffect = 5;
            nEffect += 0.2 * (level-1);
            if ( level >= getPetLevelMax(ePet, 1) )
            {
                nEffect = 9;
            }
            break;
        case E_PET::D_4_FLASH:
            nEffect = 0;
            if ( level > 50)
            {
                nEffect = 1 * 50;
                nEffect += 0.4 * (level-50);
            }
            else
            {
                nEffect = 1 * level;
            }
            break;
            
        default:
            break;
    }

    return nEffect;
}

#pragma mark - get, set : pet level
int PetNewDevilManager::getPetLevel(E_PET ePet)
{
    int idx = (int)ePet - C_COUNT_PET;
    return _uPetLevelDevil[idx].valueInt();
}

int PetNewDevilManager::getPetLevelMax(E_PET ePet, int grade)
{
    int level = 0;
    
    if ( ePet == E_PET::D_3_GATEKEEPER )
    {
        level = 20;
    }
    else
    {
        if(grade==1)        level = 20;
        else if(grade==2)   level = 40;
        else if(grade==3)   level = 60;
        else if(grade==4)   level = 100;
        else if(grade==5)   level = 150;
    }
    
    return level;
}

void PetNewDevilManager::setPetLevel(E_PET ePet, int level)
{
    int idx = (int)ePet - C_COUNT_PET;
    _uPetLevelDevil[idx] = level;
}

bool PetNewDevilManager::getPet(E_PET ePet)
{
    bool bResult = false;
    if ( getPetLevel(ePet) > 0 )
    {
        bResult = true;
    }
    
    return bResult;
}

int PetNewDevilManager::getPetAcquireFloor(E_PET ePet)
{
    int nResult = 0;
    switch (ePet) {
        case E_PET::D_1_PENGUIN:        nResult = 0;            break;
        case E_PET::D_2_HEDGEHOG:       nResult = 0;            break;
        case E_PET::D_3_GATEKEEPER:     nResult = 0;            break;
        case E_PET::D_4_FLASH:          nResult = 0;            break;
            
        default:
            break;
    }
    
    return nResult;
}

int PetNewDevilManager::getPetUpgradePrice(E_PET ePet, int level, int grade)
{
    int nGem = 0;
    
    switch (grade) {
        case 1: nGem = 300;     break;
        case 2: nGem = 500;     break;
        case 3: nGem = 700;     break;
        case 4: nGem = 1000;    break;
        case 5: nGem = 2000;    break;
            
        default:
            break;
    }
    
    return nGem;
}

#pragma mark - get, set : pet grade
int PetNewDevilManager::getPetGrade(E_PET ePet)
{
    int idx = (int)ePet - C_COUNT_PET;
    return _uPetGradeDevil[idx].valueInt();
}

void PetNewDevilManager::setPetGrade(E_PET ePet, int grade)
{
    int idx = (int)ePet - C_COUNT_PET;
    _uPetGradeDevil[idx] = grade;
}

int PetNewDevilManager::getPetGradeUpgradePrice(E_PET ePet, int grade)
{
    return 200;
}

#pragma mark - event
int PetNewDevilManager::onPetLevelUP(E_PET ePet)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     3 : 최대 레벨
     */
    int grade = getPetGrade(ePet);
    
    int level = getPetLevel(ePet);
    int levelMax = getPetLevelMax(ePet, grade);
    bool bLevelMax = level >= levelMax ? true : false;
    
    if ( bLevelMax == true )
    {
        return 3;
    }
    
    auto itemNow = ItemsMoneyManager::getInstance()->getGem();
    auto itemNeed = getPetUpgradePrice(ePet, level, grade);
    if ( itemNow < itemNeed )
    {
        return 2;
    }
    
    //
    setPetLevel(ePet, level + 1);
    ItemsMoneyManager::getInstance()->setGem(-itemNeed);

    //
    saveData();
    
    return 0;
}
 

#pragma mark - network

