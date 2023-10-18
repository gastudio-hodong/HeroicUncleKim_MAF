//
//  InfoAdventureMember.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#ifndef InfoAdventureMember_hpp
#define InfoAdventureMember_hpp

#include "GameObject/Adventure/Base/InfoAdventureLocalUpgradeBase.h"

class InfoAdventureMember : public InfoAdventureLocalUpgradeBase
{
public:
    CREATE_FUNC(InfoAdventureMember);
    InfoAdventureMember();
    virtual ~InfoAdventureMember();
    virtual bool init() override;
    
public:
    const int getParty() const;
    void setParty(const int value);
    
    const int getImageIdx() const;
    void setImageIdx(const int value);
    
    const int getRequireLv() const;
    void setRequireLv(const int value);
    
    const int getImageGrade() const override;
    
    const bool isOpen() const;
private:
    int _nParty;
    int _nImageIdx;
    int _nRequireLv;
};
#endif /* InfoAdventureMember_hpp */
