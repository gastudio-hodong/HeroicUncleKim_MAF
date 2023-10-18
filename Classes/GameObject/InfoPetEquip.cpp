//
//  InfoPetEquip.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoPetEquip.h"

USING_NS_CC;

#pragma mark - InfoPetEquip
InfoPetEquip::InfoPetEquip() :
_nIdx(0),
_ePet(E_PET::PET_NONE),

_nSlot(0),
_nLevel(0),
_nAmount(0),

_nProperty_1(0),
_nProperty_2(0),
_nIncrease(0),
_eEffecttype(E_PET_EQUIP_TYPE::NONE),

_strName(""),
_strStory(""),
_strDesc("")
{
    
}

InfoPetEquip::~InfoPetEquip()
{
    
}

bool InfoPetEquip::init()
{
    return true;
}

int InfoPetEquip::getIdx()
{
    return _nIdx;
}

void InfoPetEquip::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

E_PET InfoPetEquip::getType()
{
    return _ePet;
}
void InfoPetEquip::setType(E_PET eType)
{
    _ePet = eType;
}

int InfoPetEquip::getSlot()
{
    return _nSlot;
}
void InfoPetEquip::setSlot(int nSlot)
{
    _nSlot = nSlot;
}

int InfoPetEquip::getLevel()
{
    return _nLevel;
}
void InfoPetEquip::setLevel(int nLevel)
{
    _nLevel = nLevel;
}

int InfoPetEquip::getAmount()
{
    return _nAmount;
}
void InfoPetEquip::setAmount(int nAmount)
{
    _nAmount = nAmount;
}

double InfoPetEquip::getProperty1()
{
    return _nProperty_1;
}
double InfoPetEquip::getProperty2()
{
    return _nProperty_2;
}
void InfoPetEquip::setProperty1(double nProperty)
{
    _nProperty_1 = nProperty;
}
void InfoPetEquip::setProperty2(double nProperty)
{
    _nProperty_2 = nProperty;
}

double InfoPetEquip::getIncrease()
{
    return _nIncrease;
}
void InfoPetEquip::setIncrease(double nIncrease)
{
    _nIncrease = nIncrease;
}

E_PET_EQUIP_TYPE InfoPetEquip::getEffectType()
{
    return _eEffecttype;
}
void InfoPetEquip::setEffectType(E_PET_EQUIP_TYPE eType)
{
    _eEffecttype = eType;
}

std::string InfoPetEquip::getName()
{
    return GAME_TEXT(_strName);
}
void InfoPetEquip::setName(std::string strName)
{
    _strName = strName;
}

std::string InfoPetEquip::getDesc()
{
    return GAME_TEXT(_strDesc);
}
void InfoPetEquip::setDesc(std::string strDesc)
{
    _strDesc = strDesc;
}

std::string InfoPetEquip::getStory()
{
    return GAME_TEXT(_strStory);
}
void InfoPetEquip::setStory(std::string strStory)
{
    _strStory = strStory;
}


#pragma mark - InfoPetEquipDraw
InfoPetEquipDraw::InfoPetEquipDraw() :
_nIdx(0),
_nCount(0)
{
    
}

InfoPetEquipDraw::~InfoPetEquipDraw()
{
    
}

bool InfoPetEquipDraw::init()
{
    return true;
}

int InfoPetEquipDraw::getIdx()
{
    return _nIdx;
}

void InfoPetEquipDraw::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int InfoPetEquipDraw::getCount()
{
    return _nCount;
}

void InfoPetEquipDraw::setCount(int nCount)
{
    _nCount = nCount;
}


#pragma mark - InfoPetEquipDrawGrade
InfoPetEquipDrawGrade::InfoPetEquipDrawGrade() :
_nIdx(0),
_nGrade(0)
{
    
}

InfoPetEquipDrawGrade::~InfoPetEquipDrawGrade()
{
    
}

bool InfoPetEquipDrawGrade::init()
{
    return true;
}

int InfoPetEquipDrawGrade::getIdx()
{
    return _nIdx;
}

void InfoPetEquipDrawGrade::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int InfoPetEquipDrawGrade::getGrade()
{
    return _nGrade;
}

void InfoPetEquipDrawGrade::setGrade(int nGrade)
{
    _nGrade = nGrade;
}

#pragma mark - InfoPetEquipAmount
InfoPetEquipAmount::InfoPetEquipAmount() :
_nIdx(0),
_nAmount(0)
{
    
}

InfoPetEquipAmount::~InfoPetEquipAmount()
{
    
}

bool InfoPetEquipAmount::init()
{
    return true;
}

int InfoPetEquipAmount::getIdx()
{
    return _nIdx;
}

void InfoPetEquipAmount::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int InfoPetEquipAmount::getAmount()
{
    return _nAmount;
}

void InfoPetEquipAmount::setAmount(int nGrade)
{
    _nAmount = nGrade;
}
