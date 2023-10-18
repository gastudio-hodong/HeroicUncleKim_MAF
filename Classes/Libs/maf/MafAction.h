//
//  MafAction.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 10. 5..
//
//

#ifndef __HexagonSnake__MafAction__
#define __HexagonSnake__MafAction__

#include "cocos2d.h"

USING_NS_CC;

    class MafAction
    {
    public:
        enum ACT_STYLE
        {
            TAG_POPUP_DEFAULT=1,
            TAG_POPUP_BIT=2,
        };

        static void ActionPopup(Node* target,ACT_STYLE style);
    };



#endif /* defined(__HexagonSnake__MafAction__) */
