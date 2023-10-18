//
//  PopupToastItems.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupToastItems.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

int PopupToastItems::_nCountVisible = 0;
PopupToastItems* PopupToastItems::create(cocos2d::Vector<InfoItems*> listItems, bool bSpecial, bool bAttend)
{
    PopupToastItems* pRet = new(std::nothrow) PopupToastItems();
    if ( pRet && pRet->init(listItems, bSpecial, bAttend) )
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

PopupToastItems::PopupToastItems(void) :
_nRewardPaddingX(0),
_nRewardPaddingY(0),
_nRewardDrawCol(0),
_nRewardDrawRow(0),
_nRewardWidth(0),
_nRewardHeight(0),
_nRewardBoxSize(0),

_bSpecial(false),

_layerContainerBottom(nullptr),
_layerContainerAttend(nullptr)
{
    
}

PopupToastItems::~PopupToastItems(void)
{
    
}

bool PopupToastItems::init(cocos2d::Vector<InfoItems*> listItems, bool bSpecial, bool bAttend)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _listItems = listItems;
    _bSpecial = bSpecial;
    _bAttend = bAttend;
    
    // init
    initVar();
    initUi();
    
    return true;
}


void PopupToastItems::initVar()
{
    _nRewardPaddingX = 15;
    _nRewardPaddingY = 22;
    
    _nRewardDrawCol = 4;
    _nRewardDrawRow = (int)(_listItems.size() / _nRewardDrawCol);
    if ( _listItems.size() % _nRewardDrawCol != 0 )
    {
        _nRewardDrawRow += 1;
    }
    
    if ( _nRewardDrawRow == 1 && _listItems.size() % _nRewardDrawCol != 0 )
    {
        _nRewardDrawCol = _listItems.size() % _nRewardDrawCol;
    }
    
    _nRewardBoxSize = 90;
    _nRewardWidth = _nRewardBoxSize * _nRewardDrawCol + _nRewardPaddingX * (_nRewardDrawCol - 1);
    _nRewardHeight = _nRewardBoxSize * _nRewardDrawRow + _nRewardPaddingY * _nRewardDrawRow;
}

void PopupToastItems::initUi()
{
    double nPopupWidth = 750;
    double nPopupHeight = (191 + 20) + _nRewardHeight + (20 + 131);
    if ( nPopupHeight < 558 )
        nPopupHeight = 558;
    
    double nPopupBottomWidth = nPopupWidth - 40;
    double nPopupBottomHeight = nPopupHeight - 191 - 131;
    
    auto spriteBG = Sprite::create("Assets/ui_common/reward_bg_1.png");
    spriteBG->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    addChild(spriteBG);
    
    auto spritePopupBG = ui::Scale9Sprite::create("Assets/ui_common/reward_box_1.png", Rect::ZERO, Rect(356,200,2,2));
    spritePopupBG->setContentSize(Size(nPopupWidth, nPopupHeight));
    spritePopupBG->setPosition(getContentSize().width / 2, getContentSize().height / 2 + 200);
    spritePopupBG->setCascadeOpacityEnabled(true);
    spritePopupBG->setOpacity(0);
    addChild(spritePopupBG);
    {
        // run
        auto aniframeRun = Animation::create();
        aniframeRun->addSpriteFrameWithFile("Assets/ui_common/reward_1_1.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui_common/reward_1_2.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui_common/reward_1_3.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui_common/reward_1_4.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui_common/reward_1_5.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui_common/reward_1_6.png");
        aniframeRun->setDelayPerUnit(0.1f);
        
        auto sprintRun = Sprite::create("Assets/ui_common/reward_1_1.png");
        sprintRun->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprintRun->setPosition(spritePopupBG->getContentSize().width / 2, spritePopupBG->getContentSize().height);
        spritePopupBG->addChild(sprintRun);
        
        auto action1 = DelayTime::create(0.15);
        auto action2 = Animate::create(aniframeRun);
        auto action3 = FadeOut::create(0.5f);
        auto action4 = RemoveSelf::create();
        auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
        sprintRun->runAction(seq);
    }
    
    if ( _bSpecial == true )
    {
        auto aniSpecialRun = Animation::create();
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_1.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_2.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_3.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_4.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_5.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_6.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_7.png");
        aniSpecialRun->addSpriteFrameWithFile("Assets/ui_common/reward_sp_1_8.png");
        aniSpecialRun->setDelayPerUnit(0.1f);
        
        auto sprSpecialRun = Sprite::create("Assets/ui_common/reward_sp_1_1.png");
        sprSpecialRun->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprSpecialRun->setPosition(185,285);
        spritePopupBG->addChild(sprSpecialRun);
        
        auto action = Animate::create(aniSpecialRun);
        sprSpecialRun->runAction(action);
    }
    
    _layerContainerBottom = Layer::create();
    _layerContainerBottom->setContentSize( Size(nPopupBottomWidth, nPopupBottomHeight) );
    _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
    _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBottom->setPosition(spritePopupBG->getContentSize().width / 2, 131);
    _layerContainerBottom->setCascadeOpacityEnabled(true);
    spritePopupBG->addChild(_layerContainerBottom);
    
    _layerContainerAttend = Layer::create();
    _layerContainerAttend->setContentSize( Size(657, 520));
    _layerContainerAttend->setIgnoreAnchorPointForPosition(false);
    _layerContainerAttend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerAttend->setPosition(getContentSize().width / 2, spritePopupBG->getPositionY() - spritePopupBG->getContentSize().height / 2 + 100);
    _layerContainerAttend->setCascadeOpacityEnabled(true);
    addChild(_layerContainerAttend);
    
    // action
    {
        double nDelay = 2.0;
        if ( _bAttend == true )
            nDelay = 3.0;
        
        auto seq = Sequence::create(FadeIn::create(0.1f), DelayTime::create(nDelay), FadeOut::create(0.1f), nullptr);
        spritePopupBG->runAction(seq);
    }
    {
        auto seq = Sequence::create(FadeIn::create(0.4), FadeOut::create(0.4), nullptr);
        spriteBG->runAction(seq);
    }
    {
        double nDelay = 2.0;
        if ( _bAttend == true )
            nDelay = 3.0;
        
        auto seqParent = Sequence::create(DelayTime::create(nDelay), RemoveSelf::create(), nullptr);
        runAction(seqParent);
    }

    //
    uiReward();
    if ( _bAttend == true )
    {
        uiAttend();
    }
}

#pragma mark - ui
void PopupToastItems::uiReward()
{
    auto layerItems = Layer::create();
    layerItems->setContentSize(Size(_nRewardWidth, _nRewardHeight));
    layerItems->setIgnoreAnchorPointForPosition(false);
    layerItems->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerItems->setPosition(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height / 2 );
    layerItems->setCascadeOpacityEnabled(true);
    _layerContainerBottom->addChild(layerItems);
    
    for ( int i = 0; i < _listItems.size(); i++ )
    {
        auto obj = _listItems.at(i);
        
        int x = i % _nRewardDrawCol;
        int y = i / _nRewardDrawCol;
        double posX = x * _nRewardBoxSize + x * _nRewardPaddingX;
        double posY = layerItems->getContentSize().height - y * _nRewardBoxSize - y * _nRewardPaddingY;
        
        // item bg
        auto spriteBg = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
        spriteBg->setContentSize(Size(_nRewardBoxSize, _nRewardBoxSize));
        spriteBg->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        spriteBg->setPosition(posX, posY);
        spriteBg->setCascadeOpacityEnabled(true);
        layerItems->addChild(spriteBg);
        
        // item image
        std::string strPathIcon = obj->getPath();
        if ( (E_ITEMS)obj->getIdx() == E_ITEMS::MONSTER_SOUL )
        {
            strPathIcon = MafUtils::format("Assets/character_mon/normal/monster_%d.png", obj->getTypeParams());
        }
        
        if ( strPathIcon.length() != 0 )
        {
            auto spriteIcon = Sprite::create(strPathIcon);
            spriteIcon->setPosition(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height / 2);
            spriteBg->addChild(spriteIcon);
            
            float nWidthBg = spriteBg->getContentSize().width * 0.9;
            float nHeightBg = spriteBg->getContentSize().height * 0.9;
            float nScale = MIN(nWidthBg / spriteIcon->getContentSize().width, nHeightBg / spriteIcon->getContentSize().height);
            if ( nScale > 1 )
            {
                spriteIcon->setScale(nScale);
            }
        }
        
        // item count
        std::string strCount = obj->getCount();
        if ( obj->getType().compare("BUFF") == 0 )
        {
            if ( obj->getTypeDetail().compare("REVIVAL") == 0 )
            {
                strCount = GAME_TEXTFORMAT("t_ui_hour", atoi(obj->getCount().c_str()));
            }
            else
            {
                strCount = GAME_TEXTFORMAT("t_ui_second", atoi(obj->getCount().c_str()));
            }
        }
        if ( obj->getType().compare("ADS") == 0 )
        {
            if ( obj->getIdx() == 48 )
            {
                strCount.append("H");
            }
            else if ( obj->getIdx() == 53 )
            {
                strCount.append("D");
            }
            
        }
        else if(obj->getIdx() == (int)E_ITEMS::WEAPON_SPIRIT && atoi(obj->getCount().c_str()) <= 0)
            strCount = "";
        else if ( obj->isString() == true )
        {
            strCount = MafUtils::convertNumberToShort(strCount);
        }
        
        auto labelCount = Label::createWithTTF(strCount, GAME_FONT, 18);
        labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelCount->setPosition(spriteBg->getContentSize().width / 2, 4);
        spriteBg->addChild(labelCount);
    }
}

void PopupToastItems::uiAttend()
{
    std::string listIcon[] = {
        "Assets/ui/attendance/icon_attendance_reset_01.png",
        "Assets/ui/attendance/icon_attendance_reset_02.png",
        "Assets/ui/attendance/icon_attendance_reset_03.png",
        "Assets/ui/attendance/icon_attendance_reset_04.png",
        "Assets/ui/shop/shop_fever_1.png",
    };
    std::string listText[] = {
        "t_ui_attend_msg_8",
        "t_ui_attend_msg_9",
        "t_ui_attend_msg_10",
        "t_ui_attend_msg_11",
        "t_ui_attend_msg_13",
    };
    
    for ( int i = 0; i < sizeof(listIcon)/sizeof(std::string); i++ )
    {
        double nPosX = _layerContainerAttend->getContentSize().width / 2;
        double nPosY = _layerContainerAttend->getContentSize().height - 111 * i - 19 * i;
        
        auto spriteBG = Sprite::create("Assets/ui/attendance/attendance_reset_box1_1.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteBG->setPosition(nPosX, nPosY);
        spriteBG->setOpacity(0);
        spriteBG->setCascadeOpacityEnabled(true);
        _layerContainerAttend->addChild(spriteBG);
        
        auto spriteRotate = Sprite::create("Assets/ui/attendance/attendance_reset_box1_2.png");
        spriteRotate->setPosition(66, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteRotate);
        
        auto spriteIcon = Sprite::create(listIcon[i]);
        spriteIcon->setPosition(66, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteIcon);
        
        auto labelText = Label::createWithTTF(GAME_TEXT(listText[i]), GAME_FONT, 24);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(140, spriteBG->getContentSize().height / 2);
        labelText->setDimensions(500, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        spriteBG->addChild(labelText);
        
        // action
        auto action1_1 = DelayTime::create(0.1 * i);
        auto action1_2 = FadeIn::create(0.5);
        auto action1_3 = DelayTime::create(2.0);
        auto action1_4 = FadeOut::create(0.2);
        auto action1 = Sequence::create(action1_1, action1_2, action1_3, action1_4, NULL);
        
        auto action2_1 = DelayTime::create(0.1 * i);
        auto action2_2 = MoveBy::create(0, Vec2(0, -20));
        auto action2_3 = MoveBy::create(0.5, Vec2(0, 20));
        auto action2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
        
        auto spawn = Spawn::create(action1, action2, NULL);
        spriteBG->runAction(spawn);
    }
}

#pragma mark - show, lock, hide
void PopupToastItems::showSpecial(cocos2d::Vector<InfoItems*> listItems)
{
    show(listItems, true, false);
}

void PopupToastItems::showAttend(cocos2d::Vector<InfoItems*> listItems)
{
    show(listItems, false, true);
}

void PopupToastItems::show(cocos2d::Vector<InfoItems*> listItems)
{
    show(listItems, false, false);
}

void PopupToastItems::show(cocos2d::Vector<InfoItems*> listItems, bool bSpecial, bool bAttend)
{
    // notification
    auto node = UtilsScene::getInstance()->getRunningScene();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_SCENE_TOAST_REWARD) == nullptr )
    {
        PopupToastItems::_nCountVisible = 0;
    }
    
    // push
    PopupToastItems::_nCountVisible += 1;
    
    // toast add
    auto action1 = DelayTime::create((PopupToastItems::_nCountVisible - 1) * 1.0);
    auto action2 = CallFunc::create([=](){
        
        //
        auto popup = PopupToastItems::create(listItems, bSpecial, bAttend);
        node->addChild(popup, ZORDER_SCENE_TOAST_REWARD, TAG_SCENE_TOAST_REWARD);
    });
    auto action3 = DelayTime::create(0.5);
    auto action4 = CallFunc::create([=](){
        //
        PopupToastItems::_nCountVisible -= 1;
    });
    auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
    node->runAction(seq);
}

#pragma mark - scheduler
