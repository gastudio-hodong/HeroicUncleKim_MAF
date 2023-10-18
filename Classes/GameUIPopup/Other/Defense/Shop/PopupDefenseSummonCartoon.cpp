//
//  PopupSummonCartoon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/23.
//

#include "PopupDefenseSummonCartoon.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupDefenseSummonCartoon* PopupDefenseSummonCartoon::create(bool isSpecial, bool isSumonOnGoing)
{
    PopupDefenseSummonCartoon* pRet = new(std::nothrow) PopupDefenseSummonCartoon();
    if ( pRet && pRet->init(isSpecial, isSumonOnGoing) )
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

PopupDefenseSummonCartoon::PopupDefenseSummonCartoon(void):
_isFake(false)
,_isSpecial(false)
,_isSumonOnGoing(false)
,_currentCut(0)
,_lyCartoon(nullptr)
{
    _cutList.clear();
}

PopupDefenseSummonCartoon::~PopupDefenseSummonCartoon(void)
{
    _cutList.clear();
}

#pragma mark - init
bool PopupDefenseSummonCartoon::init(bool isSpecial, bool isSumonOnGoing)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _isSpecial = isSpecial;
    _isSumonOnGoing = isSumonOnGoing;
    if(_isSpecial)
    {
        int rand = random(1, 100);
        if(rand <= 30)
            _isFake = true;
    }
    if(_isSumonOnGoing)
        _isFake = false;
    
    initVar();
    initUI();
    
    return true;
}

void PopupDefenseSummonCartoon::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupDefenseSummonCartoon::initUI()
{
    _lyBG = LayerColor::create(Color4B::BLACK);
    _lyBG->setContentSize(getContentSize());
    _lyBG->setIgnoreAnchorPointForPosition(false);
    _lyBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->setPosition(getContentSize()/2);
    addChild(_lyBG);
    
    _lyCartoon = LayerColor::create();
    _lyCartoon->setContentSize(Size(getContentSize().width, 2070));
    _lyCartoon->setIgnoreAnchorPointForPosition(false);
    _lyCartoon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyCartoon->setPosition(getContentSize().width/2, _lyBG->getContentSize().height);
    _lyBG->addChild(_lyCartoon);
    
    
    uiMain();
    playCartoon();
}
#pragma mark - ui
void PopupDefenseSummonCartoon::uiMain()
{
    _lyCartoon->removeAllChildren();
    
    Vec2 location = Vec2(_lyCartoon->getContentSize().width/2, _lyCartoon->getContentSize().height - getContentSize().height/2);
    for(int i = 0; i < 3; ++i)
    {
        std::string path = "";
        switch(i+1)
        {
            case 1:
            {
                if(_isSpecial)
                {
                    if(_isFake)
                        path = "Assets/ui/defense/defense_random_a_1_01.png";
                    else
                        path = "Assets/ui/defense/defense_random_b_1_01.png";
                }
                else
                    path = "Assets/ui/defense/defense_random_a_1_01.png";
                    
            }break;
            case 2: path = "Assets/ui/defense/defense_random_2_01.png"; break;
            case 3:
            {
                if(_isSpecial)
                {
                    if(_isFake)
                        path = "Assets/ui/defense/defense_random_a_3_01.png";
                    else
                        path = "Assets/ui/defense/defense_random_b_3_01.png";
                }
                else
                    path = "Assets/ui/defense/defense_random_a_3_01.png";
            }break;
            default: break;
        }
        
        auto cut = Sprite::create(path);
        cut->setPosition(location);
        cut->getTexture()->setTexParameters(texParams);
        cut->setOpacity(80);
        cut->setScale(3);
        _lyCartoon->addChild(cut);
        
        _cutList.pushBack(cut);
        location += Vec2::DOWN * (cut->getContentSize().height * cut->getScale() + 20);
    }
}

#pragma mark - utils
void PopupDefenseSummonCartoon::playCartoon()
{
    if(_currentCut >= _cutList.size())
    {
        hide();
        return;
    }
    
    int count = 0;
    auto anim = Animation::create();
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = "";
        
        switch(_currentCut+1)
        {
            case 1:
            {
                if(_isSpecial)
                {
                    if(_isFake)
                        strPath = MafUtils::format("Assets/ui/defense/defense_random_a_%d_%02d.png", _currentCut + 1, i+1);
                    else
                        strPath = MafUtils::format("Assets/ui/defense/defense_random_b_%d_%02d.png", _currentCut + 1, i+1);
                }
                else
                    strPath = MafUtils::format("Assets/ui/defense/defense_random_a_%d_%02d.png", _currentCut + 1, i+1);
            }break;
                
            case 2: strPath = MafUtils::format("Assets/ui/defense/defense_random_2_%02d.png", i+1); break;
                
            case 3:
            {
                if(_isSpecial)
                    strPath = MafUtils::format("Assets/ui/defense/defense_random_b_%d_%02d.png", _currentCut + 1, i+1);
                else
                    strPath = MafUtils::format("Assets/ui/defense/defense_random_a_%d_%02d.png", _currentCut + 1, i+1);
            }break;
        }
        
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
        count++;
    }
    auto cut = _cutList.at(_currentCut);
    
    anim->setDelayPerUnit(0.1f);
    auto animate = Animate::create(anim);
    cut->runAction(Sequence::create(FadeTo::create(0.3f, 255), animate, FadeTo::create(0.3f, 80), nullptr));
    
    _currentCut++;
    if(_currentCut >= _cutList.size())
    {
        auto seq = Sequence::create(DelayTime::create(count * 0.1f + 0.3f),  CallFunc::create(CC_CALLBACK_0(PopupDefenseSummonCartoon::playCartoon, this)), nullptr);
        
        _lyCartoon->runAction(seq);
    }
    else
    {
        auto seq = Sequence::create(DelayTime::create(count * 0.08f + 0.3f), MoveBy::create(0.3f, Vec2::UP * 689), CallFunc::create(CC_CALLBACK_0(PopupDefenseSummonCartoon::playCartoon, this)), nullptr);
        
        _lyCartoon->runAction(seq);
    }

}
#pragma mark -Callback
