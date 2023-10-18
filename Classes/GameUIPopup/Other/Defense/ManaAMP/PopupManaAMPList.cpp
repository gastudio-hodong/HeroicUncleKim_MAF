//
//  PopupManaAMPList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/11.
//

#include "PopupManaAMPList.h"

#include "GameObject/Defense/InfoDefenseManaAMP.h"
#include "GameObject/Defense/InfoDefenseManaAmpStone.h"
#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUIPopup/Common/PopupToast.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPDefault.h"

#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;


PopupManaAMPList* PopupManaAMPList::create()
{
    PopupManaAMPList* pRet = new(std::nothrow) PopupManaAMPList();
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

PopupManaAMPList::PopupManaAMPList(void):
_lyTop(nullptr),
_lyTopInfo(nullptr),
_lyMiddle(nullptr),
_lyBottom(nullptr),
_table(nullptr)
{
}

PopupManaAMPList::~PopupManaAMPList(void)
{
    DefenseManaAMPManager::getInstance()->_onServerInfo.Clear(this);
    DefenseManaAMPManager::getInstance()->_onServerUpdateAMP.Clear(this);
    DefenseManaAMPManager::getInstance()->_onServerEquipStone.Clear(this);
}

#pragma mark - table
void PopupManaAMPList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupManaAMPList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 150);
}

extension::TableViewCell* PopupManaAMPList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    cell->setContentSize(size);
    
    auto floorList = DefenseManaAMPManager::getInstance()->getAMPOpenFloorList();
    if(idx >= floorList.size())
        return cell;
    
    int openFloor = floorList[idx];
    auto currentAmpData = DefenseManaAMPManager::getInstance()->getManaAmpData(idx+1);
    Color3B color;
    std::string textPower = "";
    
    auto sprBG = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox1.png");
    sprBG->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBG);
    
    //130, 27
    auto sprGear = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox_gear.png");
    sprGear->setPosition(Vec2(180, 32));
    cell->addChild(sprGear);
    
    auto sprLine = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox2.png");
    sprLine->setPosition(Vec2(cell->getContentSize()/2) + Vec2::RIGHT * 12);
    cell->addChild(sprLine);
    
    auto sprGraph = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox_graph00.png");
    sprGraph->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprGraph->setPosition(22, 8);
    sprLine->addChild(sprGraph);
    
    //32, 71
    auto sprNumber = Sprite::create(MafUtils::format("Assets/ui/defense/Horse_power_amplifier_num%d.png", idx + 1));
    sprNumber->setPosition(Vec2(32, 71));
    sprBG->addChild(sprNumber);
    
    //-35, 14
    auto sprPowerBox = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox3.png");
    sprPowerBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    sprPowerBox->setPosition(Vec2(size.width - 32, 14));
    cell->addChild(sprPowerBox);
    
    Vec2 iconLocation = Vec2(90, cell->getContentSize().height - 20);

    if(currentAmpData == nullptr)
    {
        color = Color3B(70,70,70);
        sprLine->setColor(color);
        sprGraph->setColor(color);
        sprNumber->setColor(color);
        sprGear->setColor(color);
        sprPowerBox->setVisible(false);
        
        for(int i = 0; i < 10; ++i)
        {
            std::string iconPath = "Assets/ui/defense/Horse_power_stone_mini_off.png";
            auto icon = Sprite::create(iconPath);
            icon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            icon->setPosition(iconLocation);
            icon->setColor(color);
            cell->addChild(icon);
            
            iconLocation += Vec2::RIGHT * (icon->getContentSize().width + 15);
        }
        
        auto lbCondition = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_condition") + " : " + MafUtils::format("B%d", openFloor), GAME_FONT, 28);
        lbCondition->setPosition( Vec2(cell->getContentSize()/2) );
        lbCondition->setDimensions(cell->getContentSize().width - 200, lbCondition->getContentSize().height);
        lbCondition->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
        lbCondition->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbCondition->setColor(Color3B::RED);
        cell->addChild(lbCondition);
        
        return cell;
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    auto layerBG = Layer::create();
    layerBG->setContentSize(size - Size(15, 15));
    
    auto itemBG = MafNode::MafMenuItemSprite::create(layerBG, nullptr, CC_CALLBACK_1(PopupManaAMPList::onClickAMP, this));
    itemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemBG->setPosition(size/2);
    itemBG->setTag(currentAmpData->getIndex());
    menu->addChild(itemBG);
    
    if(currentAmpData->isComplete())
    {
        if(currentAmpData->getRedStoneCount() == 10)
        {
            color = Color3B(235,200,0);
        }
        else
        {
            color = Color3B(0,225,245);
        }
        
        sprLine->setColor(color);
        sprGraph->setColor(color);
        sprNumber->setColor(color);
        sprGear->setColor(color);
        
        auto lbPowerText = Label::createWithTTF(GAME_TEXT("t_ui_town_msg_2") , GAME_FONT, 22);
        lbPowerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbPowerText->setPosition(Vec2(42, sprPowerBox->getContentSize().height/2));
        lbPowerText->setDimensions(sprPowerBox->getContentSize().width/2, lbPowerText->getContentSize().height);
        lbPowerText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbPowerText->setOverflow(Label::Overflow::SHRINK);
        sprPowerBox->addChild(lbPowerText);
        
        std::string strEffect = "%s%%";
        
        std::string strEffectTmp = MafUtils::doubleToString(currentAmpData->getManaAMPPower() * 100);
        //strEffectTmp = MafUtils::addCommaToString(strEffectTmp, 3, ',');
        
        MafUtils::convertNumberToShort(strEffectTmp);
        strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());
        
        auto lbPower = Label::createWithTTF(strEffect, GAME_FONT, 22);
        lbPower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbPower->setPosition(Vec2(sprPowerBox->getContentSize().width - 42, sprPowerBox->getContentSize().height/2));
        lbPower->setDimensions(sprPowerBox->getContentSize().width/2, lbPowerText->getContentSize().height);
        lbPower->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        lbPower->setOverflow(Label::Overflow::SHRINK);
        sprPowerBox->addChild(lbPower);
        
        sprGear->runAction(RepeatForever::create(RotateBy::create(10, 360)));
        
        auto anim = Animation::create();
        for(int i = 0; i <= 5; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/defense/Horse_power_amplifier_listbox_graph%02d.png", i);
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        anim->setDelayPerUnit(0.1f);
        auto animate = Animate::create(anim);
        sprGraph->runAction(RepeatForever::create(animate));
    }
    else
    {
        color = Color3B(110,110,110);
        sprLine->setColor(color);
        sprGraph->setColor(color);
        sprNumber->setColor(color);
        sprGear->setColor(color);
        
        auto lbIncomplete = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_incomlete_1") , GAME_FONT, 22);
        lbIncomplete->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbIncomplete->setPosition(sprPowerBox->getContentSize()/2);
        lbIncomplete->setDimensions(sprPowerBox->getContentSize().width - 10, lbIncomplete->getContentSize().height);
        lbIncomplete->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbIncomplete->setOverflow(Label::Overflow::SHRINK);
        sprPowerBox->addChild(lbIncomplete);
    }
    
    
    
    bool isLeftFail = false;
    for(int i = 0; i < 10; ++i)
    {
        std::string stdValue = "";
        std::string iconPath = "";
        bool isSuccess = false;
        bool isCurrentNumber = false;
        E_DF_AMP_STONE_Type stoneType = (E_DF_AMP_STONE_Type)currentAmpData->getStoneIndexByOrder(i+1);
        
        switch(stoneType)
        {
            case E_DF_AMP_STONE_Type::NONE:
            {
                if(i == currentAmpData->getStoneCount())
                    isCurrentNumber = true;
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_off.png";
            }break;
            case E_DF_AMP_STONE_Type::BLACK:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_gray.png";
                stdValue = "+";
            }break;
            case E_DF_AMP_STONE_Type::RED:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_red.png";
                isSuccess = true;
                if(auto data = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)E_DF_AMP_STONE_Type::RED))
                    stdValue = MafUtils::format("%d", (int)data->getOutputQuantity());
            }break;
            case E_DF_AMP_STONE_Type::YELLO:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_yellow.png";
                isSuccess = true;
                if(auto data = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)E_DF_AMP_STONE_Type::YELLO))
                    stdValue = MafUtils::format("%.1f", data->getOutputQuantity());
            }break;
        }
        
        auto icon = Sprite::create(iconPath);
        icon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        icon->setPosition(iconLocation);
        cell->addChild(icon);
        
        if(isCurrentNumber)
        {
            auto currentEffect = Sprite::create("Assets/ui/defense/Horse_power_stone_mini_select.png");
            currentEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            currentEffect->setPosition(icon->getContentSize()/2);
            icon->addChild(currentEffect);

            auto action1 = FadeOut::create(0.5f);
            auto action2 = FadeIn::create(0.5f);

            currentEffect->runAction(RepeatForever::create(Sequence::create(action1, action2,  nullptr)));
        }
        if(isSuccess)
        {
            auto successEffect = Sprite::create("Assets/ui/defense/Horse_power_stone_mini_blue.png");
            successEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            successEffect->setPosition(icon->getContentSize()/2);
            icon->addChild(successEffect);
            
            if(currentAmpData->getRedStoneCount() == 10)
            {
                successEffect->setColor(Color3B(255,240,0));
            }
            else
            {
                successEffect->setColor(Color3B(0,225,255));
            }
            
            if(i != 0 && !isLeftFail)
            {
                auto successLine = Sprite::create("Assets/ui/defense/Horse_power_stone_line_blue.png");
                successLine->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                successLine->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height/2);
                successLine->getTexture()->setTexParameters(texParams);
                cell->addChild(successLine);
                
                
                if(currentAmpData->getRedStoneCount() == 10)
                {
                    successLine->setColor(Color3B(255,240,0));
                }
                else
                {
                    successLine->setColor(Color3B(0,225,255));
                }
                
                auto lbOperator = MafNode::createBmFont("X", GAME_BM_FONT_AL, 20);
                lbOperator->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                lbOperator->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
                lbOperator->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height);
                cell->addChild(lbOperator);
                
            }
            if(isLeftFail)
                isLeftFail = false;
        }
        
        auto lbValue = MafNode::createBmFont(stdValue, GAME_BM_FONT_AL, 20);
        lbValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbValue->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        lbValue->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height + Vec2::RIGHT * icon->getContentSize().width/2);
        cell->addChild(lbValue);
        
        if(stoneType == E_DF_AMP_STONE_Type::BLACK)
            isLeftFail = true;
        
        iconLocation += Vec2::RIGHT * (icon->getContentSize().width + 15);
    }
    
    return cell;
}

ssize_t PopupManaAMPList::numberOfCellsInTableView(extension::TableView *table)
{
    auto floorList = DefenseManaAMPManager::getInstance()->getAMPOpenFloorList();
    return floorList.size();
}


#pragma mark -init
bool PopupManaAMPList::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    initVar();
    initUI();
    
    DefenseManaAMPManager::getInstance()->_onServerInfo += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupManaAMPList::onServerResponse, this), this);
    DefenseManaAMPManager::getInstance()->_onServerUpdateAMP += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupManaAMPList::onServerResponse, this), this);
    DefenseManaAMPManager::getInstance()->_onServerEquipStone += new MafFunction<void(bool, int, bool, E_DF_AMP_STONE_Type)>([=](bool bResult, int nResult, bool, E_DF_AMP_STONE_Type){
        onServerResponse(bResult, nResult);
    }, this);
    
    return true;
}

void PopupManaAMPList::onEnter()
{
    PopupBase::onEnter();
    
    DefenseManager::getInstance()->requestUpdateInfo(true, [=](bool, int){
        DefenseManaAMPManager::getInstance()->requestInfo();
    });
}

void PopupManaAMPList::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupManaAMPList::initUI()
{
    // title
    Size size = Size(750, 1170);
    
    auto lyParent = LayerColor::create();
    lyParent->setContentSize(size);
    lyParent->setIgnoreAnchorPointForPosition(false);
    lyParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lyParent->setPosition(getContentSize().width/2, 0);
    addChild(lyParent);
    
    auto sprBG = Sprite::create("Assets/ui/defense/Horse_power_amplifier_popup_box.png");
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(3);
    sprBG->setPosition(lyParent->getContentSize()/2);
    lyParent->addChild(sprBG);
    
    
    // UI
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(lyParent->getContentSize().width, 90));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(lyParent->getContentSize().width/2, lyParent->getContentSize().height);
    lyParent->addChild(_lyTop);
    
    _lyTopInfo = LayerColor::create();
    _lyTopInfo->setContentSize(Size(lyParent->getContentSize().width, 67));
    _lyTopInfo->setIgnoreAnchorPointForPosition(false);
    _lyTopInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopInfo->setPosition(_lyTop->getPosition() + Vec2::DOWN * (_lyTop->getContentSize().height));
    lyParent->addChild(_lyTopInfo);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(lyParent->getContentSize().width, 105));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(lyParent->getContentSize().width/2, 0);
    lyParent->addChild(_lyBottom);
    
    _lyMiddle = LayerColor::create();
    _lyMiddle->setContentSize(Size(706, lyParent ->getContentSize().height - _lyTop->getContentSize().height - _lyBottom->getContentSize().height - _lyTopInfo->getContentSize().height));
    _lyMiddle->setIgnoreAnchorPointForPosition(false);
    _lyMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyMiddle->setPosition(_lyTopInfo->getPosition() + Vec2::DOWN * (_lyTopInfo->getContentSize().height));
    lyParent->addChild(_lyMiddle);
    
    uiTop();
    uiTopInfo();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupManaAMPList::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_lyTop->getContentSize().width / 2, _lyTop->getContentSize().height/2) );
    _lyTop->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyTop->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/defense/Btn_defense_close.png"), nullptr, CC_CALLBACK_1(PopupManaAMPList::onClickClose, this));
    itemClose->setPosition( Vec2(_lyTop->getContentSize().width - 118, _lyTop->getContentSize().height/2) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}
void PopupManaAMPList::uiTopInfo()
{
    _lyTopInfo->removeAllChildren();
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_select"), GAME_FONT, 26);
    lbTitle->setPosition( Vec2(_lyTopInfo->getContentSize().width / 2, _lyTopInfo->getContentSize().height/2 + 7) );
    lbTitle->setDimensions(450, lbTitle->getContentSize().height);
    lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopInfo->addChild(lbTitle);
}
void PopupManaAMPList::uiTable()
{
    _lyMiddle->removeAllChildren();
    
    auto lyEffectInfo = LayerColor::create();
    lyEffectInfo->setContentSize( Size(_lyMiddle->getContentSize().width, _lyMiddle->getContentSize().height - 10));
    lyEffectInfo->setIgnoreAnchorPointForPosition(false);
    lyEffectInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyEffectInfo->setPosition( Vec2(_lyMiddle->getContentSize().width / 2, 20));
    _lyMiddle->addChild(lyEffectInfo);
    
    _table = extension::TableView::create(this, Size(lyEffectInfo->getContentSize().width, lyEffectInfo->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(lyEffectInfo->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    lyEffectInfo->addChild(_table);
    _table->reloadData();
}

void PopupManaAMPList::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/defense/Horse_power_amplifier_popup_bottom_box.png");
    sprBG->setPosition(_lyBottom->getContentSize()/2);
    _lyBottom->addChild(sprBG);
    
    auto lbPowerText = Label::createWithTTF(GAME_TEXT("t_ui_town_msg_2") , GAME_FONT, 22);
    lbPowerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbPowerText->setPosition(Vec2(40, sprBG->getContentSize().height/2));
    lbPowerText->setDimensions(sprBG->getContentSize().width/2 - 40, lbPowerText->getContentSize().height);
    lbPowerText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbPowerText->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbPowerText);
    
    std::string strEffect = "%s%%";
    
    std::string power = "0";
    for(auto info : DefenseManaAMPManager::getInstance()->getManaAmpList())
    {
        if(!info->isComplete())
            continue;
        
        if(power == "0")
        {
            power = MafUtils::doubleToString(info->getManaAMPPower());
            continue;
        }
        
        auto tt = MafUtils::bigMulNum(power, MafUtils::doubleToString(info->getManaAMPPower()));
        
        if(tt.length() > 0)
            power = MafUtils::bigAddNum(power, tt);
    }
    
    std::string strEffectTmp = MafUtils::bigMulNum(power, "100");
    
    MafUtils::convertNumberToShort(strEffectTmp);
    strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());
    
    auto lbPower = Label::createWithTTF(strEffect, GAME_FONT, 22);
    lbPower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    lbPower->setPosition(Vec2(sprBG->getContentSize().width - 40, sprBG->getContentSize().height/2));
    lbPower->setDimensions(sprBG->getContentSize().width/2, lbPowerText->getContentSize().height);
    lbPower->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
    lbPower->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbPower);
}

void PopupManaAMPList::uiRefresh()
{
    uiTop();
    uiTopInfo();
    uiBottom();
    
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
}
#pragma mark -utils
void PopupManaAMPList::onClickClose(Ref* sender)
{
    hide();
}
void PopupManaAMPList::onClickAMP(Ref* sender)
{
    auto node = (Node*) sender;
    
    auto popup = PopupManaAMPDefault::create(node->getTag());
    popup->_onReddotUpdate += new MafFunction<void(void)>(CC_CALLBACK_0(PopupManaAMPList::uiRefresh, this),this);
    popup->show();
}

void PopupManaAMPList::onServerResponse(bool bResult, int nResult)
{
    if(!bResult || nResult != 1)
    {
        hide();
        return;
    }
    uiRefresh();
}
