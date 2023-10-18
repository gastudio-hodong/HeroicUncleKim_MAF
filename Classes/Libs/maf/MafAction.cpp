//
//  MafAction.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 10. 5..
//
//

#include "MafAction.h"

static Sequence *seqPopupDefault = nullptr;
static Sequence *seqPopupBit = nullptr;

void MafAction::ActionPopup(Node* target,ACT_STYLE style){
    
    if(style == ACT_STYLE::TAG_POPUP_DEFAULT){
        if(seqPopupDefault == nullptr){
            target->setScale(0);
            ScaleTo *scale = ScaleTo::create(0.1f, 1.0f);
            seqPopupDefault = Sequence::create(scale,nullptr);
            seqPopupDefault->retain();
        }
        target->runAction(seqPopupDefault->clone());
    }
    else if(style == ACT_STYLE::TAG_POPUP_BIT){
        if(seqPopupBit == nullptr){
            target->setScale(0);
            ScaleTo *scale = ScaleTo::create(0.03f, 1.2f);
            ScaleTo *scale1 = ScaleTo::create(0.05f, 0.85f);
            ScaleTo *scaleOri = ScaleTo::create(0.07f, 1.0f);
            seqPopupBit = Sequence::create(scale,scale1,scaleOri, nullptr);
            seqPopupBit->retain();
        }
        target->runAction(seqPopupBit->clone());
    }
    
}