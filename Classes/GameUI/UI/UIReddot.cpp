//
//  UIReddot.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/09.
//

#include "UIReddot.h"

USING_NS_CC;

UIReddot* UIReddot::create(E_REDDOT_TYPE type, Vec2 anchorPoint)
{
    UIReddot* pRet = new(std::nothrow) UIReddot(type);
    if ( pRet && pRet->init(anchorPoint) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

UIReddot::UIReddot(E_REDDOT_TYPE type):
_eType(type),
_sprReddot(nullptr)
{
}

UIReddot::~UIReddot()
{
}

bool UIReddot::init(Vec2 anchorPoint)
{
    
    setName("UIReddot");
    setTag(1000);
    
    switch (_eType) {
        case E_REDDOT_TYPE::NOMAL:
        {
            _sprReddot = Sprite::create("Assets/ui_common/icon_red_1_1.png");
            _sprReddot->setAnchorPoint(anchorPoint);
            addChild(_sprReddot);
            {
                auto spriteSpotLight = Sprite::create("Assets/ui_common/icon_red_1_2.png");
                spriteSpotLight->setPosition(_sprReddot->getContentSize()/2);
                _sprReddot->addChild(spriteSpotLight);

                auto fideOut = FadeOut::create(0.5f);
                auto fideIn = FadeIn::create(0.5f);
                Vector<FiniteTimeAction*> actions;
                actions.pushBack(fideOut);
                actions.pushBack(fideIn);
                auto sequence = Sequence::create(actions);
                auto rep = RepeatForever::create(sequence);
                spriteSpotLight->runAction(rep);
            }
        }
            break;
        case E_REDDOT_TYPE::EVENT:
        {
            _sprReddot = Sprite::create("Assets/ui/pet/event1_1.png");
            _sprReddot->setAnchorPoint(anchorPoint);
            addChild(_sprReddot);
            {
                auto aniframeRun = Animation::create();
                aniframeRun->setDelayPerUnit(0.1f);
                for ( int i = 1; i <= 5; i++ )
                {
                    aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/pet/event1_%d.png", i));
                }
                aniframeRun->addSpriteFrameWithFile("Assets/ui/pet/event1_1.png");
                
                auto action1 = Animate::create(aniframeRun);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                _sprReddot->runAction(RepeatForever::create(seq));
            }
        }
            break;
        default:
            return false;
    }
    
    return true;
}

void UIReddot::hide()
{
    if(_sprReddot != nullptr)
        _sprReddot->removeFromParent();
    
    removeFromParent();
}
