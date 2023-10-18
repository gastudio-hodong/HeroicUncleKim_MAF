//
//  InfoAdventureFurniture.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#ifndef InfoAdventureFurniture_hpp
#define InfoAdventureFurniture_hpp

#include "GameObject/Adventure/Base/InfoAdventureLocalUpgradeBase.h"

class InfoAdventureFurniture : public InfoAdventureLocalUpgradeBase
{
public:
    CREATE_FUNC(InfoAdventureFurniture);
    InfoAdventureFurniture();
    virtual ~InfoAdventureFurniture();
    virtual bool init() override;
    
public:
    const int getImageGrade() const override;
    const int getLevelupCondition(const int value) const;
    void setLevelupCondition(const std::vector<int> value);
    
private:
    std::vector<int> _listCondition;
};

#endif /* InfoAdventureFurniture_hpp */
