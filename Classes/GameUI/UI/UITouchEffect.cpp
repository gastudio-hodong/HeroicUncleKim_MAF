//
//  UITouchEffect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UITouchEffect.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"

USING_NS_CC;

UITouchEffect* UITouchEffect::create()
{
    UITouchEffect* pRet = new(std::nothrow) UITouchEffect();
    if ( pRet && pRet->init() )
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

UITouchEffect::UITouchEffect(void) :
_layerContainerContent(nullptr)
{
    
}

UITouchEffect::~UITouchEffect(void)
{
    
}

bool UITouchEffect::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UITouchEffect::initVar()
{
    //
    
}

void UITouchEffect::initUi()
{
    //
    _layerContainerContent = Layer::create();
    _layerContainerContent->setIgnoreAnchorPointForPosition(false);
    _layerContainerContent->setAnchorPoint( Vec2::ZERO );
    addChild(_layerContainerContent);
    
    //
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        
        
        auto spriteTemp = Sprite::create("Assets/ui_common/ef_touch_01.png");
        spriteTemp->setPosition(touch->getLocation());
        _layerContainerContent->addChild(spriteTemp);

        auto aniframeRun = Animation::create();
        aniframeRun->setDelayPerUnit(0.1f);
        for ( int i = 1; i <= 4; i++ )
        {
            aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui_common/ef_touch_%02d.png", i));
        }
        
        
        auto aniRun = Animate::create(aniframeRun);
        auto seq = Sequence::create(aniRun, RemoveSelf::create(), nullptr);
        spriteTemp->runAction(seq);
            
        
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _layerContainerContent);
    
    
}
