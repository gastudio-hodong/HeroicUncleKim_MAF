//
//  InfoMail.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoMail_h
#define InfoMail_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoMail : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoMail);
    
    InfoMail();
    virtual ~InfoMail();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int value);
    
    std::string getTitle();
    void setTitle(std::string value);
    
    int64_t getDeleteTime();
    void setDeleteTime(int64_t value);
    
    cocos2d::Vector<InfoItems*> getListItems();
    void setItems(std::string value);
    
private:
    int  _idx;
    
    std::string _title;
    int64_t _deleteTime;

    cocos2d::Vector<InfoItems*> _listItems;
    
};
#endif /* InfoMail_h */
