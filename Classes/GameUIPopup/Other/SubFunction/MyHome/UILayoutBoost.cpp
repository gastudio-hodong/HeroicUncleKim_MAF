//
//  UILayoutBoost.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayoutBoost.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

UILayoutBoost* UILayoutBoost::create(cocos2d::Size size)
{
    UILayoutBoost* pRet = new(std::nothrow) UILayoutBoost();
    if ( pRet && pRet->init(size) )
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

UILayoutBoost::UILayoutBoost(void) :
_nMyhomeCount(0),

_layerContainer(nullptr)
{
    
}

UILayoutBoost::~UILayoutBoost(void)
{
    
}

bool UILayoutBoost::init(cocos2d::Size size)
{
    if ( !Layout::init() )
    {
        return false;
    }
   
    //
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(size);
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayoutBoost::initVar()
{
    
}

void UILayoutBoost::initUi()
{
    auto uiImgBG = ui::ImageView::create("Assets/ui_common/box_item.png");
    uiImgBG->setContentSize(getContentSize());
    uiImgBG->setScale9Enabled(true);
    uiImgBG->setCapInsets(Rect(5,30,10,10));
    uiImgBG->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
    addChild(uiImgBG);
    
    _layerContainer = ui::Layout::create();
    _layerContainer->setContentSize(getContentSize());
    _layerContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layerContainer->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
    addChild(_layerContainer);
    
    //
    drawContents();
}

#pragma mark - ui

#pragma mark - draw
void UILayoutBoost::drawContents()
{
    //
    _nMyhomeCount = 0;
    
    std::string strViewData = UserDefault::getInstance()->getStringForKey(KEY_MYHOME_COUNT, "");
    if ( !strViewData.empty() )
    {
        strViewData = MafAes256::Decrypt(strViewData);
        _nMyhomeCount = atoi(strViewData.c_str());
    }
    
    //
    int nLockCount = 0;
    for(int i = 1; i <= C_MAX_MYHOME; i++)
    {
        int adsWatch = UserInfoManager::getInstance()->getMyhomeWatchcount(i);
        int adsWatchMax = getAdsWatchMax(i);
        if ( adsWatch >= adsWatchMax )
        {
            nLockCount++;
        }
    }
    
    if ( nLockCount >= C_MAX_MYHOME )
    {
        drawLock();
    }
    else
    {
        drawPorgress();
    }
}

void UILayoutBoost::drawPorgress()
{
    //
    _layerContainer->removeAllChildren();
    
    auto layout = _layerContainer;
    
    auto size = layout->getContentSize();
    
    //
    auto spriteHomeIcon = Sprite::create("Assets/ui/myhome/homeboost_1.png");
    spriteHomeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    spriteHomeIcon->setPosition(0, size.height / 2);
    layout->addChild(spriteHomeIcon);

    //
    auto spriteInfoBG = Sprite::create("Assets/ui/myhome/homeboost_text_box.png");
    spriteInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteInfoBG->setPosition(size.width - 66, size.height / 2);
    layout->addChild(spriteInfoBG);
    
    auto spriteInfoIcon = Sprite::create("Assets/ui/myhome/btn_ad_item3.png");
    spriteInfoIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteInfoIcon->setPosition(size.width, size.height / 2);
    layout->addChild(spriteInfoIcon);
    {
        auto label = MafNode::createBmFont("+3min", GAME_BM_FONT_AL, 24);
        label->setPosition(spriteInfoBG->getContentSize().width / 2, spriteInfoBG->getContentSize().height / 2);
        label->setColor(Color3B(128,255,0));
        spriteInfoBG->addChild(label);
    }
    
    //
    auto spriteProgressBG = Sprite::create("Assets/ui/myhome/homeboost_Progress bar_1.png");
    spriteProgressBG->setPosition(size.width / 2, size.height / 2);
    layout->addChild(spriteProgressBG);
    {
        int nCountMax = 5;
        int nCount = _nMyhomeCount-1;
        double nPercent = (double)nCount / (nCountMax-1) * 100;
        if ( nPercent > 100 )
            nPercent = 100;
        
        auto progressFloor = ProgressTimer::create(Sprite::create("Assets/ui/myhome/homeboost_Progress bar_2.png"));
        progressFloor->setPosition(spriteProgressBG->getContentSize()/2);
        progressFloor->setType(ProgressTimer::Type::BAR);
        progressFloor->setBarChangeRate(Vec2(1,0));
        progressFloor->setMidpoint(Vec2(0, 0.5));
        progressFloor->setPercentage(nPercent);
        spriteProgressBG->addChild(progressFloor);
        
        for ( int i = 0; i < nCountMax; i++ )
        {
            double nPercent = (double)i / (nCountMax-1) * 100;
            double nPosX = spriteProgressBG->getContentSize().width * nPercent / 100;
            
            std::string strIconPath = "Assets/ui/myhome/homeboost_Progress bar_off.png";
            if ( i < _nMyhomeCount )
            {
                strIconPath = "Assets/ui/myhome/homeboost_Progress bar_on.png";
            }
            auto spriteProgressIcon = Sprite::create(strIconPath);
            spriteProgressIcon->setPosition(nPosX, spriteProgressBG->getContentSize().height / 2);
            spriteProgressBG->addChild(spriteProgressIcon);

        }
        
    }
    
 
    if ( _nMyhomeCount > 0 )
    {
        auto spriteEffect = ui::Scale9Sprite::create("Assets/ui_common/box_item_1_1.png", Rect::ZERO, Rect(5,30,10,10));
        spriteEffect->setContentSize(size);
        spriteEffect->setPosition(size.width / 2, size.height / 2);
        spriteEffect->setOpacity(0);
        layout->addChild(spriteEffect);
        
        auto action1 = FadeIn::create(0.5);
        auto action2 = FadeOut::create(0.5);
        auto action3 = DelayTime::create(0.6);
        auto seq1 = Sequence::create(action1, action2, action3, nullptr);
        spriteEffect->runAction(RepeatForever::create(seq1));
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        animation->addSpriteFrameWithFile("Assets/ui/myhome/homeboost_1.png");
        animation->addSpriteFrameWithFile("Assets/ui/myhome/homeboost_2.png");
        animation->addSpriteFrameWithFile("Assets/ui/myhome/homeboost_3.png");
        animation->addSpriteFrameWithFile("Assets/ui/myhome/homeboost_4.png");
        animation->addSpriteFrameWithFile("Assets/ui/myhome/homeboost_5.png");
        animation->addSpriteFrameWithFile("Assets/ui/myhome/homeboost_6.png");
        
        auto seq2 = Sequence::create(Animate::create(animation), DelayTime::create(1.0), nullptr);
        spriteHomeIcon->runAction(RepeatForever::create(seq2));
    }
}

void UILayoutBoost::drawLock()
{
    //
    _layerContainer->removeAllChildren();
    
    auto layout = _layerContainer;
    
    auto size = layout->getContentSize();
    
    //
    auto uiTextInfo = ui::CText::create(GAME_TEXT("t_ui_myhome_info"), GAME_FONT, 20);
    uiTextInfo->setPosition(Vec2(size.width / 2, size.height / 2));
    uiTextInfo->setTextAreaSize(Size(size.width - 5, size.height - 5));
    uiTextInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiTextInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    layout->addChild(uiTextInfo);
}

#pragma mark - set, get, add
int UILayoutBoost::getAdsWatchMax(int floor)
{
    int levelMax = 0;
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);//레벨 받아와서 다시 그리기
    if( level == 0 )
    {
        levelMax = 1;
    }
    else
    {
        switch (level) {
            case 1:     levelMax = 3;    break;
            case 2:     levelMax = 3;    break;
            case 3:     levelMax = 3;    break;
            case 4:     levelMax = 3;    break;
            case 5:     levelMax = 4;    break;
            case 6:     levelMax = 4;    break;
            case 7:     levelMax = 4;    break;
            case 8:     levelMax = 4;    break;
            case 9:     levelMax = 5;    break;
            case 10:    levelMax = 5;    break;
        }
    }
    
    return levelMax;
}

#pragma mark - click


#pragma mark - game
void UILayoutBoost::refresh()
{
    drawContents();
}
