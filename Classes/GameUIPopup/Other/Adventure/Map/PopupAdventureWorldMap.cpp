//
//  PopupAdventureWorldMap.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/25.
//

#include "PopupAdventureWorldMap.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureFurnitureUpgrade.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureParty.h"
#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventurePartyMiniSpec.h"
#include "GameUIPopup/Other/Adventure/Map/PopupAdventureStage.h"
#include "GameUIPopup/Other/Adventure/Boss/PopupAdventureBossSelect.h"
#include "GameUIPopup/Other/Adventure/Boss/PopupAdventureBossBattle.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureWorldMap* PopupAdventureWorldMap::create(const int nAdventureID, bool isFriend)
{
    PopupAdventureWorldMap* pRet = new(std::nothrow) PopupAdventureWorldMap();
    if ( pRet && pRet->init(nAdventureID, isFriend) )
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

PopupAdventureWorldMap::PopupAdventureWorldMap(void):
_isFriend(false)
,_nCurrentAdventureID(0)
,_fIncrese(0)
,_lyMapUI(nullptr)
,_lyTouchAreaUI(nullptr)
,_lyBottomUI(nullptr)
,_callbackHide(nullptr)
{
}

PopupAdventureWorldMap::~PopupAdventureWorldMap(void)
{
}

#pragma mark -init
bool PopupAdventureWorldMap::init(const int nAdventureID, bool isFriend)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _nCurrentAdventureID = nAdventureID;
    _isFriend = isFriend;
    
    initVar();
    initUI();
    _bHideBackKey = false;
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}
#pragma mark - Override
void PopupAdventureWorldMap::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureWorldMap::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}

#pragma mark - init
void PopupAdventureWorldMap::initVar()
{
    double nWidth = 750;
    double nHeight = getContentSize().height;
    if ( nHeight > 1280 )
        nHeight = 1280;
    
    _fIncrese = 1280 - nHeight;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width/2, 0);
}

void PopupAdventureWorldMap::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_worldmap_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureWorldMap::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    Size size = _spriteContainer->getContentSize();
    _lyTouchAreaUI = LayerColor::create();
    _lyTouchAreaUI->setContentSize( Size(size.width - 10, size.height - 375));
    _lyTouchAreaUI->setIgnoreAnchorPointForPosition(false);
    _lyTouchAreaUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTouchAreaUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTouchAreaUI, 2);
    
    _lyMapUI = LayerColor::create();
    _lyMapUI->setContentSize( Size(size.width - 10, size.height - 375));
    _lyMapUI->setIgnoreAnchorPointForPosition(false);
    _lyMapUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMapUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyMapUI, 1);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 450));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _lyBottomUI->setPosition( _spriteContainer->getContentSize().width/2, 0);
    _spriteContainer->addChild(_lyBottomUI, 3);
    
    uiMap();
    uiTouchArea();
    uiBottom();
}

void PopupAdventureWorldMap::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark -ui
void PopupAdventureWorldMap::uiMap()
{
    _lyMapUI->removeAllChildren();
    
    Vec2 posStateInfo = Vec2(_lyMapUI->getContentSize().width/2, 155);
    if(_isFriend)
        posStateInfo = Vec2(_lyMapUI->getContentSize().width/2, 10);
    auto sprStateIconInfoBox = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_work_info_box.png", Rect::ZERO, Rect(5, 0, 6, 16));
    sprStateIconInfoBox->setContentSize(Size(590, 30));
    sprStateIconInfoBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprStateIconInfoBox->setPosition(posStateInfo);
    _lyMapUI->addChild(sprStateIconInfoBox, 11);
    {
        float posX = 10;
        for(int i = 0; i < 4; ++i)
        {
            auto sprIcon = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_work_icon%02d.png", i+1));
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprIcon->setPosition(posX, sprStateIconInfoBox->getContentSize().height/2);
            sprStateIconInfoBox->addChild(sprIcon);
            
            auto lbIconName = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_stage_state_%d", i+1)), GAME_FONT, 22);
            lbIconName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbIconName->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width + 5));
            lbIconName->setDimensions(115, sprStateIconInfoBox->getContentSize().height);
            lbIconName->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbIconName->setOverflow(Label::Overflow::SHRINK);
            sprStateIconInfoBox->addChild(lbIconName);
            
            posX += 142;
        }
    }
    
    auto sprTextBox = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_5.png");
    sprTextBox->setContentSize(Size(300, 40));
    sprTextBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprTextBox->setPosition(sprStateIconInfoBox->getPosition() + Vec2::UP * (sprStateIconInfoBox->getContentSize().height + 5));
    _lyMapUI->addChild(sprTextBox, 11);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_worldmap_info_1"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprTextBox->getContentSize()/2);
        lbText->setDimensions(sprTextBox->getContentSize().width - 10, sprTextBox->getContentSize().height);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B(114,69,25,255), 1);
        sprTextBox->addChild(lbText);
    }
    
    auto sprBG = Sprite::create("Assets/ui/adventure/adventure_bg.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyMapUI->getContentSize().width/2, _lyMapUI->getContentSize().height);
    _lyMapUI->addChild(sprBG, 1);
    
    auto sprAreaLine = Sprite::create("Assets/ui/adventure/adventure_bg_area.png");
    sprAreaLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprAreaLine->setPosition(_lyMapUI->getContentSize().width/2, _lyMapUI->getContentSize().height);
    _lyMapUI->addChild(sprAreaLine, 10);
    sprAreaLine->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), nullptr)));
    
    auto spriteEffect1 = Sprite::create("Assets/ui/adventure/adventure_bg_obj_1_1.png");
    spriteEffect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteEffect1->setPosition(sprBG->getPosition());
    _lyMapUI->addChild(spriteEffect1, 1);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 4; i++ )
        {
            animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/adventure/adventure_bg_obj_1_%d.png", i));
        }
        spriteEffect1->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    auto listArea = AdventureManager::getInstance()->getListInfoMap();
    
    for(int i = 0; i < listArea.size(); ++i)
    {
        auto obj = listArea.at(i);
        if(!obj->isOpen())
        {
            auto sprLockArea = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_bg_lock_%02d.png", obj->getIdx()));
            sprLockArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            sprLockArea->setPosition(_lyMapUI->getContentSize().width/2, _lyMapUI->getContentSize().height);
            sprLockArea->setColor(Color3B(120,120,120));
            _lyMapUI->addChild(sprLockArea, 1);
        }
    }
}

void PopupAdventureWorldMap::uiTouchArea()
{
    _lyTouchAreaUI->removeAllChildren();
     
    
    std::vector<std::pair<Size, Vec2>> list =
    {
        std::make_pair(Size(290, 260), Vec2(600, 380)),
        std::make_pair(Size(300, 130), Vec2(180, 670)),
        std::make_pair(Size(160, 150), Vec2(160, 480)),
        std::make_pair(Size(190, 160), Vec2(650,630)),
        std::make_pair(Size(170, 150), Vec2(100,280)),
        std::make_pair(Size(300, 150), Vec2(160,840)),
        std::make_pair(Size(250, 170), Vec2(500,830)),
    };
    
    auto listArea = AdventureManager::getInstance()->getListInfoMap();
    for(int i = 0; i < list.size(); ++i)
    {
        auto obj = list[i];
        auto objArea = listArea.at(i);
        
        auto lyPoint = ui::Layout::create();
        lyPoint->setContentSize(obj.first);
        lyPoint->setIgnoreAnchorPointForPosition(false);
        lyPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lyPoint->setPosition(obj.second + Vec2::DOWN * _fIncrese);
        lyPoint->addClickEventListener(CC_CALLBACK_1(PopupAdventureWorldMap::onClickArea, this));
        lyPoint->setTouchEnabled(true);
        lyPoint->setTag(i+1);
        _lyTouchAreaUI->addChild(lyPoint);
        
        auto sprAreaNumber = Sprite::create("Assets/ui/adventure/adventure_area_text.png");
        sprAreaNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprAreaNumber->setPosition(lyPoint->getContentSize()/2);
        lyPoint->addChild(sprAreaNumber);
        {
            auto lbText = Label::createWithTTF(std::to_string(i+1), GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(Vec2(sprAreaNumber->getContentSize()/2));
            sprAreaNumber->addChild(lbText);
        }
        
        std::string strIconPath = "Assets/ui/adventure/adventure_area_select_lock.png";
        if(objArea->isOpen())
            strIconPath = "Assets/ui/adventure/adventure_area_select.png";
        
        auto sprSelect = Sprite::create(strIconPath);
        sprSelect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprSelect->setPosition(sprAreaNumber->getPosition() + Vec2::UP * 4);
        lyPoint->addChild(sprSelect);
        
        if(objArea->isOpen())
        {
            auto action1 = MoveBy::create(0.5f,Vec2(0, 15));
            auto action2 = action1->reverse();
            auto seq = Sequence::create(action1,action2, NULL);
            sprSelect->runAction(RepeatForever::create(seq));
            
            auto listStage = objArea->getListStage();
            int size = (int)listStage.size();
            if(objArea->getIdx() == 1)
                size--;
            
            float width = 25;
            float posX = lyPoint->getContentSize().width/2 - width * (size/2) - 5 * (size/2);
            for(int j = 0; j < listStage.size(); ++j)
            {
                auto objStage = listStage.at(j);
                if(objStage->getIdx() == 0)
                    continue;
                
                std::string strIconPath = "Assets/ui/adventure/adventure_work_icon04.png";
                if(objStage->isOpen())
                    strIconPath = "Assets/ui/adventure/adventure_work_icon01.png";
                if(objStage->isRunning())
                    strIconPath = "Assets/ui/adventure/adventure_work_icon02.png";
                if(objStage->isComplete())
                    strIconPath = "Assets/ui/adventure/adventure_work_icon03.png";
                
                auto sprStageStateIcon = Sprite::create(strIconPath);
                sprStageStateIcon->setPosition(posX, sprAreaNumber->getPosition().y - sprAreaNumber->getContentSize().height/2 - 5);
                sprStageStateIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                lyPoint->addChild(sprStageStateIcon);
                
                if(size % 2 == 0)
                    sprStageStateIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                
                posX += width + 5;
            }
        }
    }
    
    // 보스
    auto uiLayoutBoss = ui::Layout::create();
    uiLayoutBoss->setContentSize(Size(132, 140));
    uiLayoutBoss->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutBoss->setPosition(Vec2(335, 450));
    uiLayoutBoss->setTouchEnabled(true);
    uiLayoutBoss->addClickEventListener(CC_CALLBACK_1(PopupAdventureWorldMap::onClickAreaBoss, this));
    _lyTouchAreaUI->addChild(uiLayoutBoss);
    {
        int level = AdventureManager::getInstance()->getLevel();
        int levelCondition = AdventureManager::getInstance()->getSetting("boss_open_map_lv")->getValueToInt();
        
        if ( level >= levelCondition )
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1);
            for ( int i = 1; i <= 5; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/adventure/boss/adventure_boss_on_2_%d.png", i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            auto uiAreaWave = ui::ImageView::create("Assets/ui/adventure/boss/adventure_boss_on_2_1.png");
            uiAreaWave->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiAreaWave->setPosition(Vec2(uiLayoutBoss->getContentSize().width / 2, 0));
            uiAreaWave->getVirtualRenderer()->stopAllActions();
            uiAreaWave->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            uiLayoutBoss->addChild(uiAreaWave);
            
            auto iconPath = "Assets/ui/adventure/boss/adventure_boss_on_1.png";
            
            auto bossIdx = AdventureManager::getInstance()->getBossIdx();
            if ( bossIdx != 0 )
            {
                auto objInfoBoss = AdventureManager::getInstance()->getInfoBoss(bossIdx);
                if ( objInfoBoss->getHpNow() <= 0 )
                {
                    iconPath = "Assets/ui/adventure/boss/adventure_boss_off_1.png";
                }
            }
            
            auto uiAreaIcon = ui::ImageView::create(iconPath);
            uiAreaIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiAreaIcon->setPosition(Vec2(uiLayoutBoss->getContentSize().width / 2, uiLayoutBoss->getContentSize().height));
            uiAreaIcon->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, 10)), MoveBy::create(0.5, Vec2(0, -10)), NULL)));
            uiLayoutBoss->addChild(uiAreaIcon);
        }
        else
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1);
            for ( int i = 1; i <= 4; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/adventure/boss/adventure_boss_off_2_%d.png", i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            auto uiAreaWave = ui::ImageView::create("Assets/ui/adventure/boss/adventure_boss_off_2_1.png");
            uiAreaWave->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiAreaWave->setPosition(Vec2(uiLayoutBoss->getContentSize().width / 2, 0));
            uiAreaWave->getVirtualRenderer()->stopAllActions();
            uiAreaWave->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            uiLayoutBoss->addChild(uiAreaWave);
        }
    }
    
    
    // 갈매기
    auto spriteEffect2 = Sprite::create("Assets/ui/adventure/adventure_bg_obj_2_1.png");
    spriteEffect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteEffect2->setPosition(_lyTouchAreaUI->getContentSize().width/2, _lyTouchAreaUI->getContentSize().height);
    _lyTouchAreaUI->addChild(spriteEffect2, 10);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.10f);
        for ( int i = 1; i <= 4; i++ )
        {
            animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/adventure/adventure_bg_obj_2_%d.png", i));
        }
        spriteEffect2->runAction(RepeatForever::create(Animate::create(animation)));
        
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto spriteMapBG = (Sprite*)sender->getParent();
            if ( spriteMapBG == nullptr )
            {
                return;
            }
            
            ccBezierConfig bezierConfig;
            bezierConfig.controlPoint_1 = Vec2(random(0, (int)spriteMapBG->getContentSize().width), -50);
            bezierConfig.controlPoint_2 = Vec2(random(0, (int)spriteMapBG->getContentSize().width), spriteMapBG->getContentSize().height / 2);
            bezierConfig.endPosition    = Vec2(random(0, (int)spriteMapBG->getContentSize().width), spriteMapBG->getContentSize().height + 50);
            
            auto action1 = BezierTo::create(30, bezierConfig);
            auto action2 = Place::create(Vec2(random(0, (int)spriteMapBG->getContentSize().width), -50));
            auto seq = Sequence::create(action1, action2, NULL);
            sender->runAction(seq);
            
        });
        auto action2 = DelayTime::create(31);
        auto seq = Sequence::create(action1, action2, NULL);
        spriteEffect2->runAction(RepeatForever::create(seq));
    }
    
    // 구름
    auto spriteEffect3 = Sprite::create("Assets/ui/adventure/adventure_bg_obj_3_1.png");
    spriteEffect3->setPosition(_lyTouchAreaUI->getContentSize().width / 2, _lyTouchAreaUI->getContentSize().height / 2);
    spriteEffect3->setOpacity(0);
    _lyTouchAreaUI->addChild(spriteEffect3, 10);
    
    // 구름
    auto spriteEffect4 = Sprite::create("Assets/ui/adventure/adventure_bg_obj_3_1.png");
    spriteEffect4->setPosition(_lyTouchAreaUI->getContentSize().width / 2, _lyTouchAreaUI->getContentSize().height / 2);
    spriteEffect4->setOpacity(0);
    _lyTouchAreaUI->addChild(spriteEffect4, 10);
    {
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto spriteMapBG = (Sprite*)sender->getParent();
            if ( spriteMapBG == nullptr )
            {
                return;
            }
            
            int nDelayTime = random(6, 13);
            auto action1 = Place::create(Vec2(random(0, (int)spriteMapBG->getContentSize().width), random(300, (int)spriteMapBG->getContentSize().height)));
            auto action2 = FadeIn::create(1.0);
            action2->setOpacity(128);
            auto action3 = DelayTime::create(nDelayTime);
            auto action4 = FadeOut::create(1.0);
            auto seq = Sequence::create(action1, action2, action3, action4, NULL);
            auto spawn = Spawn::create(seq, MoveBy::create(nDelayTime+2, Vec2(-200, 0)), NULL);
            sender->runAction(spawn);
        });
        auto action2 = DelayTime::create(15);
        auto seq = Sequence::create(action1, action2, NULL);
        auto repeat = RepeatForever::create(seq);
        spriteEffect3->runAction(repeat);
        
    }
    {
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto spriteMapBG = (Sprite*)sender->getParent();
            if ( spriteMapBG == nullptr )
            {
                return;
            }
            
            int nDelayTime = random(6, 13);
            auto action1 = Place::create(Vec2(random(0, (int)spriteMapBG->getContentSize().width), random(300, (int)spriteMapBG->getContentSize().height)));
            auto action2 = FadeIn::create(1.0);
            action2->setOpacity(128);
            auto action3 = DelayTime::create(nDelayTime);
            auto action4 = FadeOut::create(1.0);
            auto seq = Sequence::create(action1, action2, action3, action4, NULL);
            auto spawn = Spawn::create(seq, MoveBy::create(nDelayTime+2, Vec2(-200, 0)), NULL);
            sender->runAction(spawn);
        });
        auto action2 = DelayTime::create(20);
        auto seq = Sequence::create(action1, action2, NULL);
        auto repeat = RepeatForever::create(seq);
        spriteEffect4->runAction(repeat);
    }
}

void PopupAdventureWorldMap::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    if(_isFriend)
        _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 300));
    
    auto sprHideBox = ui::Scale9Sprite::create("Assets/ui/purchase/shop_box1_0.png");
    sprHideBox->setContentSize(Size(_spriteContainer->getContentSize().width, _lyBottomUI->getContentSize().height));
    sprHideBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprHideBox->setPosition(_lyBottomUI->getContentSize().width/2, 0);
    _lyBottomUI->addChild(sprHideBox);

    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setContentSize(_lyBottomUI->getContentSize());
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(_lyBottomUI->getContentSize().width/2, 0);
    _lyBottomUI->addChild(sprBox);
    
    auto uiSpec = UILayerAdventurePartyMiniSpec::create(sprBox->getContentSize());
    uiSpec->setAdventureID(_nCurrentAdventureID);
    uiSpec->setIsFriend(_isFriend);
    uiSpec->initUi();
    sprBox->addChild(uiSpec);
}

#pragma mark -click
void PopupAdventureWorldMap::onClickClose(Ref* const pSender)
{
    if(AdventureManager::getInstance()->getTutorialStep() == 1)
        return;
    
    hide();
}

void PopupAdventureWorldMap::onClickArea(Ref* const pSender)
{
    auto uiButton = (ui::Layout*)pSender;
    
    int nArea = uiButton->getTag();
    
    auto objMap = AdventureManager::getInstance()->getInfoMap(nArea);
    if(!objMap->isOpen())
    {
        if(AdventureManager::getInstance()->getTutorialStep() > 6)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_advt_worldmap_error_title_2", objMap->getOpenCondition()));
            popup->addButton(GAME_TEXT("t_ui_advt_bt_1"), false, [&](Ref*){
                auto popup = PopupAdventureFurnitureUpgrade::create(1);
                popup->setCallbackUpgrade(CC_CALLBACK_0(PopupAdventureWorldMap::callbackRefresh, this));
                popup->show();
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
        else
        {
            Vec2 posToast = _spriteContainer->getParent()->convertToWorldSpace(_spriteContainer->getContentSize()/2);
            PopupToast::showImmediately(GAME_TEXT("t_ui_advt_error_7"), posToast.x, posToast.y);
        }
        return;
    }
    
    auto popup = PopupAdventureStage::create(_nCurrentAdventureID, nArea, _isFriend);
    popup->setCallbackHide(CC_CALLBACK_0(PopupAdventureWorldMap::callbackHide, this));
    popup->setCallbackRefresh(CC_CALLBACK_0(PopupAdventureWorldMap::callbackRefresh, this));
    popup->show();
}

void PopupAdventureWorldMap::onClickAreaBoss(Ref* const sender)
{
    int level = AdventureManager::getInstance()->getLevel();
    int levelCondition = AdventureManager::getInstance()->getSetting("boss_open_map_lv")->getValueToInt();
    if ( level < levelCondition )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_advt_boss_unlock", levelCondition));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    auto bossIdx = AdventureManager::getInstance()->getBossIdx();
    if ( bossIdx == 0 )
    {
        auto popup = PopupAdventureBossSelect::create();
        popup->show();
    }
    else
    {
        auto popup = PopupAdventureBossBattle::create();
        popup->show();
        
        hide();
    }
}

#pragma mark -callback
void PopupAdventureWorldMap::callbackHide()
{
    if(_callbackHide != nullptr)
        _callbackHide();
    
    hide();
}

void PopupAdventureWorldMap::callbackRefresh()
{
    uiMap();
    uiTouchArea();
    uiBottom();
}
