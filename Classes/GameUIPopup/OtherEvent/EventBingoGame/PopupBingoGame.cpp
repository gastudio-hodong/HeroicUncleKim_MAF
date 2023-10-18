//
//  PopupBingoGame.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/21.
//

#include "PopupBingoGame.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoItems.h"

#include "GameUIPopup/OtherEvent/EventBingoGame/PopupBingoNumber.h"

#include "ManagerEvent/EventBingoManager.h"

USING_NS_CC;

PopupBingoGame* PopupBingoGame::create()
{
    PopupBingoGame* pRet = new(std::nothrow) PopupBingoGame();
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

PopupBingoGame::PopupBingoGame(void) :
_timer(0),
_callbackClose(nullptr),

_sprBlockEffect(nullptr),
_lyBoard(nullptr),
_lyTopUI(nullptr),
_lyBottomUI(nullptr)
{
    _listBoardBlock.clear();
    _listCossLineBlock.clear();
}

PopupBingoGame::~PopupBingoGame(void)
{
    _listCossLineBlock.clear();
    _listBoardBlock.clear();
}

bool PopupBingoGame::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupBingoGame::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    EventBingoManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupBingoGame::callbackInfo, this));
    //
}

void PopupBingoGame::update(float dt)
{
    _timer += dt;
    
    if(_timer >= 5)
    {
        if(_sprBlockEffect != nullptr)
            _sprBlockEffect->setVisible(false);
        unscheduleUpdate();
    }
}

#pragma mark - init
void PopupBingoGame::initVar()
{
}

void PopupBingoGame::initUi()
{
    float width = getContentSize().width;
    float height = 1327;
    
    if(width >= 750)
        width = 750;
    
    _spriteContainer->setContentSize(Size(width, height));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);

    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_bingo_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBingoGame::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    _lyTopUI = Layer::create();
    _lyTopUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 375) );
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 80);
    _spriteContainer->addChild(_lyTopUI);
    
    _lyBoard = Layer::create();
    _lyBoard->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 725));
    _lyBoard->setIgnoreAnchorPointForPosition(false);
    _lyBoard->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBoard->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyBoard);
    
    _lyBottomUI = Layer::create();
    _lyBottomUI->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 140));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition(_lyBoard->getPosition() + Vec2::DOWN * _lyBoard->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
}

#pragma mark - ui
void PopupBingoGame::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBG= Sprite::create("Assets/ui/event/bingo_bg_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4);
    _lyTopUI->addChild(sprBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyTopUI->addChild(menu);
    
    auto event = EventBingoManager::getInstance()->getEventIdx();
    
    auto nEventEndTime = EventBingoManager::getInstance()->getEventEndTime();
    
    int64_t nTimeNow = UtilsDate::getInstance()->getTime();
        
    int nTimeDiff = (int)(nEventEndTime - nTimeNow);
    
    int d = nTimeDiff / 24 / 3600;
    int h = nTimeDiff / 3600;
    
    if(d >= 1 ) h %= 24;
    
    if(d <= 0)
        d = 0;
    if(h <= 0)
        h = 0;
    
    std::string strEventTime = "";
    
    if(event == 24)
    {
        strEventTime = GAME_TEXT("t_ui_token_text_upper_1") + " " + GAME_TEXTFORMAT("t_ui_event_bingo_timer_1", d, h);
    }
    else
    {
        strEventTime = GAME_TEXT("t_ui_event_bingo_end") + " " + GAME_TEXTFORMAT("t_ui_event_bingo_timer_1", d, h);
    }
    
    auto lbEventTime = Label::createWithTTF(strEventTime, GAME_FONT, 22);
    lbEventTime->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbEventTime->setPosition(Vec2(10, _lyTopUI->getContentSize().height - 2));
    _lyTopUI->addChild(lbEventTime);
    
    auto itemGuide = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/pet/btn_pet_info.png"), nullptr, CC_CALLBACK_1(PopupBingoGame::onClickHelp,this));
    itemGuide->setTouchRect(itemGuide->getContentSize() * 1.5);
    itemGuide->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    itemGuide->setPosition(Vec2(_lyTopUI->getContentSize().width - 5, _lyTopUI->getContentSize().height - 5));
    menu->addChild(itemGuide);
    
    auto lbGuide = Label::createWithTTF(GAME_TEXT("t_ui_event_btn_guide"), GAME_FONT, 22);
    lbGuide->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    lbGuide->setPosition(itemGuide->getPosition() + Vec2::LEFT * (itemGuide->getContentSize().width + 5));
    _lyTopUI->addChild(lbGuide);
    
    
    auto itemBingoTicket = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/event_btn_03.png"), nullptr, CC_CALLBACK_1(PopupBingoGame::onClickUseBingoTicket, this));
    itemBingoTicket->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemBingoTicket->setPosition(Vec2(10, 0));
    menu->addChild(itemBingoTicket);
    {
        if(EventBingoManager::getInstance()->getBingoCount() >= 12)
        {
            auto lbContent = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_transfer_next", EventBingoManager::getInstance()->getBingoTicketCount()), GAME_FONT, 34);
            lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbContent->setPosition(itemBingoTicket->getContentSize()/2);
            lbContent->setDimensions(itemBingoTicket->getContentSize().width - 10, itemBingoTicket->getContentSize().height - 10);
            lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
            lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbContent->setColor(Color3B(255,224,98));
            itemBingoTicket->addChild(lbContent);
        }
        else
        {
            
            auto lbContent = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_event_bingo_desc", EventBingoManager::getInstance()->getBingoTicketCount()), GAME_FONT, 30);
            lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbContent->enableBold();
            lbContent->setPosition(itemBingoTicket->getContentSize()/2);
            lbContent->setDimensions(itemBingoTicket->getContentSize().width - 10, itemBingoTicket->getContentSize().height/2);
            lbContent->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::BOTTOM);
            lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbContent->setColor(Color3B(224,184,123));
            itemBingoTicket->addChild(lbContent);
            
            auto lbNum = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_event_bingo_btn_num", EventBingoManager::getInstance()->getBingoTicketCount()), GAME_FONT, 24);
            lbNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbNum->setPosition(itemBingoTicket->getContentSize()/2);
            lbNum->setDimensions(itemBingoTicket->getContentSize().width - 10, itemBingoTicket->getContentSize().height/2);
            lbNum->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::TOP);
            lbNum->setOverflow(cocos2d::Label::Overflow::SHRINK);
            itemBingoTicket->addChild(lbNum);
        }
    }
    
    auto objBlockInfo = EventBingoManager::getInstance()->getLineRewardBlock(37);
    if(objBlockInfo != nullptr)
    {
        std::string strTilePath = "Assets/ui/event/bingo_blocks_2_1.png";
        if(objBlockInfo->isReward())
        {
            strTilePath = "Assets/ui/event/bingo_blocks_2_2.png";
        }
        
        auto itemRewardBlock = MafNode::MafMenuItemSprite::create(Sprite::create(strTilePath), nullptr, CC_CALLBACK_1(PopupBingoGame::onClickLineReward, this));
        itemRewardBlock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        itemRewardBlock->setTag(objBlockInfo->getNum());
        itemRewardBlock->setPosition(itemBingoTicket->getPosition() + Vec2::RIGHT * itemBingoTicket->getContentSize().width);
        menu->addChild(itemRewardBlock);
        {
            auto item = objBlockInfo->getItem();
            if(item != nullptr)
            {
                auto sprRewardIcon = Sprite::create(item->getPath());
                sprRewardIcon->getTexture()->setTexParameters(texParams);
                sprRewardIcon->setScale(2);
                sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprRewardIcon->setPosition(Vec2(itemRewardBlock->getContentSize()/2) + Vec2::UP * 10);
                itemRewardBlock->addChild(sprRewardIcon);
                
                
                auto lbCount = MafNode::createBmFont(item->getCount(), GAME_BM_FONT_AL, 24);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                lbCount->setPosition(sprRewardIcon->getPosition() + Vec2::DOWN * (sprRewardIcon->getContentSize().height/2));
                itemRewardBlock->addChild(lbCount);
            }
            
            if(objBlockInfo->isReward())
            {
                auto sprRewardIcon = Sprite::create("Assets/icon/icon_check_01.png");
                sprRewardIcon->getTexture()->setTexParameters(texParams);
                sprRewardIcon->setScale(2);
                sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprRewardIcon->setPosition(itemRewardBlock->getContentSize()/2);
                itemRewardBlock->addChild(sprRewardIcon);
            }
            
            if(EventBingoManager::getInstance()->isRewardPossible(objBlockInfo->getNum()) && !objBlockInfo->isReward())
            {
                auto sprLight = Sprite::create("Assets/ui/event/bingo_blocks_3_1.png");
                sprLight->setPosition(itemRewardBlock->getContentSize()/2);
                itemRewardBlock->addChild(sprLight);

                auto fideOut = FadeOut::create(0.5f);
                auto fideIn = FadeIn::create(0.5f);
                Vector<FiniteTimeAction*> actions;
                actions.pushBack(fideOut);
                actions.pushBack(fideIn);
                auto sequence = Sequence::create(actions);
                auto rep = RepeatForever::create(sequence);
                sprLight->runAction(rep);
            }
        }
    }
}

void PopupBingoGame::uiBoard()
{
    _listCossLineBlock.clear();
    _listBoardBlock.clear();
    _lyBoard->removeAllChildren();
    _sprBlockEffect = nullptr;
    
    double nWidth = 120;
    double nHeight = 120;
    
    auto listBoardBlock = EventBingoManager::getInstance()->getListBoardBlock();
    
    double posStartX = nWidth/2 + 10;
    double posStartY = _lyBoard->getContentSize().height;
    double posLastX = 0;
    double posLastY = 0;
    for ( int i = 0; i < listBoardBlock.size(); i++ )
    {
        auto objBlockInfo = listBoardBlock.at(i);
        if(objBlockInfo == nullptr)
            continue;
        
        int nColumn = i % 5;
        int nRow = i / 5;
        double nPosX = posStartX + nColumn * nWidth;
        double nPosY = posStartY - nRow * nHeight - nHeight / 2;
        posLastX = nPosX;
        posLastY = nPosY;
        //
        std::string strTilePath = "Assets/ui/event/bingo_blocks_1_1.png";
        if(objBlockInfo->isReward())
        {
            if((i + 1) % 2 == 0)
            {
                strTilePath = "Assets/ui/event/bingo_blocks_1_3.png";
            }
            else
            {
                strTilePath = "Assets/ui/event/bingo_blocks_1_2.png";
            }
        }
        auto spriteTile = Sprite::create(strTilePath);
        spriteTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        spriteTile->setPosition(nPosX, nPosY);
        spriteTile->setTag(objBlockInfo->getIdx());
        _lyBoard->addChild(spriteTile);
        {
            auto lbNum = Label::createWithTTF(std::to_string(objBlockInfo->getNum()), GAME_FONT, 50);
            lbNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbNum->setPosition(spriteTile->getContentSize()/2);
            lbNum->setDimensions(spriteTile->getContentSize().width - 10, spriteTile->getContentSize().height - 10);
            lbNum->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
            lbNum->setOverflow(cocos2d::Label::Overflow::SHRINK);
            spriteTile->addChild(lbNum);
            if(objBlockInfo->isReward())
            {
                lbNum->setOpacity(30);
            }
        }
        
        if(objBlockInfo->getIdx() == 3 || objBlockInfo->getIdx() == 8 || objBlockInfo->getIdx() == 11 ||
           objBlockInfo->getIdx() == 12 || objBlockInfo->getIdx() == 13 || objBlockInfo->getIdx() == 14 ||
           objBlockInfo->getIdx() == 15 || objBlockInfo->getIdx() == 18 || objBlockInfo->getIdx() == 23 )
        {
            _listCossLineBlock.pushBack(spriteTile);
        }
        
        _listBoardBlock.pushBack(spriteTile);
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyBoard->addChild(menu);
    
    listBoardBlock = EventBingoManager::getInstance()->getListLineRewardBlock();
    for(int i = 0; i < 5; ++i)
    {
        auto objBlockInfo = listBoardBlock.at(i);
        if(objBlockInfo == nullptr)
            continue;
        
        int nColumn = i % 5;
        double nPosX = posStartX + nColumn * nWidth;
        std::string strTilePath = "Assets/ui/event/bingo_blocks_2_1.png";
        if(objBlockInfo->isReward())
        {
            strTilePath = "Assets/ui/event/bingo_blocks_2_2.png";
        }
        
        auto itemRewardBlock = MafNode::MafMenuItemSprite::create(Sprite::create(strTilePath), nullptr, CC_CALLBACK_1(PopupBingoGame::onClickLineReward, this));
        itemRewardBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemRewardBlock->setTag(objBlockInfo->getNum());
        itemRewardBlock->setPosition(nPosX, posLastY - itemRewardBlock->getContentSize().height);
        menu->addChild(itemRewardBlock);
        {
            auto item = objBlockInfo->getItem();
            if(item == nullptr)
                continue;
            
            auto sprRewardIcon = Sprite::create(item->getPath());
            sprRewardIcon->getTexture()->setTexParameters(texParams);
            sprRewardIcon->setScale(2);
            sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprRewardIcon->setPosition(Vec2(itemRewardBlock->getContentSize()/2) + Vec2::UP * 10);
            itemRewardBlock->addChild(sprRewardIcon);
            
            
            auto lbCount = MafNode::createBmFont(item->getCount(), GAME_BM_FONT_AL, 24);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbCount->setPosition(sprRewardIcon->getPosition() + Vec2::DOWN * (sprRewardIcon->getContentSize().height/2));
            itemRewardBlock->addChild(lbCount);
            
            if(objBlockInfo->isReward())
            {
                auto sprRewardIcon = Sprite::create("Assets/icon/icon_check_01.png");
                sprRewardIcon->getTexture()->setTexParameters(texParams);
                sprRewardIcon->setScale(2);
                sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprRewardIcon->setPosition(itemRewardBlock->getContentSize()/2);
                itemRewardBlock->addChild(sprRewardIcon);
            }
            
            if(EventBingoManager::getInstance()->isRewardPossible(objBlockInfo->getNum()) && !objBlockInfo->isReward())
            {
                auto sprLight = Sprite::create("Assets/ui/event/bingo_blocks_3_1.png");
                sprLight->setPosition(itemRewardBlock->getContentSize()/2);
                itemRewardBlock->addChild(sprLight);

                auto fideOut = FadeOut::create(0.5f);
                auto fideIn = FadeIn::create(0.5f);
                Vector<FiniteTimeAction*> actions;
                actions.pushBack(fideOut);
                actions.pushBack(fideIn);
                auto sequence = Sequence::create(actions);
                auto rep = RepeatForever::create(sequence);
                sprLight->runAction(rep);
            }
        }
    }
    
    for(int i = 0; i < 6; ++i)
    {
        auto objBlockInfo = listBoardBlock.at(i + 5);
        if(objBlockInfo == nullptr)
            continue;
        
        double nPosY = posStartY - i * nHeight - nHeight / 2;
        std::string strTilePath = "Assets/ui/event/bingo_blocks_2_1.png";
        if(objBlockInfo->isReward())
        {
            strTilePath = "Assets/ui/event/bingo_blocks_2_2.png";
        }
        
        auto itemRewardBlock = MafNode::MafMenuItemSprite::create(Sprite::create(strTilePath), nullptr, CC_CALLBACK_1(PopupBingoGame::onClickLineReward, this));
        itemRewardBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemRewardBlock->setTag(objBlockInfo->getNum());
        itemRewardBlock->setPosition(posLastX + itemRewardBlock->getContentSize().width, nPosY);
        menu->addChild(itemRewardBlock);
        {
            auto item = objBlockInfo->getItem();
            if(item == nullptr)
                continue;
            
            auto sprRewardIcon = Sprite::create(item->getPath());
            sprRewardIcon->getTexture()->setTexParameters(texParams);
            sprRewardIcon->setScale(2);
            sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprRewardIcon->setPosition(Vec2(itemRewardBlock->getContentSize()/2) + Vec2::UP * 10);
            itemRewardBlock->addChild(sprRewardIcon);
            
            
            auto lbCount = MafNode::createBmFont(item->getCount(), GAME_BM_FONT_AL, 24);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbCount->setPosition(sprRewardIcon->getPosition() + Vec2::DOWN * (sprRewardIcon->getContentSize().height/2));
            itemRewardBlock->addChild(lbCount);
            
            if(objBlockInfo->isReward())
            {
                auto sprRewardIcon = Sprite::create("Assets/icon/icon_check_01.png");
                sprRewardIcon->getTexture()->setTexParameters(texParams);
                sprRewardIcon->setScale(2);
                sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprRewardIcon->setPosition(itemRewardBlock->getContentSize()/2);
                itemRewardBlock->addChild(sprRewardIcon);
            }
            
            if(EventBingoManager::getInstance()->isRewardPossible(objBlockInfo->getNum()) && !objBlockInfo->isReward())
            {
                auto sprLight = Sprite::create("Assets/ui/event/bingo_blocks_3_1.png");
                sprLight->setPosition(itemRewardBlock->getContentSize()/2);
                itemRewardBlock->addChild(sprLight);

                auto fideOut = FadeOut::create(0.5f);
                auto fideIn = FadeIn::create(0.5f);
                Vector<FiniteTimeAction*> actions;
                actions.pushBack(fideOut);
                actions.pushBack(fideIn);
                auto sequence = Sequence::create(actions);
                auto rep = RepeatForever::create(sequence);
                sprLight->runAction(rep);
            }
        }
    }
    
    uiBingoLine();
    uiBoardBlockReward();
    
    _sprBlockEffect = Sprite::create("Assets/ui/event/bingo_blocks_3_1.png");
    _sprBlockEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprBlockEffect->setPosition(Vec2::ZERO);
    _lyBoard->addChild(_sprBlockEffect);
    {
        auto fideOut = FadeOut::create(0.5f);
        auto fideIn = FadeIn::create(0.5f);
        Vector<FiniteTimeAction*> actions;
        actions.pushBack(fideOut);
        actions.pushBack(fideIn);
        auto sequence = Sequence::create(actions);
        auto rep = RepeatForever::create(sequence);
        _sprBlockEffect->runAction(rep);
        
        _sprBlockEffect->setVisible(false);
    }
}

void PopupBingoGame::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyBottomUI->addChild(menu);
    
    auto listQuestInfo = EventBingoManager::getInstance()->getListBingoQuestBlock();
    
    Size sizeBtn = Size(234,106);
    
    float startX = 13 + sizeBtn.width/2;
    
    for(int i = 0; i < listQuestInfo.size(); ++i)
    {
        auto questInfo = listQuestInfo.at(i);
        std::string path = "Assets/ui_common/btn_2_1_off.png";
        
        if(EventBingoManager::getInstance()->isQuestRewardPossible(questInfo->getNum()))
            path = "Assets/ui_common/btn_2_4_on.png";
        
        auto sprQuestBtn = ui::Scale9Sprite::create(path);
        sprQuestBtn->setContentSize(Size(234,106));
        auto item = questInfo->getItem();
        
        auto itemQuestBtn = MafNode::MafMenuItemSprite::create(sprQuestBtn, nullptr, CC_CALLBACK_1(PopupBingoGame::onClickQuestReward, this));
        itemQuestBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemQuestBtn->setPosition(Vec2(startX, _lyBottomUI->getContentSize().height/2));
        itemQuestBtn->setTag(questInfo->getNum());
        menu->addChild(itemQuestBtn);
        startX += sprQuestBtn->getContentSize().width + 5;
        
        auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
        sprBox->setContentSize(Size(82,82));
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprBox->setPosition(Vec2(10, itemQuestBtn->getContentSize().height/2));
        itemQuestBtn->addChild(sprBox);
        {
            auto sprIcon = Sprite::create(item->getPath());
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIcon->setPosition(sprBox->getContentSize()/2);
            sprBox->addChild(sprIcon);
            
            auto lbContent = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_event_bingo_btn_round", questInfo->getNum()), GAME_FONT, 28);
            lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbContent->setPosition(sprBox->getPosition() + Vec2::RIGHT * (sprBox->getContentSize().width + 5) + Vec2::UP * 10);
            lbContent->setDimensions(itemQuestBtn->getContentSize().width/2 - 5, itemQuestBtn->getContentSize().height/2);
            lbContent->setAlignment(cocos2d::TextHAlignment::LEFT,cocos2d::TextVAlignment::CENTER);
            lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbContent->setColor(Color3B(224,184,123));
            itemQuestBtn->addChild(lbContent);
            
            auto lbRewardName = Label::createWithTTF(item->getText() +" "+ item->getCount(), GAME_FONT, 22);
            lbRewardName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbRewardName->setPosition(lbContent->getPosition() + Vec2::DOWN * 26);
            lbRewardName->setDimensions(itemQuestBtn->getContentSize().width/2 - 5, itemQuestBtn->getContentSize().height/3);
            lbRewardName->setAlignment(cocos2d::TextHAlignment::LEFT,cocos2d::TextVAlignment::CENTER);
            lbRewardName->setOverflow(cocos2d::Label::Overflow::SHRINK);
            itemQuestBtn->addChild(lbRewardName);
            
            if(EventBingoManager::getInstance()->isQuestRewardPossible(questInfo->getNum()) && !questInfo->isReward())
            {
                auto sprLight = ui::Scale9Sprite::create("Assets/ui/event/bingo_blocks_3_1.png");
                sprLight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprLight->setPosition(itemQuestBtn->getContentSize()/2);
                sprLight->setContentSize(itemQuestBtn->getContentSize());
                itemQuestBtn->addChild(sprLight);

                auto fideOut = FadeOut::create(0.5f);
                auto fideIn = FadeIn::create(0.5f);
                Vector<FiniteTimeAction*> actions;
                actions.pushBack(fideOut);
                actions.pushBack(fideIn);
                auto sequence = Sequence::create(actions);
                auto rep = RepeatForever::create(sequence);
                sprLight->runAction(rep);
            }
            
            if(questInfo->isReward())
            {
                auto sprRewardIcon = Sprite::create("Assets/icon/icon_check_01.png");
                sprRewardIcon->getTexture()->setTexParameters(texParams);
                sprRewardIcon->setScale(2);
                sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprRewardIcon->setPosition(sprBox->getContentSize()/2);
                sprBox->addChild(sprRewardIcon);
            }
            
        }
        
    }
    
}

void PopupBingoGame::uiBingoLine()
{
    std::map<int, int> mapBingoLine;
    mapBingoLine.insert(std::make_pair(26, 11));
    mapBingoLine.insert(std::make_pair(27, 12));
    mapBingoLine.insert(std::make_pair(28, 13));
    mapBingoLine.insert(std::make_pair(29, 14));
    mapBingoLine.insert(std::make_pair(30, 15));
    mapBingoLine.insert(std::make_pair(31, 3));
    mapBingoLine.insert(std::make_pair(32, 8));
    mapBingoLine.insert(std::make_pair(33, 13));
    mapBingoLine.insert(std::make_pair(34, 18));
    mapBingoLine.insert(std::make_pair(35, 23));
    mapBingoLine.insert(std::make_pair(36, 13));
    mapBingoLine.insert(std::make_pair(37, 13));
    
    for(auto value : mapBingoLine)
    {
        int rewardBlockIdx = value.first;
        float rotate = 0;
        std::string strPath = "Assets/ui/event/bingo_line_1_2.png";
        auto sprBlock = getCrossLineBlock(value.second);
        if(!EventBingoManager::getInstance()->isRewardPossible(rewardBlockIdx) || sprBlock == nullptr)
            continue;
        
        if(rewardBlockIdx == 31 || rewardBlockIdx == 32 || rewardBlockIdx == 33
           || rewardBlockIdx == 34 || rewardBlockIdx == 35)
        {
            strPath = "Assets/ui/event/bingo_line_1_1.png";
            rotate = 90;
        }
        else if(rewardBlockIdx == 36)
        {
            strPath = "Assets/ui/event/bingo_line_1_3.png";
            rotate = 135;
        }
        else if(rewardBlockIdx == 37)
        {
            strPath = "Assets/ui/event/bingo_line_1_3.png";
            rotate = 45;
        }
        
        auto sprLine = ui::Scale9Sprite::create(strPath);
        sprLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprLine->setPosition(sprBlock->getPosition());
        sprLine->setRotation(rotate);
        _lyBoard->addChild(sprLine);
        
        if(rewardBlockIdx == 36)
        {
            sprLine->setContentSize(sprLine->getContentSize() + Size(0,230));
        }
        else if(rewardBlockIdx == 37)
        {
            sprLine->setContentSize(sprLine->getContentSize() + Size(0,230));
        }
        
    }
}

void PopupBingoGame::uiBoardBlockReward()
{
    auto listBoardBlock = EventBingoManager::getInstance()->getListBoardBlock();
    for(int i = 0; i < _listBoardBlock.size(); ++i)
    {
        auto spriteTile = _listBoardBlock.at(i);
        auto objBlockInfo = listBoardBlock.at(i);
        
        auto item = objBlockInfo->getItem();
        if(spriteTile == nullptr || item == nullptr)
            continue;

        auto sprRewardIcon = Sprite::create(item->getPath());
        sprRewardIcon->getTexture()->setTexParameters(texParams);
        sprRewardIcon->setScale(2);
        sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprRewardIcon->setPosition(spriteTile->getPosition() + Vec2::UP * 10);
        _lyBoard->addChild(sprRewardIcon);


        auto lbCount = MafNode::createBmFont(item->getCount(), GAME_BM_FONT_AL, 24);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbCount->setPosition(sprRewardIcon->getPosition() + Vec2::DOWN * (sprRewardIcon->getContentSize().height/2));
        _lyBoard->addChild(lbCount);
    }
}

void PopupBingoGame::uiRedraw()
{
    unscheduleUpdate();
    
    uiTop();
    uiBoard();
    uiBottom();
}

#pragma mark - utils
Sprite* PopupBingoGame::getCrossLineBlock(int nIdx)
{
    Sprite* result = nullptr;
    
    for(auto spr : _listCossLineBlock)
    {
        if(spr->getTag() == nIdx)
        {
            result = spr;
            break;
        }
    }
    
    return result;
}

#pragma mark - click
void PopupBingoGame::onClickClose(Ref* sender)
{
    hide();
}

void PopupBingoGame::onClickLineReward(Ref* sender)
{
    auto node = (Node*) sender;
    
    if(EventBingoManager::getInstance()->isRewardPossible(node->getTag()) == false || EventBingoManager::getInstance()->getLineRewardBlock(node->getTag())->isReward())
        return;
    
    PopupLoading::show();
    EventBingoManager::getInstance()->requestReward(CC_CALLBACK_2(PopupBingoGame::callbackReward,this), node->getTag());
}

void PopupBingoGame::onClickQuestReward(Ref* sender)
{
    auto node = (Node*) sender;
    
    if(EventBingoManager::getInstance()->isQuestRewardPossible(node->getTag()) == false || EventBingoManager::getInstance()->getQuestRewardBlock(node->getTag())->isReward())
        return;
    
    PopupLoading::show();
    EventBingoManager::getInstance()->requestQuestReward(CC_CALLBACK_2(PopupBingoGame::callbackReward,this), node->getTag());
}

void PopupBingoGame::onClickUseBingoTicket(Ref* sender)
{
    if(EventBingoManager::getInstance()->getBingoCount() >= 12)
    {
        onClickNextRound();
        return;
    }
    
    if(EventBingoManager::getInstance()->getBingoTicketCount() <= 0)
    {
        
        std::string massage = GAME_TEXT("t_ui_event_bingo_empty");
        
        if(EventBingoManager::getInstance()->getEventIdx() == 25)
            massage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), massage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
            
        return;
    }
    else
    {
        PopupLoading::show();
        EventBingoManager::getInstance()->requestUseTicket(CC_CALLBACK_4(PopupBingoGame::callbackUseTicket,this));
    }
}

void PopupBingoGame::onClickNextRound()
{
    if(EventBingoManager::getInstance()->isRoundEnd() == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_bingo_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
            
        return;
    }
    
    PopupLoading::show();
    EventBingoManager::getInstance()->requestNextBoard(CC_CALLBACK_2(PopupBingoGame::callbackNextRound,this));
}

void PopupBingoGame::onClickHelp(Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_bingo_detail"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->setContentSize(Size(655, 750));
    popup->setContentAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    popup->show();
}

#pragma mark - callback
void PopupBingoGame::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    uiRedraw();
}

void PopupBingoGame::callbackReward(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    uiRedraw();
    
}

void PopupBingoGame::callbackNextRound(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    if(EventBingoManager::getInstance()->getBingoCompleteCount() == 3)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_event_bingo_title"), GAME_TEXT("t_ui_event_bingo_done"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    
    uiRedraw();
    
}

void PopupBingoGame::callbackUseTicket(bool bResult, int nResult, int nNun, Vector<InfoItems*> listReward)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    uiRedraw();
    
    auto popup = PopupBingoNumber::create(nNun, listReward);
    popup->show();
    
    auto listInfo = EventBingoManager::getInstance()->getListBoardBlock();
    
    for(int i = 0; i < _listBoardBlock.size(); ++i)
    {
        auto sprBlock = _listBoardBlock.at(i);
        auto objInfo = listInfo.at(i);
        
        if(objInfo->getNum() == nNun)
        {
            _sprBlockEffect->setPosition(sprBlock->getPosition());
            _sprBlockEffect->setVisible(true);
            
            _timer = 0;
            
            scheduleUpdate();
            break;
        }
    }
}
