//
//  InfoReviewCostume.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoReviewCostume_h
#define InfoReviewCostume_h

#include "Common/ConfigDefault.h"

class InfoReviewCostume : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoReviewCostume);
    
    InfoReviewCostume();
    virtual ~InfoReviewCostume();
    virtual bool init();
    
public:
    bool getBest();
    void setBest(bool data);
    
    int getIdx();
    void setIdx(int data);
    
    std::string getUserId();
    void setUserId(std::string str);
    
    std::string getContents();
    void setContents(std::string str);
    
    int getRating();
    void setRating(int data);
    
    int getCountRecommend();
    void setCountRecommend(int data);
    
    
private:
    bool _isBest;
    int _nIdx;
    
    std::string _strUserId;
    std::string _strContents;
    
    int _nRating;
    int _nCountRecommend;
    
    
};

#endif /* InfoReviewCostume_h */
