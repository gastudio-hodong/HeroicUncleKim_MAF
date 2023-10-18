//
//  InfoScene.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoScene_h
#define InfoScene_h

#include "Common/ConfigDefault.h"

class InfoScene : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoScene);
    
    InfoScene();
    virtual ~InfoScene();
    virtual bool init();
    
public:
    void setParams(int nParams);
    int getParams();
    
    void setParamsString(std::string strParams);
    std::string getParamsString();
    
private:
    int _nParams;
    std::string _strParams;
    
};
#endif /* InfoScene_h */
