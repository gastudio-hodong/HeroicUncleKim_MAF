//
//  InfoEventOriginData.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/02.
//

#include "InfoEventOriginData.h"

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>

USING_NS_CC;

InfoEventOriginData::InfoEventOriginData() :
_nIdx(0)
,_nEventIdx(0)
,_nEventType(0)
,_nEventStartTime(0)
,_nEventEndTime(0)
,_strOtherData("")
{
    
}

InfoEventOriginData::~InfoEventOriginData()
{
    
}

bool InfoEventOriginData::init()
{
    return true;
}


const int InfoEventOriginData::getIdx()const
{
    return _nIdx;
}

void InfoEventOriginData::setIdx(const int value)
{
    _nIdx = value;
}

const int InfoEventOriginData::getEventIdx()const
{
    return _nEventIdx;
}

void InfoEventOriginData::setEventIdx(const int value)
{
    _nEventIdx = value;
}

const int InfoEventOriginData::getEventType()const
{
    return _nEventType;
}

void InfoEventOriginData::setEventType(const int value)
{
    _nEventType = value;
}

const int64_t InfoEventOriginData::getEventStartTime() const
{
    return _nEventStartTime;
}

void InfoEventOriginData::setEventStartTime(const int64_t value)
{
    _nEventStartTime = value;
}

void InfoEventOriginData::setEventStartTime(const std::string value)
{
    std::istringstream in(value);

    std::tm t{};
    t.tm_isdst = -1; // let std::mktime try to figure out if DST is in effect

    in >> std::get_time(&t, "%Y-%m-%d %H:%M:%S"); // extract it into a std::tm
    
    std::time_t timestamp = std::mktime(&t);   // get epoch

    timestamp += 9 * 60 * 60;                   // add 2 hours

    std::tm utc = *std::gmtime(&timestamp);    // make a UTC std::tm
//    std::tm lt = *std::localtime(&timestamp);  // make a localtime std::tm
    
    // print the results:
//    std::cout << std::put_time(&utc, "%Y-%m-%d %H:%M:%S") << '\n' << std::put_time(&lt, "%Y-%m-%d %H:%M:%S") << '\n';

    _nEventStartTime = mktime(&utc);
}

const int64_t InfoEventOriginData::getEventEndTime() const
{
    return _nEventEndTime;
}

void InfoEventOriginData::setEventEndTime(const int64_t value)
{
    _nEventEndTime = value;
}

void InfoEventOriginData::setEventEndTime(const std::string value)
{
    std::istringstream in(value);

    std::tm t{};
    t.tm_isdst = -1; // let std::mktime try to figure out if DST is in effect

    in >> std::get_time(&t, "%Y-%m-%d %H:%M:%S"); // extract it into a std::tm
    
    std::time_t timestamp = std::mktime(&t);   // get epoch

    timestamp += 9 * 60 * 60;                   // add 2 hours

    std::tm utc = *std::gmtime(&timestamp);    // make a UTC std::tm
//    std::tm lt = *std::localtime(&timestamp);  // make a localtime std::tm
    
    // print the results:
//    std::cout << std::put_time(&utc, "%Y-%m-%d %H:%M:%S") << '\n' << std::put_time(&lt, "%Y-%m-%d %H:%M:%S") << '\n';

    _nEventEndTime = mktime(&utc);
}

const std::string InfoEventOriginData::getOtherData()const
{
    return _strOtherData;
}

void InfoEventOriginData::setOtherData(const std::string value)
{
    _strOtherData = value;
}
