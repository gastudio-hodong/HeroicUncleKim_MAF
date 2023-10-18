//
//  InfoDialogActor.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/20.
//

#ifndef InfoDialogActor_hpp
#define InfoDialogActor_hpp

#include "Common/ConfigDefault.h"

class InfoDialogActor : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoDialogActor);
    InfoDialogActor();
    virtual ~InfoDialogActor();
    virtual bool init();
    
public:
    const int getID() const;
    void setID(const int value);
    
    const std::string getImagePath() const;
    void setImagePath(const std::string value);
    
protected:
    int _nID;
    std::string _strImagePath;
};


#endif /* InfoDialogActor_hpp */
