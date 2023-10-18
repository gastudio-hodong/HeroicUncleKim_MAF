//
//  PopupTimetravel.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupTimeTravel.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Friend/LayerFriendList.h"
#include "GameUIPopup/Other/Friend/LayerFriendInvite.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/TimeTravelManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupTimeTravel* PopupTimeTravel::create()
{
    PopupTimeTravel* pRet = new(std::nothrow) PopupTimeTravel();
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

PopupTimeTravel::PopupTimeTravel(void) :
_nFloorPlus(0),
_onCallbackTimeTravel(nullptr),

_layerContainerInfo(nullptr),
_layerContainerButton(nullptr),
_layerContainerContents(nullptr),

_table(nullptr),
_labelTime(nullptr),
_labelTimeTravelCount(nullptr),
_labelTimeTravelTime(nullptr)
{
    
}

PopupTimeTravel::~PopupTimeTravel(void)
{
    
}

bool PopupTimeTravel::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    return true;
}
#pragma mark - init
void PopupTimeTravel::initVar()
{
    //
    PopupLoading::show();
    
    TimeTravelManager::getInstance()->setCallbackCalculation(CC_CALLBACK_0(PopupTimeTravel::callbackTimeTravelCalculation, this));
    auto t = std::thread(CC_CALLBACK_0(TimeTravelManager::OnRewardCalculation, TimeTravelManager::getInstance()));
    t.detach();
}

void PopupTimeTravel::initUi()
{
    // size
    Size size = Size(630, 900);
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_time_jump_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupTimeTravel::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerInfo = LayerColor::create();
    _layerContainerInfo->setContentSize( Size(size.width - 25, 298) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height - 75) );
    _spriteContainer->addChild(_layerContainerInfo);
    
    // 재화
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 55));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - _layerContainerInfo->getContentSize().height));
    _spriteContainer->addChild(uiCurrency);
    
    //
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(size.width - 25, 116) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 25, size.height - 75 - _layerContainerInfo->getContentSize().height - 55 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - _layerContainerInfo->getContentSize().height - 55) );
    _spriteContainer->addChild(_layerContainerContents);
    
    
 
    //
    uiInfo();
    uiContent();
    uiButton();
}

#pragma mark - table
void PopupTimeTravel::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupTimeTravel::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 100);
}

extension::TableViewCell* PopupTimeTravel::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    
    E_TIME_TRAVEL listType[] = {E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD, E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES, E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD};
    E_TIME_TRAVEL type = listType[idx];
    
    int level = TimeTravelManager::getInstance()->getItemLevel(type);
    int levelMax = TimeTravelManager::getInstance()->getItemMaxLevel(type);
    
    int effect = TimeTravelManager::getInstance()->getItemEffectSub(type, level);
    int price = TimeTravelManager::getInstance()->getItemPrice(type, level);
    int floorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    
    int nCondition = TimeTravelManager::getInstance()->getItemMaxLevelCondition();
    int nConditionDiff = (nCondition + 1) - floorHigh;
    
    auto spriteBg = ui::Scale9Sprite::create(PIXEL, Rect::ZERO, Rect(0,0,10,10));
    spriteBg->setContentSize(Size(size.width * 0.98, size.height * 0.95));
    spriteBg->setPosition(size.width / 2, size.height / 2);
    spriteBg->setColor(Color3B(29,41,51));
    cell->addChild(spriteBg);
    
    // 버튼
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBg->addChild(menu);
    
    auto spriteBuyEnable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteBuyEnable->setContentSize(Size(180, 78));
    
    auto spriteBuySelected = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(33,33,2,2));
    spriteBuySelected->setContentSize(Size(180, 78));
    
    auto spriteBuyDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(33,33,2,2));
    spriteBuyDisable->setContentSize(Size(180, 78));
    
    auto itemBuy = MafNode::MafMenuItemSprite::create(spriteBuyEnable, spriteBuySelected, spriteBuyDisable, CC_CALLBACK_1(PopupTimeTravel::onClickItemBuy, this));
    itemBuy->setPosition(spriteBg->getContentSize().width - itemBuy->getContentSize().width / 2 - 10, spriteBg->getContentSize().height / 2 );
    itemBuy->setTag(type);
    menu->addChild(itemBuy);
    
    
    if ( level >= levelMax )
    {
        if ( nCondition != 0 )
        {
            itemBuy->setEnabled(false);
            
            std::string strCondition = GAME_TEXTFORMAT("t_ui_pet_msg_6_1", nCondition);
            strCondition.append("\n");
            strCondition.append(GAME_TEXTFORMAT("t_ui_pet_msg_17", nConditionDiff));
            
            auto labelText = Label::createWithTTF(strCondition, GAME_FONT, 24);
            labelText->setPosition(itemBuy->getContentSize().width / 2, itemBuy->getContentSize().height / 2);
            labelText->setDimensions(itemBuy->getContentSize().width * 0.95, itemBuy->getContentSize().height * 0.95);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemBuy->addChild(labelText);
        }
        else
        {
            itemBuy->setVisible(false);
        }
    }
    else
    {
        std::string strIcon = "Assets/icon/icon_jewel.png";
        auto spritIcon = Sprite::create(strIcon);
        spritIcon->setPosition(itemBuy->getContentSize().width * 0.25, itemBuy->getContentSize().height / 2);
        itemBuy->addChild(spritIcon);
        
        
        std::string strPrice = MafUtils::format("x %d", price);
        auto labelPrice = Label::createWithTTF(strPrice, GAME_FONT, 22);
        labelPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelPrice->setPosition(itemBuy->getContentSize().width * 0.35, itemBuy->getContentSize().height / 2);
        labelPrice->setColor(COLOR_COIN);
        itemBuy->addChild(labelPrice);
        
        itemBuy->setEnabled(true);
        itemBuy->unselected();
        if ( ItemsMoneyManager::getInstance()->getGem() < price )
        {
            itemBuy->selected();
        }
    }
    
    Size sizeContent = spriteBg->getContentSize();
    if ( itemBuy->isVisible() == true )
    {
        sizeContent.width = sizeContent.width - itemBuy->getContentSize().width - 20;
    }
    auto layerContent = Layer::create();
    layerContent->setContentSize(sizeContent);
    layerContent->setIgnoreAnchorPointForPosition(false);
    layerContent->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    layerContent->setPosition(Vec2::ZERO);
    spriteBg->addChild(layerContent);
    {
        //
        std::string strTitleKey = TimeTravelManager::getInstance()->getItemNameKey(type);
        std::string strTitle = GAME_TEXTFORMAT(strTitleKey, level, levelMax);
        std::string strOptionKey = TimeTravelManager::getInstance()->getItemOptionKey(type);
        std::string strOption = GAME_TEXTFORMAT(strOptionKey, effect);
        if ( type == E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD || type ==  E_TIME_TRAVEL::TIME_TRAVEL_CHARGE_DES )
        {
            strOption = GAME_TEXTFORMAT(strOptionKey, effect);
        }
        
        auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 27);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelTitle->setPosition(Vec2(10, layerContent->getContentSize().height*0.98));
        labelTitle->setColor(COLOR_COIN);
        labelTitle->setDimensions(layerContent->getContentSize().width, labelTitle->getContentSize().height);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        labelTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        layerContent->addChild(labelTitle);

        //
        auto labelOption = Label::createWithTTF(strOption, GAME_FONT, 22);
        labelOption->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelOption->setPosition(10, layerContent->getContentSize().height * 0.025);
        layerContent->addChild(labelOption);
    }
    
    return cell;
}

ssize_t PopupTimeTravel::numberOfCellsInTableView(extension::TableView *table)
{
    return 3;
}

#pragma mark - ui
void PopupTimeTravel::uiInfo()
{
    //
    _layerContainerInfo->removeAllChildren();
    
    //
    auto imageBG = ui::ImageView::create("Assets/ui/fever/time_jump_bg_1.png");
    imageBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    imageBG->setPosition(Vec2(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height));
    _layerContainerInfo->addChild(imageBG);
    
    auto textInfo1 = Label::createWithTTF(GAME_TEXT("t_ui_time_jump_mag_1"), GAME_FONT, 28);
    textInfo1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textInfo1->setPosition(Vec2(_layerContainerInfo->getContentSize().width / 2, 150));
    _layerContainerInfo->addChild(textInfo1);
    
    auto textInfo2 = Label::createWithTTF(GAME_TEXT("t_ui_time_jump_mag_2"), GAME_FONT, 20);
    textInfo2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textInfo2->setPosition(Vec2(_layerContainerInfo->getContentSize().width / 2, textInfo1->getPositionY() - textInfo1->getContentSize().height));
    textInfo2->setColor(Color3B(255, 131, 69));
    textInfo2->setDimensions(_layerContainerInfo->getContentSize().width - 10, 0);
    textInfo2->setOverflow(Label::Overflow::SHRINK);
    textInfo2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _layerContainerInfo->addChild(textInfo2);
    
    auto spriteTimeBG = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_5.png");
    spriteTimeBG->setContentSize(Size(242,62));
    spriteTimeBG->setPosition(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height - 50);
    _layerContainerInfo->addChild(spriteTimeBG);
    
    _labelTime = Label::createWithTTF("00:00", GAME_FONT_AL, 60);
    _labelTime->setPosition(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height - 50);
    _layerContainerInfo->addChild(_labelTime);
    {
        auto spriteIcon = Sprite::create("Assets/ui/prison/clock.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteIcon->setPosition(-20, _labelTime->getContentSize().height / 2);
        spriteIcon->setName("WIDGET_ICON");
        _labelTime->addChild(spriteIcon);
        _labelTime->setPositionX(_labelTime->getPositionX() + spriteIcon->getBoundingBox().size.width / 2);
    }
    
    _labelFloorPlus = Label::createWithTTF(GAME_TEXT("t_ui_time_jump_mag_4"), GAME_FONT, 26);
    _labelFloorPlus->setPosition(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height - 100);
    _labelFloorPlus->setColor(Color3B(81, 255, 0));
    _layerContainerInfo->addChild(_labelFloorPlus);
    {
        auto spriteIcon = Sprite::create("Assets/ui/offline/icon_offline_normal.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteIcon->setPosition(-20, _labelFloorPlus->getContentSize().height / 2);
        spriteIcon->setScale(0.6f);
        spriteIcon->setName("WIDGET_ICON");
        _labelFloorPlus->addChild(spriteIcon);
        _labelFloorPlus->setPositionX(_labelFloorPlus->getPositionX() + spriteIcon->getBoundingBox().size.width / 2);
    }
    
    //
    drawInfo();
}

void PopupTimeTravel::uiContent()
{
    //
    _layerContainerContents->removeAllChildren();
    
    //
    _table = extension::TableView::create(this, _layerContainerContents->getContentSize());
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerContents->getContentSize().width / 2, 0) );
    _table->setDelegate(this);
    _table->setBounceable(false);
    _layerContainerContents->addChild(_table);
}

void PopupTimeTravel::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();

    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);

    // item Ok
    auto spriteOk = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteOk->setContentSize(Size(260, 116));
    
    auto spriteOkDisable = ui::Scale9Sprite::create("Assets/ui_common/btn_1_off.png", Rect::ZERO, Rect(32,35,10,2));
    spriteOkDisable->setContentSize(Size(260, 116));
    
    auto itemOk = MafNode::MafMenuItemSprite::create(spriteOk, nullptr, spriteOkDisable, CC_CALLBACK_1(PopupTimeTravel::onClickUse, this));
    menu->addChild(itemOk);
    {
        _labelTimeTravelCount = Label::createWithTTF("", GAME_FONT, 36);
        _labelTimeTravelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _labelTimeTravelCount->setPosition(itemOk->getContentSize().width / 2, itemOk->getContentSize().height / 2 - 10);
        _labelTimeTravelCount->setColor(Color3B(255,223,28));
        itemOk->addChild(_labelTimeTravelCount);
        
        _labelTimeTravelTime = Label::createWithTTF("", GAME_FONT, 22);
        _labelTimeTravelTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _labelTimeTravelTime->setPosition(itemOk->getContentSize().width / 2, itemOk->getContentSize().height / 2 - 5);
        itemOk->addChild(_labelTimeTravelTime);
        
        // action
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto itemOk = (MafNode::MafMenuItemSprite*)sender;
            int count = TimeTravelManager::getInstance()->getTimeTravelCount();
            if ( count <= 0 || _nFloorPlus <= 0 )
            {
                ((Sprite*)itemOk->getNormalImage())->setTexture("Assets/ui_common/btn_1_off.png");
                ((Sprite*)itemOk->getNormalImage())->setContentSize(Size(260, 116));
            }
            else
            {
                ((Sprite*)itemOk->getNormalImage())->setTexture("Assets/ui_common/btn_1_1_on.png");
                ((Sprite*)itemOk->getNormalImage())->setContentSize(Size(260, 116));
            }
            
            
        });
        auto action2 = DelayTime::create(0.1f);
        auto seq = Sequence::create(action1, action2, NULL);
        itemOk->runAction(RepeatForever::create(seq));
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
    
    //
    drawButton();
}

#pragma mark - ui draw
void PopupTimeTravel::drawInfo()
{
    int level = TimeTravelManager::getInstance()->getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD);
    int time = TimeTravelManager::getInstance()->getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD, level);
    
    //
    int32_t m = time / 60;
    int32_t s = time % 60;

    std::string strTime = StringUtils::format("%02d:%02d", m, s);
    _labelTime->setString(strTime);
}

void PopupTimeTravel::drawButton()
{
    int count = TimeTravelManager::getInstance()->getTimeTravelCount();
    int countMax = TimeTravelManager::getInstance()->getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, TimeTravelManager::getInstance()->getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));
    
    _labelTimeTravelCount->setString(StringUtils::format("%d / %d", count, countMax));
    
    //
    {
        auto action1 = CallFuncN::create([=](Node* sender){
        
            int time = TimeTravelManager::getInstance()->getTimeTravelChangeTime();
            int32_t m = (int32_t)(time / 60);
            int32_t s = time % 60;

            std::string strTime = StringUtils::format("%d : %02d", m, s);
            ((Label*)sender)->setString(strTime);
            
        });
        auto action2 = DelayTime::create(1.0f);
        auto seq = Sequence::create(action1, action2, NULL);
        _labelTimeTravelTime->stopAllActions();
        _labelTimeTravelTime->runAction(RepeatForever::create(seq));
    }
    
    //
    {
        auto action1 = CallFuncN::create([=](Node* sender){
        
            int count = TimeTravelManager::getInstance()->getTimeTravelCount();
            int countMax = TimeTravelManager::getInstance()->getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD, TimeTravelManager::getInstance()->getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_COUNT_ADD));

            std::string strTime = StringUtils::format("%d / %d", count, countMax);
            ((Label*)sender)->setString(strTime);
            
        });
        auto action2 = DelayTime::create(1.0f);
        auto seq = Sequence::create(action1, action2, NULL);
        _labelTimeTravelCount->stopAllActions();
        _labelTimeTravelCount->runAction(RepeatForever::create(seq));
    }
}

#pragma mark - set, get, other
void PopupTimeTravel::setCallbackTimeTravel(const std::function<void(void)>& callback)
{
    _onCallbackTimeTravel = callback;
}

#pragma mark - click
void PopupTimeTravel::onClickClose(Ref* sender)
{
    hide();
}

void PopupTimeTravel::onClickItemBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto item = (MafNode::MafMenuItemSprite*)sender;
    E_TIME_TRAVEL type = (E_TIME_TRAVEL)item->getTag();
    
    int level = TimeTravelManager::getInstance()->getItemLevel(type);
    int levelMax = TimeTravelManager::getInstance()->getItemMaxLevel(type);
    if ( level >= levelMax )
    {
        return;
    }
    
    auto price = TimeTravelManager::getInstance()->getItemPrice(type, level);
    if ( ItemsMoneyManager::getInstance()->getGem() < price )
    {
        item->selected();
        
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        return;
    }
    
    bool isSuccess = TimeTravelManager::getInstance()->OnItemLevel(type);
    if ( isSuccess == true )
    {
        _table->reloadData();
        
        //
        drawInfo();
        drawButton();
        
        //
        if ( type == E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD )
        {
            PopupLoading::show();
        
            TimeTravelManager::getInstance()->setCallbackCalculation(CC_CALLBACK_0(PopupTimeTravel::callbackTimeTravelCalculation, this));
            auto t = std::thread(CC_CALLBACK_0(TimeTravelManager::OnRewardCalculation, TimeTravelManager::getInstance()));
            t.detach();
        }
    }
}

void PopupTimeTravel::onClickUse(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    if ( UserInfoManager::getInstance()->getItemFeverTime() > 0 || UserInfoManager::getInstance()->getFeverTime() > 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_time_jump_mag_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int count = TimeTravelManager::getInstance()->getTimeTravelCount();
    if ( count <= 0 )
    {
        return;
    }
    
    if ( _nFloorPlus <= 0 )
    {
        Vec2 posToast = item->getParent()->convertToWorldSpace(item->getPosition());
        posToast.y += item->getContentSize().height / 2;
        posToast.y += 10;
        
        PopupToast::showImmediately(GAME_TEXT("t_ui_time_jump_error1"), posToast.x, posToast.y);
        return;
    }
    
    bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
    int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
    if ( bPlay == true || nEnterAutoCount > 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            
            // 지하감옥 삭제
            auto sceen = UtilsScene::getInstance()->getRunningScene();
            sceen->removeChildByTag(TAG_SCENE_PRISON);
            
            //
            onTimeTravel();
            
        });
        popup->show();
    }
    else
    {
        onTimeTravel();
    }
}

#pragma mark - callback
void PopupTimeTravel::onTimeTravel()
{
    bool isSuccess = TimeTravelManager::getInstance()->OnTimeTravel();
    if ( isSuccess == true )
    {
        if ( _onCallbackTimeTravel != nullptr )
        {
            _onCallbackTimeTravel();
        }
        
        hide();
    }
}
 
#pragma mark - callback
void PopupTimeTravel::callbackTimeTravelCalculation()
{
    PopupLoading::hide();
    
    _nFloorPlus = TimeTravelManager::getInstance()->getRewardFloor();
    if ( _nFloorPlus < 0 )
    {
        _nFloorPlus = 0;
    }
    
    _labelFloorPlus->setString(MafUtils::format("+ %d", _nFloorPlus));
    
    auto spriteIcon = (Sprite*)_labelFloorPlus->getChildByName("WIDGET_ICON");
    _labelFloorPlus->setPositionX(_layerContainerInfo->getContentSize().width / 2);
    _labelFloorPlus->setPositionX(_labelFloorPlus->getPositionX() + spriteIcon->getBoundingBox().size.width / 2);
}
