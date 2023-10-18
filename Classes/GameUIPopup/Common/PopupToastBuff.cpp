//
//  PopupToastBuff.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupToastBuff.h"

USING_NS_CC;

int PopupToastBuff::_nCountVisible = 0;
PopupToastBuff* PopupToastBuff::create(E_TOAST_BUFF eType)
{
    PopupToastBuff* pRet = new(std::nothrow) PopupToastBuff();
    if ( pRet && pRet->init(eType) )
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

PopupToastBuff::PopupToastBuff(void) :
_eType(E_TOAST_BUFF::NONE)
{
    
}

PopupToastBuff::~PopupToastBuff(void)
{
    PopupToastBuff::_nCountVisible -= 1;
}

bool PopupToastBuff::init(E_TOAST_BUFF eType)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _eType = eType;
    
    // init
    initVar();
    initUi();
    
    return true;
}


void PopupToastBuff::initVar()
{
    
}

void PopupToastBuff::initUi()
{
    _layerContainerBottom = Layer::create();
    _layerContainerBottom->setContentSize( Size(getContentSize().width, 532) );
    _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
    _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBottom->setPosition(getContentSize().width / 2, getContentSize().height - 100);
    _layerContainerBottom->setCascadeOpacityEnabled(true);
    addChild(_layerContainerBottom);
    
    std::string strEffectMain = "";
    std::string strEffectSub = "";
    
    if ( _eType == E_TOAST_BUFF::BUFF_ATK )
    {
        strEffectMain = "Assets/ui/ads/buf_ef_3_1.png";
        strEffectSub = "Assets/ui/ads/buf_ef_3_%d.png";
    }
    else if ( _eType == E_TOAST_BUFF::BUFF_SPD )
    {
        strEffectMain = "Assets/ui/ads/buf_ef_1_1.png";
        strEffectSub = "Assets/ui/ads/buf_ef_1_%d.png";
    }
    else if ( _eType == E_TOAST_BUFF::BUFF_COIN )
    {
        strEffectMain = "Assets/ui/ads/buf_ef_2_1.png";
        strEffectSub = "Assets/ui/ads/buf_ef_2_%d.png";
    }
    else if ( _eType == E_TOAST_BUFF::BUFF_ALL )
    {
        strEffectMain = "Assets/ui/ads/buf_ef_4_1.png";
        strEffectSub = "Assets/ui/ads/buf_ef_4_%d.png";
    }
    
    if ( strEffectMain.length() == 0 || strEffectSub.length() == 0 )
    {
        auto seqParent = Sequence::create(DelayTime::create(0.1), RemoveSelf::create(), nullptr);
        runAction(seqParent);
        return;
    }
        
    auto spriteEffect = Sprite::create(strEffectMain);
    spriteEffect->setPosition(0, _layerContainerBottom->getContentSize().height / 2);
    _layerContainerBottom->addChild(spriteEffect);
    {
        // run
        auto aniframeRun = Animation::create();
        for ( int i = 1; i <= 4; i++ )
        {
            aniframeRun->addSpriteFrameWithFile(MafUtils::format(strEffectSub.c_str(), i));
        }
        aniframeRun->setDelayPerUnit(0.1f);
        
        //
        spriteEffect->runAction(RepeatForever::create(Animate::create(aniframeRun)));
     
        //
        auto action1_1 = MoveTo::create(0.5, Vec2(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height / 2));
        auto action1_2 = EaseOut::create(action1_1, 20.0);
        auto action1_3 = FadeOut::create(0.4);
        auto seq = Sequence::create(action1_2, action1_3, nullptr);
        spriteEffect->runAction(seq);
    }
    
    //
    auto seqParent = Sequence::create(DelayTime::create(0.9), RemoveSelf::create(), nullptr);
    runAction(seqParent);
}

#pragma mark - ui
#pragma mark - show, lock, hide
void PopupToastBuff::show(E_TOAST_BUFF eType)
{
    // notification
    auto node = UtilsScene::getInstance()->getRunningScene();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_SCENE_TOAST_BUFF) == nullptr )
    {
        PopupToastBuff::_nCountVisible = 1;
        
        auto popup = PopupToastBuff::create(eType);
        node->addChild(popup, ZORDER_SCENE_TOAST_BUFF, TAG_SCENE_TOAST_BUFF);
    }
    else
    {
        PopupToastBuff::_nCountVisible += 1;
        
        auto action1 = DelayTime::create((PopupToastBuff::_nCountVisible - 1) * 0.9);
        auto action2 = CallFunc::create([=](){
            auto popup = PopupToastBuff::create(eType);
            node->addChild(popup, ZORDER_SCENE_TOAST_BUFF, TAG_SCENE_TOAST_BUFF);
        });
        auto seq = Sequence::create(action1, action2, nullptr);
        node->runAction(seq);
    }
}

#pragma mark - scheduler
