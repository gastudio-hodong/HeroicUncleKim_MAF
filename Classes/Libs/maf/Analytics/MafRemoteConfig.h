//
//  MafAnalytics.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 16..
//
//

#ifndef __HexagonSnake__MafRemoteConfig__
#define __HexagonSnake__MafRemoteConfig__

#include "cocos2d.h"

class MafRemoteConfig
{
private :
    static MafRemoteConfig* getInstance();

public:
    static void InitRemoteConfigAndLoadDefaultData(std::string file);
    static void LoadRemoteConfigData(std::string file);
    
    //
    static bool isRemoteConfigValue(std::string key);
        
    //
    static std::string GetRemoteValueString(std::string key);
    static int GetRemoteValueInt(std::string key);
    static bool GetRemoteValueBool(std::string key);

public:
    std::map<std::string, std::string> _data;
    
};

#endif /* defined(__HexagonSnake__MafAnalytics__) */
