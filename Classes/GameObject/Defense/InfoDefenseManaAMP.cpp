//
//  InfoDefenseManaAMP.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#include "InfoDefenseManaAMP.h"
#include "GameObject/Defense/InfoDefenseManaAmpStone.h"
#include "ManagerGame/DefenseManaAMPManager.h"

InfoDefenseManaAMP* InfoDefenseManaAMP::create()
{
    InfoDefenseManaAMP *pRet = new(std::nothrow) InfoDefenseManaAMP();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

InfoDefenseManaAMP::InfoDefenseManaAMP() :
_index(0),
_currentSuccessRate(0),
_cost(0),
_openFloor(0)
{
    _listStone.clear();
}

InfoDefenseManaAMP::~InfoDefenseManaAMP()
{
    _listStone.clear();
}

bool InfoDefenseManaAMP::init()
{
    return true;
}

int InfoDefenseManaAMP::getIndex() const
{
    return _index;
}

void InfoDefenseManaAMP::setIndex(const int value)
{
    _index = value;
}

int InfoDefenseManaAMP::getCurrentSuccessRate() const
{
    return _currentSuccessRate;
}

void InfoDefenseManaAMP::setCurrentSuccessRate(const int value)
{
    _currentSuccessRate = value;
}

int InfoDefenseManaAMP::getStoneIndexByOrder(const int pos)
{
    int result = 0;
    
    if(_listStone.size() >= pos)
        result = _listStone[pos-1];
    
    return result;
}
std::vector<int> InfoDefenseManaAMP::getStoneList()
{
    return _listStone;
}

void InfoDefenseManaAMP::setStoneList(std::vector<int> list)
{
    _listStone = list;
}
int InfoDefenseManaAMP::getStoneCount()
{
    return _listStone.size();
}
bool InfoDefenseManaAMP::isComplete()
{
    return _listStone.size() == 10;
}

int InfoDefenseManaAMP::getRedStoneCount() const
{
    int result = 0;
    
    for(auto stone : _listStone)
    {
        if(stone == (int)E_DF_AMP_STONE_Type::RED)
            result++;
    }
    return result;
}

int InfoDefenseManaAMP::getEquipCost()
{
    return _cost;
}

void InfoDefenseManaAMP::setEquipCOst(const int value)
{
    _cost = value;
}

int InfoDefenseManaAMP::getOpenFloor()
{
    return _openFloor;
}

void InfoDefenseManaAMP::setOpenFloor(const int value)
{
    _openFloor = value;
}

double InfoDefenseManaAMP::getManaAMPPower()
{
    double result = 0;
    
    if(_listStone.size() < 10)
        return result;
    
    std::vector<double> effectList;
    
    int iter = 0;
    for(auto stoneIndxe : _listStone)
    {
        if(iter + 1 > effectList.size())
            effectList.push_back(0);
        
        double tmp = effectList[iter];
        
        if(auto stone = DefenseManaAMPManager::getInstance()->getManaAmpStoneData(stoneIndxe))
        {
            switch(stone->getType())
            {
                case E_DF_AMP_STONE_Type::BLACK:
                {
                    iter++;
                }break;
                case E_DF_AMP_STONE_Type::RED:
                case E_DF_AMP_STONE_Type::YELLO:
                {
                    if(tmp == 0)
                        tmp = 1;
                    tmp *= stone->getOutputQuantity();
                    effectList[iter] = tmp;
                }break;
                default: break;
            }
        }
    }
    
    for(auto v : effectList)
    {
        result += v;
    }
    
    return result;
}

bool InfoDefenseManaAMP::isReddotCondition()
{
    bool result = UserDefault::getInstance()->getBoolForKey(MafUtils::format("df_ma_rd_comp_%d", _index).c_str(), false);
    
    return !result;
}
void InfoDefenseManaAMP::completeReddotCondition()
{
    UserDefault::getInstance()->setBoolForKey(MafUtils::format("df_ma_rd_comp_%d", _index).c_str(), true);
}
