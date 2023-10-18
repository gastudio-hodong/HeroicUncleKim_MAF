//
//  QuestManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef QuestManager_h
#define QuestManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class QuestManager : public MafSingleton<QuestManager>
{
public:
    QuestManager(void);
    virtual ~QuestManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    
    // info
    
    // get, set :
    std::string getImagePath(int idx);
    
    // get, set : level
    int getLevel(int idx);
    int getLevelMax();
    void setLevel(int level);
    
    // event
    int onReset(int floor);
    int onQuestBuy(int idx, int levelPlus);
    
    // network
    
protected:
    // network
    
    
private:
    //
    E_PLACE _ePlace;
    
    //
    
    
    //
    
    // callback 
};


#endif /* QuestManager_hpp */
