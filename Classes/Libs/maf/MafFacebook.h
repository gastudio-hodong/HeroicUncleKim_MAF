//
//  MafFacebook.h
//  FantasyClicker
//
//  Created by SanHeo on 2015. 9. 9..
//
//

#ifndef __FantasyClicker__MafFacebook__
#define __FantasyClicker__MafFacebook__

#include "cocos2d.h"

class MafFacebook
{
public:
    
    MafFacebook();
    static MafFacebook* getInstance();
    
    static void FacebookInit();
    
    static void FacebookLog(const char *strEvent, const char* strParam, int value);
    static void FacebookLog(const char *strEvent, const char* strParam, const char* strValue);
    
};


#endif /* defined(__FantasyClicker__MafFacebook__) */
