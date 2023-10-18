//
//  InfoGridTileData.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#ifndef InfoGridTileData_hpp
#define InfoGridTileData_hpp
#include "Common/ConfigDefault.h"

class InfoGridTileData : public cocos2d::Ref
{
public:
    static InfoGridTileData* create();
    
    InfoGridTileData();
    virtual ~InfoGridTileData();
    virtual bool init();
    
public:
    IntPoint getIndex() const;
    void setIndex(const IntPoint index);
    
    Vec2 getLocation() const;
    void setLocation(const Vec2 location);
    
    void resetData();
private:
    IntPoint _index;
    Vec2 _location;
};

#endif /* InfoGridTileData_hpp */
