//
//  InfoItems.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoItems_h
#define InfoItems_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems : public cocos2d::Ref, public cocos2d::Clonable
{
public:
    static InfoItems* create(E_ITEMS eType);
    static InfoItems* create(int nIdx);
    
    
    InfoItems();
    virtual ~InfoItems();
    virtual bool init(int nIdx);
    
    // Overrides Clonable
    virtual InfoItems *clone() const override;
    
public:
    
    int getIdx();
    void setIdx(int nIdx);
    
    std::string getText();
    void setTextKey(std::string strKey);
    
    std::string getComment();
    void setCommentKey(std::string strKey);
    
    std::string getPath();
    void setPath(std::string strPath);
    
    std::string getType();
    void setType(std::string strType);
    
    std::string getTypeDetail();
    void setTypeDetail(std::string strTypeDetail);
    
    std::string getTypeName();
    void setTypeName(std::string strTypeName);
    
    int getTypeParams();
    void setTypeParams(int nTypeParams);
    
    std::string getReward();
    void setReward(std::string strReward);
    
    std::string getCount();
    void setCount(int nCount);
    void setCount(std::string strCount);
    
    cocos2d::Color3B getColor();
    
    bool isString();
    void setString(bool bString);
    
    bool isServer();
    void setServer(bool bServer);
    
    bool isVersion();
    void setVersion(std::string strVersion);
    
private:
    int _nIdx;
    
    std::string _strTextKey;
    std::string _strCommentKey;
    std::string _strPath;
    
    std::string _strType;
    std::string _strTypeDetail;
    std::string _strTypeName;
    int _nTypeParams;
    
    std::string _strReward;
    
    std::string _strCount;
    
    bool _bString;
    bool _bServer;
    
    std::string _strVersion;
};

#endif /* InfoItems_h */
