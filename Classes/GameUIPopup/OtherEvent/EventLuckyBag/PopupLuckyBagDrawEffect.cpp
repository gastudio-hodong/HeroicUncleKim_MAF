//
//  PopupLuckyBagDrawEffect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/01/03.
//
#include "PopupLuckyBagDrawEffect.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventLuckyBagManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DialogManager.h"

USING_NS_CC;

PopupLuckyBagDrawEffect* PopupLuckyBagDrawEffect::create(E_TYPE type, std::string strReward)
{
    PopupLuckyBagDrawEffect* pRet = new(std::nothrow) PopupLuckyBagDrawEffect();
    if ( pRet && pRet->init(type, strReward) )
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

PopupLuckyBagDrawEffect::PopupLuckyBagDrawEffect():
_eType(E_TYPE::TRY)
,_strReward("")
,_bInitUI(false)
,_uiMain(nullptr)
{
}

PopupLuckyBagDrawEffect::~PopupLuckyBagDrawEffect(void)
{
}
bool PopupLuckyBagDrawEffect::init(E_TYPE type, std::string strReward)
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    _eType = type;
    _strReward = strReward;
    _bHideBackKey = false;
    initVar();
    initUI();
    
    return true;
}

void PopupLuckyBagDrawEffect::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        
        switch(_eType)
        {
            case E_TYPE::TRY:
                uiTry();
                break;
            case E_TYPE::FAIL:
                uiFail();
                break;
            case E_TYPE::ENDING:
                uiEnding();
                break;
        }
        //
    }
}

#pragma mark - ui
void PopupLuckyBagDrawEffect::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupLuckyBagDrawEffect::initUI()
{
    auto layout = ui::Layout::create();
    layout->setContentSize(getContentSize());
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(getContentSize().width / 2, 0));
    layout->setName("PARENT_LAYOUT");
    addChild(layout);
    
    _uiMain = ui::Layout::create();
    _uiMain->setContentSize(layout->getContentSize());
    _uiMain->setIgnoreAnchorPointForPosition(false);
    _uiMain->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _uiMain->setPosition(Vec2(layout->getContentSize()/2));
    _uiMain->setClippingEnabled(false);
    layout->addChild(_uiMain);
}
void PopupLuckyBagDrawEffect::uiTry()
{
    float moveSpeed = 0.25f;
    
    auto uiBong = ui::ImageView::create("Assets/ui/event/event_8th_bg_2_1.png");
    uiBong->setScale(3);
    uiBong->setPosition(Vec2(_uiMain->getContentSize().width + uiBong->getContentSize().width/2 * uiBong->getScale(), _uiMain->getContentSize().height / 2));
    ((ui::Scale9Sprite*)uiBong->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    _uiMain->addChild(uiBong);
    
    auto action1_1 = MoveTo::create(moveSpeed, Vec2(_uiMain->getContentSize().width / 2, _uiMain->getContentSize().height / 2));
    auto action1_2 = EaseBackOut::create(action1_1);
    
    auto action1_3 = CallFunc::create([&, uiBong]{
        
        auto anim = Animation::create();
        anim->setDelayPerUnit(0.05f);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/event_8th_bg_2_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        auto animate = Animate::create(anim);
        ((ui::Scale9Sprite*)uiBong->getVirtualRenderer())->runAction(animate);
    });
    
    auto action1_4 = MoveTo::create(moveSpeed, Vec2(-uiBong->getContentSize().width / 2 * uiBong->getScale(), _uiMain->getContentSize().height/2));
    auto action1_5 = EaseIn::create(action1_4, moveSpeed);
    
    auto seq = Sequence::create(action1_2, action1_3, DelayTime::create(0.5f), action1_5, CallFunc::create([=](){
        
        int nCount = EventLuckyBagManager::getInstance()->getCountNow();
        EventLuckyBagManager::getInstance()->requestProceeLuckyBag(true, 1, nCount);
        
        hide();
    }), nullptr);
    uiBong->runAction(seq);
}

void PopupLuckyBagDrawEffect::uiFail()
{
    
    SoundManager::SoundEffectStart(SOUND_EFFECT_EVENT_LUCKYBAG_FAIL, E_SOUND_TYPE::CONTENTS);
    
    auto uiMamon = ui::Scale9Sprite::create("Assets/ui/event/event_8th_bg_3_1.png");
    uiMamon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiMamon->setScale(3);
    uiMamon->setPosition(_uiMain->getContentSize()/2);
    uiMamon->getTexture()->setTexParameters(texParams);
    _uiMain->addChild(uiMamon);
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for ( int i = 1; i <= 4; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/event_8th_bg_3_%d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    auto seq = Sequence::create(DelayTime::create(0.15f), Animate::create(anim), DelayTime::create(0.15f), Shake::create(0.3f, 10), DelayTime::create(0.3f), CallFunc::create([=](){
        
        Vec2 posToast = getContentSize()/2;
        
        PopupToast::show(GAME_TEXT("t_ui_event_enhance_draw_fail"), posToast.x, posToast.y);
        
        auto list = ItemsManager::getInstance()->getConvertArray(_strReward);
        PopupToastItems::show(list);
        hide();
    }), nullptr);
    uiMamon->runAction(seq);
}

void PopupLuckyBagDrawEffect::uiEnding()
{
    auto uiEnding = ui::Scale9Sprite::create("Assets/ui/event/event_8th_bg_4_1.png");
    uiEnding->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiEnding->setScale(3);
    uiEnding->setPosition(_uiMain->getContentSize()/2);
    uiEnding->getTexture()->setTexParameters(texParams);
    _uiMain->addChild(uiEnding);
    
    
    auto action1 = ScaleBy::create(4.f, 7);
    auto action2 = FadeOut::create(1.5f);
    auto actionSpawn = Spawn::create(action1, action2, nullptr);
    
    auto seq = Sequence::create(DelayTime::create(1.5), actionSpawn, CallFunc::create([=](){
        
        // 다음 다른 이벤트에 사용.
        // DialogManager::getInstance()->showDialog("EVENT_ENHANCE_DRAW_EP");
        hide();
    }), NULL);
    uiEnding->runAction(seq);
}
