//
//  InfoProductBanner.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoProductBanner_h
#define InfoProductBanner_h

#include "Common/ConfigDefault.h"

class InfoProductBanner : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoProductBanner);
    
    InfoProductBanner();
    virtual ~InfoProductBanner();
    virtual bool init();
    
public:
    int getItemIdx();
    void setItemIdx(int nItemIdx);
    
    bool isReceive();
    void setReceive(bool bReceive);
    
    std::string getBGPath();
    void setBGPath(std::string strPath);
    
    std::string getBGLang();
    void setBGLang(std::string strLang);
    
private:
    int _nItemIdx;
    bool _bReceive;
    std::string _strBGPath;
    std::string _strBGLang;
    
    
};

#endif /* InfoProductBanner_h */
