//
//  InfoDrawRate.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#ifndef InfoDrawRate_hpp
#define InfoDrawRate_hpp

#include "Common/ConfigDefault.h"

class InfoDrawRate : public cocos2d::Ref
{
public:
    
    InfoDrawRate();
    virtual ~InfoDrawRate();
    virtual bool init();
public:
    const int getType() const;
    void setType(const int value);
    
    const int getLevel() const;
    void setLevel(const int value);
    
    const std::vector<std::pair<int,int>> getListRate() const;
    void setListRate(const std::vector<std::pair<int,int>> value);
    
private:
    int _nLevel;
    int _nType;
    std::vector<std::pair<int,int>> _listRate;
};

#endif /* InfoDrawRate_hpp */
