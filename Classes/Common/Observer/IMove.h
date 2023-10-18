//
//  IMove.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 05/06/2019.
//

#ifndef IMove_h
#define IMove_h

#include "Common/ConfigDefault.h"

class IMove
{
public:
    virtual void move(E_AREA_MOVE eType) = 0;
};

#endif /* IMove_h */
