//
//  InfoReviewCostume.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoReviewCostume.h"

USING_NS_CC;

InfoReviewCostume::InfoReviewCostume() :
_isBest(false),
_nIdx(0),
_strUserId(""),
_strContents(""),
_nRating(0),
_nCountRecommend(0)
{
    
}

InfoReviewCostume::~InfoReviewCostume()
{
    
}

bool InfoReviewCostume::init()
{
    return true;
}

bool InfoReviewCostume::getBest()
{
    return _isBest;
}

void InfoReviewCostume::setBest(bool data)
{
    _isBest = data;
}

int InfoReviewCostume::getIdx()
{
    return _nIdx;
}

void InfoReviewCostume::setIdx(int data)
{
    _nIdx= data;
}

std::string InfoReviewCostume::getUserId()
{
    return _strUserId;
}

void InfoReviewCostume::setUserId(std::string str)
{
    _strUserId = str;
}

std::string InfoReviewCostume::getContents()
{
    return _strContents;
}

void InfoReviewCostume::setContents(std::string str)
{
    _strContents = str;
}

int InfoReviewCostume::getRating()
{
    return _nRating;
}

void InfoReviewCostume::setRating(int data)
{
    _nRating = data;
}

int InfoReviewCostume::getCountRecommend()
{
    return _nCountRecommend;
}

void InfoReviewCostume::setCountRecommend(int data)
{
    _nCountRecommend = data;
}
