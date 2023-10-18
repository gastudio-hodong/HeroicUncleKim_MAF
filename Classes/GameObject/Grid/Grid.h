//
//  Grid.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#ifndef Grid_hpp
#define Grid_hpp

#include "Common/ConfigDefault.h"

class InfoGridTileData;

class Grid : public Node
{
public:
    static Grid* create();
    // Sets default values for this actor's properties
    Grid();
    virtual ~Grid();

protected:
    // Called when the game starts or when spawned
    virtual bool init() override;
    virtual void onEnter() override;
public:
    // Called every frame
    virtual void update(float dt) override;

    void spawnGrid(const Vec2 centerLocation, const Vec2 tileSize, const IntPoint tileCount, bool isCriteriaLeftBottom = false);

    void addGridTile(InfoGridTileData* data);
    
    void removeGridTile(const IntPoint& index);

    void destroyGrid();
    void calculateCenterAndBottomLeft();
    void calculateCenterAndTopRight();

    Vec2 getTileLocationFromGridIndex(const IntPoint index, bool isCriteriaLeftBottom = false) const;
    bool isIndexValid(const IntPoint index) const;
    
    void showTileVisual();
public:
    
    inline Vec2 getGridCenterLocation() const
    {
        return _gridCenterLocation;
    }

    inline Vec2 getGridTileSize() const
    {
        return _gridTileSize;
    }

    inline IntPoint getGridTileCount() const
    {
        return _gridTileCount;
    }

    inline Vec2 getGridCriteriaCornerLocation() const
    {
        return _gridCriteriaCornerLocation;
    }

    inline std::map<IntPoint, InfoGridTileData*>& getGridTiles()
    {
        return _gridTiles;
    }
    
    inline InfoGridTileData* getGridTileData(const IntPoint index)
    {
        InfoGridTileData* Result = nullptr;
        if(isContainsTileData(index))
            Result = _gridTiles.find(index)->second;
        return Result;
    }
    
    inline bool isContainsTileData(const IntPoint index) const
    {
        return _gridTiles.contains(index);
    }
    
protected://Valueables
    int _activateTileNum;
    
    Vec2 _gridCenterLocation;
    Vec2 _gridTileSize;
    IntPoint _gridTileCount;
    Vec2 _gridCriteriaCornerLocation;

    std::map<IntPoint, InfoGridTileData*> _gridTiles;
public:
    //event
    MafDelegate<void(IntPoint)> _onTileDataUpdated_;
    MafDelegate<void()> _onGridDestroyed_;
    MafDelegate<void()> _onGridGenerated_;
};
#endif /* Grid_hpp */
