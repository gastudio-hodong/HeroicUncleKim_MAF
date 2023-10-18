//
//  InfoDefenseMap.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#ifndef InfoDefenseMap_hpp
#define InfoDefenseMap_hpp


#include "Common/ConfigDefault.h"

class InfoDefenseMap : public cocos2d::Ref
{
public:
    static InfoDefenseMap* create();
    
    InfoDefenseMap();
    virtual ~InfoDefenseMap();
    virtual bool init();
    
public:
    int getIndex() const;
    void setIndex(const int index);
    
    std::vector<IntPoint> getPath() const;
    void setPath(const std::vector<IntPoint> path);
    
    std::string getImageFilePath();
    void setImageFilePath(std::string path);
    
private:
    int _index;
    std::vector<IntPoint> _path;
    std::string _imgFilePath;
};


#endif /* InfoDefenseMap_hpp */
