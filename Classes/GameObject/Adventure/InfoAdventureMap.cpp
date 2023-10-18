//
// InfoAdventureMap.cpp
// FantasyClicker-mobile
//
// Created by MAF_TG on 2022/04/25.
//

#include "InfoAdventureMap.h"

#include "GameObject/Adventure/InfoAdventureStage.h"

USING_NS_CC;

#pragma mark -
InfoAdventureMap::InfoAdventureMap():
_nIdx(0)
{
  _listStage.clear();
}

InfoAdventureMap::~InfoAdventureMap()
{
  _listStage.clear();
}

bool InfoAdventureMap::init()
{
   
  return true;
}
const int InfoAdventureMap::getIdx() const
{
  return _nIdx;
}
void InfoAdventureMap::setIdx(const int value)
{
  _nIdx = value;
}

const cocos2d::Vector<InfoAdventureStage*>& InfoAdventureMap::getListStage() const
{
  return _listStage;
}
InfoAdventureStage* const InfoAdventureMap::getStage(const int nIdx) const
{
  InfoAdventureStage* result = nullptr;
   
   
  for(auto obj : _listStage)
  {
    if(obj->getIdx() == nIdx)
    {
      result = obj;
      break;
    }
  }
   
  return result;
}
void InfoAdventureMap::setListStage(const cocos2d::Vector<InfoAdventureStage*> value)
{
  _listStage = value;
}
void InfoAdventureMap::pushBackStage(InfoAdventureStage* const value)
{
  _listStage.pushBack(value);
}
void InfoAdventureMap::resetStage()
{
    for(const auto obj : _listStage)
    {
        obj->setEndTime(0);
        obj->setIsRunning(false);
        obj->setIsComplete(false);
    }
}
const bool InfoAdventureMap::isOpen() const
{
    auto objStage = getStage(1);
    
    if(_nIdx == 1)
        objStage = getStage(0);
   
    if(objStage == nullptr)
        return false;
   
    return objStage->isOpen();
}
const int InfoAdventureMap::getOpenCondition() const
{
    auto objStage = getStage(1);
    
    if(_nIdx == 1)
        objStage = getStage(0);
   
    if(objStage == nullptr)
        return false;
   
    return objStage->getRequireLv();
}
