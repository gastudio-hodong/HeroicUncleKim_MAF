//
//  InfoDefenseFloor.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/20.
//

#ifndef InfoDefenseFloor_hpp
#define InfoDefenseFloor_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseFloor : public cocos2d::Ref
{
public:
    static InfoDefenseFloor* create();
    
    InfoDefenseFloor();
    virtual ~InfoDefenseFloor();
    virtual bool init();
    
public:
    int getIndex() const;
    void setIndex(const int value);
    
    int getMapNumber() const;
    void setMapNumber(const int value);
    
    int getStartFloor()const;
    void setStartFloor(const int value);
    
    int getEndFloor()const;
    void setEndFloor(const int value);
    
    double getMonsterHp()const;
    void setMonsterHp(const int value);
    
    int getMonsterValue()const;
    void setMonsterValue(const double value);
    
    int get10SecMonsterSpawnCount()const;
    void set10SecMonsterSpawnCount(const int value);
    
    int getNormalPer()const;
    void setNormalPer(const int value);
    
    int getRunnerIdx()const;
    void setRunnerIdx(const int value);
    
    int getRunnerPer()const;
    void setRunnerPer(const int value);
    
    int getTankerIdx()const;
    void setTankerIdx(const int value);
    
    int getTankerPer()const;
    void setTankerPer(const int value);
    
    int getBossIdx()const;
    void setBossIdx(const int value);
    
    std::string getRewardRepeat() const;
    void setRewardRepeat(const std::string value);
    
    std::string getRewardFail() const;
    void setRewardFail(const std::string value);
private:
    int _index;
    int _mapNumber;
    int _startFloor;
    int _endFloor;
    double _monsterHp;
    int _monsterValue;
    int _10SecMonsterSpawnCount;
    int _normalPer;
    int _runnerIdx;
    int _runnerPer;
    int _tankerIdx;
    int _tankerPer;
    int _bossIdx;
    std::string _rewardRepeat;
    std::string _rewardFail;
};

#endif /* InfoDefenseFloor_hpp */
