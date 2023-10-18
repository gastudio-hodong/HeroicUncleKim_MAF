//
//  InfoSpecialMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/29.
//

#ifndef InfoSpecialMission_hpp
#define InfoSpecialMission_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoSpecialMission : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoSpecialMission);
    InfoSpecialMission();
    virtual ~InfoSpecialMission();
    virtual bool init();
    
    const bool isServerReward()const ;
    void setIsServerReward(const bool value);
    
    const int getIdx()const ;
    void setIdx(const int value);
    
    const std::string getContent()const ;
    void setContent(const std::string value);
    
    const E_SHORTCUTS getShortcutType()const ;
    void setShortcutType(const E_SHORTCUTS value);
    
    cocos2d::Vector<InfoItems*> getListMissionReward() const;
    void setListMissionReward(const cocos2d::Vector<InfoItems*> list);
    
private:
    bool _isServerReward;
    int _idx;
    std::string _content;
    E_SHORTCUTS _shortcuts;
    cocos2d::Vector<InfoItems*> _listMissionReward;
};
#endif /* InfoSpecialMission_hpp */
