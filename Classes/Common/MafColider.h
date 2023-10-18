//
//  MafColider.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef MafColider_h
#define MafColider_h

#include "cocos2d.h"
class IMafColider
{
public:
    virtual cocos2d::Rect GetBoundingBox() = 0;
    virtual bool IsActivate() = 0;
    virtual void Collision(IMafColider* Other) final;
protected:
    virtual void CollisionBegin(IMafColider* Other);
    virtual void CollisionStay(IMafColider* Other);
    virtual void CollisionEnd(IMafColider* Other);
    
    virtual void CleanupColiderList() final;
    /*
     아이디어
     콜리전 엔터를 추가 해야함.
     Collision에서 처음 충돌 한 Other Colider의 경우 CollisionEnter를 호출하고 리스트에 담는다.
     그렇지 않을 경우 Collision 진행중을 호출한다.
     또한 Collision에서 호출될 때 충돌되었던 리스트를 우선적으로 검사하여 현재 IsActivate가 false인 경우 리스트에서 뺀다.
    */
protected:
    std::vector<IMafColider*> _listCollisionColider;
};


#endif /* MafColider_h */
