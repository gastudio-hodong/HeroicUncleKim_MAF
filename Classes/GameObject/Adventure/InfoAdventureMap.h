/* InfoAdventureMap_hpp */
//
// InfoAdventureMap.hpp
// FantasyClicker-mobile
//
// Created by MAF_TG on 2022/04/25.
//

#ifndef InfoAdventureMap_hpp
#define InfoAdventureMap_hpp

#include "Common/ConfigDefault.h"

class InfoAdventureStage;
class InfoAdventureMap : public cocos2d::Ref
{
public:
  CREATE_FUNC(InfoAdventureMap);
  InfoAdventureMap();
  virtual ~InfoAdventureMap();
  virtual bool init();

public:
    const int getIdx() const;
    void setIdx(const int value);

    const cocos2d::Vector<InfoAdventureStage*>& getListStage() const;

    InfoAdventureStage* const getStage(const int nIdx) const;
    void setListStage(const cocos2d::Vector<InfoAdventureStage*> value);
    void pushBackStage(InfoAdventureStage* const value);
    void resetStage();
    const bool isOpen() const;
    const int getOpenCondition() const;
private:
  int _nIdx;

  cocos2d::Vector<InfoAdventureStage*> _listStage;
};
#endif /* InfoAdventureMap_hpp */
