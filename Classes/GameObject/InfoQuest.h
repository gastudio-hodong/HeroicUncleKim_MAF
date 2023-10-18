//
//  InfoQuest.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoQuest_h
#define InfoQuest_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoQuest : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoQuest);
    
    InfoQuest();
    virtual ~InfoQuest();
    virtual bool init();
    
public:
    int getGroup();
    void setGroup(int value);
    
    int getIdx();
    void setIdx(int value);
    
    int getType();
    void setType(int value);
    
    std::string getTitle();
    void setTitleKey(std::string value);
    void setTitle(std::string value);
    
    std::string getDesc();
    void setDescKey(std::string value);
    void setDesc(std::string value);
    
    uint64_t getCountNow();
    void setCountNow(uint64_t value);
    
    uint64_t getCountMax();
    void setCountMax(uint64_t value);
    
    double getCountBigNow();
    void setCountBigNow(double value);
    
    double getCountBigMax();
    void setCountBigMax(double value);
    
    bool isCountShort();
    void setCountShort(bool value);
    
    bool isReceive();
    void setReceive(bool value);
    
    cocos2d::Vector<InfoItems*> getListReward();
    void setReward(std::string value);
    void addReward(int idx, int params, int count);
    
private:
    int  _group;
    int  _idx;
    int  _type;
    
    std::string _titleKey;
    std::string _title;
    std::string _descKey;
    std::string _desc;
    
    uint64_t  _nCurrentCount;
    uint64_t  _nCompleteCount;
    double _nCurrentBigCount;
    double _nCompleteBigCount;
    
    bool _bCountShort;
    bool _bReceive;
    
    cocos2d::Vector<InfoItems*> _listItems;
    
};
#endif /* InfoQuest_h */
