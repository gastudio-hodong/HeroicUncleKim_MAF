//
//  MafColider.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/13.
//

#include "MafColider.h"


void IMafColider::Collision(IMafColider* Other)
{
    CleanupColiderList();
    for(auto colider : _listCollisionColider)
    {
        if(colider == Other)
        {
            CollisionStay(Other);
            return;
        }
    }
    
    CollisionBegin(Other);
    CollisionStay(Other);
}

void IMafColider::CollisionBegin(IMafColider* Other)
{
    _listCollisionColider.push_back(Other);
}
void IMafColider::CollisionStay(IMafColider* Other)
{
    
}
void IMafColider::CollisionEnd(IMafColider* Other)
{
    
}

void IMafColider::CleanupColiderList()
{
    for(auto iter = _listCollisionColider.begin(); iter != _listCollisionColider.end();)
    {
        auto otherColider = (*iter);
        
        if(!(*iter)->IsActivate())
        {
            _listCollisionColider.erase(iter);
            continue;
        }
        
        auto otherNode = dynamic_cast <cocos2d::Node*>(otherColider);
        auto thisNode = dynamic_cast <cocos2d::Node*>(this);
        
        if(otherNode == nullptr || thisNode == nullptr)
        {
            iter++;
            continue;
        }
        
        if(thisNode->getPosition().getDistance(otherNode->getPosition()) > GetBoundingBox().size.width + otherColider->GetBoundingBox().size.width)
        {
            CollisionEnd(otherColider);
            _listCollisionColider.erase(iter);
            continue;
        }
        
        iter++;
    }
}
