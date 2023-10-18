//
//  PopupCostumeReview.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCostumeReview.h"

#include "ManagerGame/UserInfoManager.h"
#include "GameObject/InfoReviewCostume.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeManager.h"

USING_NS_CC;

PopupCostumeReview* PopupCostumeReview::create(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    PopupCostumeReview* pRet = new(std::nothrow) PopupCostumeReview();
    if ( pRet && pRet->init(eType, eIdx) )
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

PopupCostumeReview::PopupCostumeReview(void) :
_eType(E_COSTUME::HEAD),
_eIdx(E_COSTUME_IDX::IDX_1_DEFAULT),

_isRequestReviewList(false),
_isRequestReviewListWaiting(false),
_nListReviewIdx(0),
_strLanguage("en"),

_layerReveiwList(nullptr),
_layerReveiwMy(nullptr),
_labelListNothing(nullptr),
_cbRating(nullptr),
_edit(nullptr),
_table(nullptr)
{
    
}

PopupCostumeReview::~PopupCostumeReview(void)
{
    
}

bool PopupCostumeReview::init(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // costume
    _eType = eType;
    _eIdx = eIdx;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupCostumeReview::onEnter()
{
    PopupBase::onEnter();
    
    requestList();
    requestBest();
    requestMyReviewInfo();
    requestMyRecommendList();
}

#pragma mark - init
void PopupCostumeReview::initVar()
{
    _objReviewMy = InfoReviewCostume::create();
    _objReviewMy->retain();
    
    _strLanguage = TextManager::getInstance()->getLang();
    if ( (_strLanguage.compare("ko") == 0 || _strLanguage.compare("en") == 0 || _strLanguage.compare("jp") == 0) == false )
    {
        _strLanguage = "en";
    }
}

void PopupCostumeReview::initUi()
{
    Size size = Size(730, 800);
    
    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(20);
    
    //
    auto objCostume = CostumeManager::getInstance()->getCostume(_eType, _eIdx);
    auto labelTitle = Label::createWithTTF(objCostume->getTitle(), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCostumeReview::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    // ui
    uiReviewMy();
    uiReviewList();
}

#pragma mark - table
void PopupCostumeReview::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    const Vec2 minOffset = view->minContainerOffset();
    const Vec2 maxOffset = view->maxContainerOffset();
    
    if ( view->isDragging() == true )
    {
        if ( minOffset.y < 0 && view->getContentOffset().y >= 150 )
        {
            _isRequestReviewListWaiting = true;
        }
        else if ( minOffset.y >= 0 && view->getContentOffset().y >= minOffset.y + 150 )
        {
            _isRequestReviewListWaiting = true;
        }
        else
        {
            _isRequestReviewListWaiting = false;
        }
    }
    else
    {
        if ( _isRequestReviewListWaiting == true )
        {
            _isRequestReviewListWaiting = false;
            
            Vec2 offset = view->getContentOffset();
            offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
            offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
            
            _table->setContentOffset(offset);
            
            // network
            requestList();
        }
    }
}

void PopupCostumeReview::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupCostumeReview::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 94);
}

extension::TableViewCell* PopupCostumeReview::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    // obj
    auto obj = (InfoReviewCostume*)_listReview.at(idx);
    
    
    auto spriteBg = ui::Scale9Sprite::create("Assets/ui/review/review_list_box.png", Rect::ZERO, Rect(0,0,10,10));
    spriteBg->setContentSize(Size(size.width, spriteBg->getContentSize().height));
    spriteBg->setPosition( Vec2(size.width / 2, size.height / 2) );
    cell->addChild(spriteBg);
    
    // 평가 점수
    auto layerRating = Layer::create();
    layerRating->setContentSize( Size(obj->getRating() * 21, 21) );
    layerRating->setIgnoreAnchorPointForPosition(false);
    layerRating->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerRating->setPosition( Vec2(70, spriteBg->getContentSize().height / 2) );
    spriteBg->addChild(layerRating);
    for ( int i = 0; i < obj->getRating(); i++ )
    {
        auto sprite = Sprite::create("Assets/ui/review/review_star.png");
        sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        sprite->setPosition( Vec2(i * 21, layerRating->getContentSize().height / 2) );
        layerRating->addChild(sprite);
    }
 
    
    // 내용
    auto labelContents = Label::createWithSystemFont(obj->getContents(), SYSTEM_FONT, 22);
    labelContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
    labelContents->setPosition( Vec2(150, spriteBg->getContentSize().height / 2) );
    labelContents->setDimensions(460, 0);
    labelContents->setAlignment(TextHAlignment::LEFT,TextVAlignment::CENTER);
    labelContents->setLineSpacing(-5);
    spriteBg->addChild(labelContents);
    if ( obj->getBest() == true )
    {
        auto spriteBest = Sprite::create("Assets/ui/review/review_best.png");
        spriteBest->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        spriteBest->setPosition( Vec2(150, spriteBg->getContentSize().height - 5) );
        spriteBg->addChild(spriteBest);
        
        labelContents->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        labelContents->setPosition( Vec2(150, spriteBest->getPositionY() - spriteBest->getContentSize().height) );
        
    }
    
    // recommend count
    std::string strRecommend = MafUtils::toString(obj->getCountRecommend());
    strRecommend = MafUtils::convertNumberToShort(strRecommend);
    
    auto labelRecommend = Label::createWithTTF(strRecommend, GAME_FONT, 24);
    labelRecommend->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    labelRecommend->setPosition( Vec2(spriteBg->getContentSize().width - 60, spriteBg->getContentSize().height / 2 - 10) );
    spriteBg->addChild(labelRecommend);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    spriteBg->addChild(menu);
    
    // item recommend
    std::string strPathRecommendIcon = "Assets/ui/review/review_like_1.png";
    auto iter = std::find(_listMyRecommend.begin(), _listMyRecommend.end(), Value(obj->getIdx()));
    if (iter != _listMyRecommend.end())
    {
        strPathRecommendIcon = "Assets/ui/review/review_like_2.png";
    }
    
    auto itemRecommend = MafNode::MafMenuItemSprite::create(Sprite::create(strPathRecommendIcon), nullptr, CC_CALLBACK_1(PopupCostumeReview::onClickReviewRecommend, this));
    itemRecommend->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    itemRecommend->setPosition( Vec2(spriteBg->getContentSize().width - 60, spriteBg->getContentSize().height / 2 - 8) );
    itemRecommend->setTag((int)idx);
    itemRecommend->setTouchRect( Size(itemRecommend->getContentSize().width * 2.0f, itemRecommend->getContentSize().height * 1.5f) );
    menu->addChild(itemRecommend);
    
    return cell;
}

ssize_t PopupCostumeReview::numberOfCellsInTableView(extension::TableView *table)
{
    return _listReview.size();
}

#pragma mark - ui
void PopupCostumeReview::uiReviewList()
{
    if ( _layerReveiwList == nullptr )
    {
        float width = 720;
        float height = _spriteContainer->getContentSize().height - 75 - _layerReveiwMy->getContentSize().height;
        height -= 12; // padding
        
        _layerReveiwList = Layer::create();
        _layerReveiwList->setContentSize( Size(width, height) );
        _layerReveiwList->setIgnoreAnchorPointForPosition(false);
        _layerReveiwList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerReveiwList->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _layerReveiwMy->getContentSize().height) );
        _spriteContainer->addChild(_layerReveiwList, PopupCostumeReview::zorder_layer_list);

        auto spriteTop = ui::Scale9Sprite::create("Assets/ui/review/review_list_category.png", Rect::ZERO, Rect(0,0,10,10));
        spriteTop->setContentSize(Size(_layerReveiwList->getContentSize().width, spriteTop->getContentSize().height));
        spriteTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        spriteTop->setPosition( Vec2(_layerReveiwList->getContentSize().width / 2, _layerReveiwList->getContentSize().height) );
        _layerReveiwList->addChild(spriteTop);
        {
            auto labelStar = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_score"), GAME_FONT, 24);
            labelStar->setPosition( Vec2(75, spriteTop->getContentSize().height / 2) );
            spriteTop->addChild(labelStar);
            
            auto labelComment = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_comment"), GAME_FONT, 24);
            labelComment->setPosition( Vec2(360, spriteTop->getContentSize().height / 2) );
            spriteTop->addChild(labelComment);
            
            auto labelRecommend = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_recommend"), GAME_FONT, 24);
            labelRecommend->setPosition( Vec2(670, spriteTop->getContentSize().height / 2) );
            labelRecommend->setDimensions(110, labelRecommend->getContentSize().height);
            labelRecommend->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelRecommend->setOverflow(Label::Overflow::SHRINK);
            spriteTop->addChild(labelRecommend);
            
        }

        // list table
        _table = extension::TableView::create(this, Size(_layerReveiwList->getContentSize().width, _layerReveiwList->getContentSize().height - spriteTop->getContentSize().height - 5));
        _table->setDirection(extension::ScrollView::Direction::VERTICAL);
        _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
        _table->setIgnoreAnchorPointForPosition(false);
        _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _table->setPosition( Vec2(_layerReveiwList->getContentSize().width / 2, 0) );
        _table->setDelegate(this);
        _layerReveiwList->addChild(_table);

        // label nothing
        _labelListNothing = Label::createWithTTF(GAME_TEXT("t_ui_msg_nodata"), GAME_FONT, 24);
        _labelListNothing->setPosition( Vec2(_layerReveiwList->getContentSize().width / 2, _layerReveiwList->getContentSize().height / 2) );
        _labelListNothing->setColor(Color3B::WHITE);
        _layerReveiwList->addChild(_labelListNothing);
    }

    // table load
    Vec2 a = _table->maxContainerOffset();
    Vec2 b = _table->minContainerOffset();

    if ( _table->getContentOffset().y <= 0 )
    {
        Vec2 pos = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(pos);

        const Vec2 minOffset = _table->minContainerOffset();
        const Vec2 maxOffset = _table->maxContainerOffset();
        Vec2 offset = _table->getContentOffset();
        offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
        offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
        _table->setContentOffset(offset);
    }
    else
    {
        _table->reloadData();
    }

    // label empty
    if ( _listReview.size() != 0 )
    {
        _labelListNothing->setVisible(false);
    }
    else
    {
        _labelListNothing->setVisible(true);
    }
}

void PopupCostumeReview::uiReviewMy()
{
    if ( _layerReveiwMy == nullptr )
    {
        _layerReveiwMy = Layer::create();
        _layerReveiwMy->setContentSize( Size(720, 90) );
        _layerReveiwMy->setIgnoreAnchorPointForPosition(false);
        _layerReveiwMy->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerReveiwMy->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 0) );
        _spriteContainer->addChild(_layerReveiwMy, PopupCostumeReview::zorder_layer_my);
    }
    _layerReveiwMy->removeAllChildren();
    
    // ComboBox
    ValueVector listRating;
    for ( int i = 1; i <= 5; i++ )
    {
        listRating.push_back(Value(i));
    }
    _cbRating = UIComboBox::create(listRating, UIComboBox::eType::type_up, "Assets/ui/review/review_grade_box.png", "Assets/ui/review/review_grade_listbox.png");
    _cbRating->setIconArrow("Assets/ui/review/review_grade_up.png", "Assets/ui/review/review_grade_down.png");
    _cbRating->setDataSelect("3");
    _cbRating->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
    _cbRating->setPosition( Vec2(120, _layerReveiwMy->getContentSize().height / 2) );
    _cbRating->setEnable(true);
    _layerReveiwMy->addChild(_cbRating);
  

    // EditBox
    _edit = ui::EditBox::create(Size(448, 54), "Assets/ui/review/review_commentbox.png");
    _edit->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
    _edit->setPosition(Vec2(130, _layerReveiwMy->getContentSize().height / 2));
    _edit->setMaxLength(40);
    _edit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _edit->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    _edit->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
    _edit->setEnabled(true);
    _edit->setFontColor(Color3B::WHITE);
    _edit->setPlaceholderFont(GAME_FONT, 24);
    _edit->setFont(GAME_FONT, 24);
    _edit->setPlaceHolder(GAME_TEXT("t_ui_costume_review_msg_1").c_str());
    _layerReveiwMy->addChild(_edit);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerReveiwMy->addChild(menu);
    
    // item send
    auto itemSend = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/review/btn_review_send.png"), nullptr, CC_CALLBACK_1(PopupCostumeReview::onClickReviewRegister, this));
    itemSend->setPosition( Vec2(_layerReveiwMy->getContentSize().width - 65, _layerReveiwMy->getContentSize().height / 2) );
    itemSend->setVisible(true);
    menu->addChild(itemSend);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_register"), GAME_FONT, 24);
        label->setPosition( Vec2(itemSend->getContentSize().width / 2, itemSend->getContentSize().height / 2) );
        itemSend->addChild(label);
    }
    
    // item Cancel
    auto itemCancel = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/review/btn_review_sendcancel.png"), nullptr, CC_CALLBACK_1(PopupCostumeReview::onClickReviewUnRegister, this));
    itemCancel->setPosition( Vec2(_layerReveiwMy->getContentSize().width - 65, _layerReveiwMy->getContentSize().height / 2) );
    itemCancel->setVisible(false);
    menu->addChild(itemCancel);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_cancel"), GAME_FONT, 24);
        label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        label->setPosition( Vec2(itemCancel->getContentSize().width / 2, 5) );
        itemCancel->addChild(label);
    }
    
    // item warring
    auto layerWarring = LayerColor::create(Color4B::RED, 658, 58);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_msg_3"), GAME_FONT, 24);
        label->setPosition( Vec2(layerWarring->getContentSize().width / 2, layerWarring->getContentSize().height / 2 ) );
        label->setDimensions(layerWarring->getContentSize().width * 0.8, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        layerWarring->addChild(label);
        
        auto spriteIcon1 = Sprite::create("Assets/ui/review/review_delete.png");
        spriteIcon1->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        spriteIcon1->setPosition( Vec2(0, label->getContentSize().height / 2) );
        label->addChild(spriteIcon1);
        
        auto spriteIcon2 = Sprite::create("Assets/ui/review/review_delete.png");
        spriteIcon2->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        spriteIcon2->setPosition( Vec2(label->getContentSize().width, label->getContentSize().height / 2) );
        label->addChild(spriteIcon2);
        
    }
    
    auto itemWarring = MafNode::MafMenuItemSprite::create(layerWarring, nullptr, CC_CALLBACK_1(PopupCostumeReview::onClickReviewWarring, this));
    itemWarring->setPosition( Vec2(_layerReveiwMy->getContentSize().width / 2, _layerReveiwMy->getContentSize().height / 2) );
    itemWarring->setVisible(false);
    itemWarring->setTouchRect(_layerReveiwMy->getContentSize());
    menu->addChild(itemWarring);
    
    
    // 내가 입력한 리뷰 출력
    if ( _objReviewMy->getIdx() != 0 )
    {
        if ( _objReviewMy->getContents().length() != 0 )
        {
            _edit->setText(_objReviewMy->getContents().c_str());
            _edit->setEnabled(false);
            
            _cbRating->setDataSelect( MafUtils::toString(_objReviewMy->getRating()) );
            _cbRating->setEnable(false);
            
            itemSend->setVisible(false);
            itemCancel->setVisible(true);
        }
        else
        {
            itemWarring->setVisible(true);
        }
        
        
        // recommend count
        auto labelRecommend = Label::createWithTTF(MafUtils::toString(_objReviewMy->getCountRecommend()), GAME_FONT, 24);
        labelRecommend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelRecommend->setPosition( Vec2(itemCancel->getContentSize().width / 2, 55) );
        itemCancel->addChild(labelRecommend);
        
        auto spriteRecommend = Sprite::create("Assets/ui/review/review_like_1.png");
        spriteRecommend->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        spriteRecommend->setPosition( Vec2(itemCancel->getContentSize().width / 2 - 5, 55) );
        itemCancel->addChild(spriteRecommend);
    }
}

#pragma mark - set, get, sort, add
bool PopupCostumeReview::sortList(Ref *a, Ref *b)
{
    InfoReviewCostume *pObj1 = static_cast<InfoReviewCostume *>(a);
    InfoReviewCostume *pObj2 = static_cast<InfoReviewCostume *>(b);
    
    if ( pObj1->getBest() == true || pObj2->getBest() == true )
    {
        return pObj1->getBest() > pObj2->getBest();
    }
    else
    {
        return pObj1->getIdx() > pObj2->getIdx();
    }
}

#pragma mark - click
void PopupCostumeReview::onClickClose(Ref* sender)
{
    hide();
}

void PopupCostumeReview::onClickReviewRegister(cocos2d::Ref* sender)
{
    std::string strRating = _cbRating->getDataSelect();
    std::string strContests = _edit->getText();
    strContests = MafUtils::TrimRight(strContests);
    if ( strContests.length() == 0 )
    {
        return;
    }
    
    MafUtils::eraseCharcter(strContests, ' ');
    if ( TextManager::getInstance()->isBadWord(strContests) == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_nick_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    bool isHave = false;
    if ( _eType == E_COSTUME::SETITEM )
    {
        isHave = CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, _eIdx);
    }
    else
    {
        isHave = CostumeManager::getInstance()->isEquip(_eType, _eIdx);
        if( isHave == false && _eType == E_COSTUME::JEWEL && _eIdx >= E_COSTUME_IDX::IDX_HONOR_1001  )
        {
            int level = UserInfoManager::getInstance()->getHonorLevel((E_HONOR)_eIdx);
            if ( level != 0 )
            {
                isHave = true;
            }
        }
    }
    
    if ( isHave == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_review_msg_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    requestRegister(strRating, strContests);
}

void PopupCostumeReview::onClickReviewUnRegister(cocos2d::Ref* sender)
{
    requestUnRegister();
}

void PopupCostumeReview::onClickReviewRecommend(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    _nListReviewIdx = item->getTag();
    if ( _nListReviewIdx < 0 )
        return;
    
    auto obj = (InfoReviewCostume*)_listReview.at(_nListReviewIdx);
    
    auto iter = std::find(_listMyRecommend.begin(), _listMyRecommend.end(), Value(obj->getIdx()));
    if (iter != _listMyRecommend.end())
    {
        // 추천 취소
        requestUnRecommend();
    }
    else
    {
        // 추천
        requestRecommend();
    }
}

void PopupCostumeReview::onClickReviewWarring(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    item->setVisible(false);
}

#pragma mark - network
void PopupCostumeReview::requestList()
{
    // 추가 리스트 요청
    if ( _isRequestReviewList == true )
        return;
    
    // network
    PopupLoading::show();
    
    _isRequestReviewList = true;
    int nPos = (int)_listReview.size() / 10;
    
    std::string url = "/costume_review/v2/costume/%d/%d/%d/%s";
    url = MafUtils::format(url.c_str(), _eType, _eIdx, nPos, _strLanguage.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseList(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseList(network::HttpResponse* response,std::string &data)
{
    // 추가 리스트 요청
    _isRequestReviewList = false;
    
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_coslist") == false || jsonParser["_coslist"].IsArray() == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser["_coslist"].Size(); i++ )
    {
        const rapidjson::Value& jsonReview = jsonParser["_coslist"][i];
        
        int nIdx = jsonReview["idx"].GetInt();
        std::string strUserid = jsonReview["userid"].GetString();
        std::string strContents = jsonReview["content"].GetString();
        int nRating = jsonReview["rating"].GetInt();
        int nCountRecommend = jsonReview["count_good"].GetInt();
        
        auto obj = InfoReviewCostume::create();
        obj->setIdx(nIdx);
        obj->setUserId(strUserid);
        obj->setContents(strContents);
        obj->setRating(nRating);
        obj->setCountRecommend(nCountRecommend);
        
        // 중복추가 하지 않음
        bool isAlready = false;
        for(auto objIn : _listReview)
        {
            if ( objIn->getIdx() == obj->getIdx() )
            {
                isAlready = true;
                break;
            }
        }
        
        if ( isAlready == false )
        {
            _listReview.pushBack(obj);
        }
    }
    
    // sort
    std::sort(_listReview.begin(), _listReview.end(), CC_CALLBACK_2(PopupCostumeReview::sortList, this));
    
    // show
    uiReviewList();
}

void PopupCostumeReview::requestBest()
{
    PopupLoading::show();
    
    std::string url = "/costume_review/v2/costume/best/%d/%d/%s";
    url = MafUtils::format(url.c_str(), _eType, _eIdx, _strLanguage.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBest(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseBest(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_coslist") == false || jsonParser["_coslist"].IsArray() == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }

    if ( jsonParser["_coslist"].Size() != 0 )
    {
        const rapidjson::Value& jsonReview = jsonParser["_coslist"][0u];
        
        int nIdx = jsonReview["idx"].GetInt();
        std::string strUserid = jsonReview["userid"].GetString();
        std::string strContents = jsonReview["content"].GetString();
        int nRating = jsonReview["rating"].GetInt();
        int nCountRecommend = jsonReview["count_good"].GetInt();
        
        auto obj = InfoReviewCostume::create();
        obj->setBest(true);
        obj->setIdx(nIdx);
        obj->setUserId(strUserid);
        obj->setContents(strContents);
        obj->setRating(nRating);
        obj->setCountRecommend(nCountRecommend);
        
        // 리스트에 있는거 삭제
        for(auto objIn : _listReview)
        {
            if ( objIn->getIdx() == obj->getIdx() )
            {
                _listReview.eraseObject(objIn);
                break;
            }
        }
    
        // 추가
        _listReview.pushBack(obj);
    }
    
    // sort
    std::sort(_listReview.begin(), _listReview.end(), CC_CALLBACK_2(PopupCostumeReview::sortList, this));
    
    // show
    uiReviewList();
}

#pragma mark - network(나의 리뷰 정보)
void PopupCostumeReview::requestMyReviewInfo()
{
    PopupLoading::show();
    
    std::string url = "/costume_review/v2/costume/my/%d/%d/%s/%s";
    url = MafUtils::format(url.c_str(), _eType, _eIdx, AccountManager::getInstance()->getUserID().c_str(), _strLanguage.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMyReviewInfo(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseMyReviewInfo(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_coslist") == false || jsonParser["_coslist"].IsArray() == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    if ( jsonParser["_coslist"].Size() != 0 )
    {
        const rapidjson::Value& jsonReview = jsonParser["_coslist"][0u];
        
        int nIdx = jsonReview["idx"].GetInt();
        std::string strUserid = jsonReview["userid"].GetString();
        std::string strContents = jsonReview["content"].GetString();
        int nRating = jsonReview["rating"].GetInt();
        int nCountRecommend = jsonReview["count_good"].GetInt();
        
        _objReviewMy->setIdx(nIdx);
        _objReviewMy->setUserId(strUserid);
        _objReviewMy->setContents(strContents);
        _objReviewMy->setRating(nRating);
        _objReviewMy->setCountRecommend(nCountRecommend);
    }
    
    uiReviewMy();
}

void PopupCostumeReview::requestMyRecommendList()
{
    PopupLoading::show();
    
    std::string url = "/costume_review/v2/log/costume/recommend/%d/%d/%s/%s";
    url = MafUtils::format(url.c_str(), _eType, _eIdx, AccountManager::getInstance()->getUserID().c_str(), _strLanguage.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMyRecommendList(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseMyRecommendList(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_coslist") == false || jsonParser["_coslist"].IsArray() == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    _listMyRecommend.clear();
    for ( int i = 0; i < jsonParser["_coslist"].Size(); i++ )
    {
        const rapidjson::Value& jsonReview = jsonParser["_coslist"][i];
        
        int nIdx = jsonReview["review_idx"].GetInt();
        _listMyRecommend.push_back(Value(nIdx));
    }
    
    // show
    uiReviewList();
}

#pragma mark - network(리뷰 추천)
void PopupCostumeReview::requestRecommend()
{
    PopupLoading::show();
    
    // obj
    auto obj = (InfoReviewCostume*)_listReview.at(_nListReviewIdx);
    
    // network
    std::string url = "/costume_review/v2/costume/recommend/%d/%s/%s";
    url = MafUtils::format(url.c_str(), obj->getIdx(), AccountManager::getInstance()->getUserID().c_str(), _strLanguage.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseRecommend(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseRecommend(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( bError == true || response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("succss") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    bool isResult = jsonParser["succss"].GetBool();
    if ( isResult == true )
    {
        auto obj = (InfoReviewCostume*)_listReview.at(_nListReviewIdx);
        obj->setCountRecommend(obj->getCountRecommend() + 1);
        
        _listMyRecommend.push_back(Value(obj->getIdx()));
        
        // show
        uiReviewList();
    }
    
}

void PopupCostumeReview::requestUnRecommend()
{
    PopupLoading::show();
    
    // obj
    auto obj = (InfoReviewCostume*)_listReview.at(_nListReviewIdx);
    
    // network
    std::string url = "/costume_review/v2/costume/unrecommend/%d/%s/%s";
    url = MafUtils::format(url.c_str(), obj->getIdx(), AccountManager::getInstance()->getUserID().c_str(), _strLanguage.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUnRecommend(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseUnRecommend(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("succss") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    bool isResult = jsonParser["succss"].GetBool();
    if ( isResult == true )
    {
        auto obj = (InfoReviewCostume*)_listReview.at(_nListReviewIdx);
        obj->setCountRecommend(obj->getCountRecommend()-1);
        
        auto iter = std::find(_listMyRecommend.begin(), _listMyRecommend.end(), Value(obj->getIdx()));
        if (iter != _listMyRecommend.end())
        {
            _listMyRecommend.erase(iter);
        }
            
        // show
        uiReviewList();
    }
}

#pragma mark - network(리뷰 등록)
void PopupCostumeReview::requestRegister(std::string strRating, std::string strContests)
{
    PopupLoading::show();
    
    // network
    std::string url = "/costume_review/v2/costume/post";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_costype", _eType);
    req->setData("_cosnum", _eIdx);
    req->setData("_content", strContests.c_str());
    req->setData("_rating", strRating.c_str());
    req->setData("_lang", _strLanguage.c_str());
    
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseRegister(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseRegister(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_coslist") == false || jsonParser["_coslist"].IsArray() == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    if ( jsonParser["_coslist"].Size() != 0 )
    {
        const rapidjson::Value& jsonReview = jsonParser["_coslist"][0u];
        
        int nIdx = jsonReview["idx"].GetInt();
        std::string strUserid = jsonReview["userid"].GetString();
        std::string strContents = jsonReview["content"].GetString();
        int nRating = jsonReview["rating"].GetInt();
        int nCountRecommend = jsonReview["count_good"].GetInt();
        
        auto obj = InfoReviewCostume::create();
        obj->setIdx(nIdx);
        obj->setUserId(strUserid);
        obj->setContents(strContents);
        obj->setRating(nRating);
        obj->setCountRecommend(nCountRecommend);
        
        _objReviewMy->setIdx(nIdx);
        _objReviewMy->setUserId(strUserid);
        _objReviewMy->setContents(strContents);
        _objReviewMy->setRating(nRating);
        _objReviewMy->setCountRecommend(nCountRecommend);
        
        _listReview.pushBack(obj);

        // sort
        std::sort(_listReview.begin(), _listReview.end(), CC_CALLBACK_2(PopupCostumeReview::sortList, this));
        
        // show
        uiReviewList();
        uiReviewMy();
    
        // best review
        if ( _listReview.size() == 1 )
        {
            requestBest();
        }
    }
}

void PopupCostumeReview::requestUnRegister()
{
    PopupLoading::show();
    
    // network
    std::string url = "/costume_review/v2/costume/delete";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_costype", _eType);
    req->setData("_cosnum", _eIdx);
    req->setData("_lang", _strLanguage.c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUnRegister(response,dd);
    };
    req->send(cb);
}

void PopupCostumeReview::responseUnRegister(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("succss") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    bool isResult = jsonParser["succss"].GetBool();
    if ( isResult == true )
    {
        // 리스트에서 삭제
        for(auto objIn : _listReview)
        {
            if ( objIn->getIdx() == _objReviewMy->getIdx() )
            {
                // 베스트 요청
                if ( objIn->getBest() == true )
                {
                    requestBest();
                }
                // 리스트에서 삭제
                _listReview.eraseObject(objIn);
                break;
            }
        }

        // 내꺼 삭제
        _objReviewMy->release();
        
        // 내꺼 새로 생성
        _objReviewMy = InfoReviewCostume::create();
        _objReviewMy->retain();
        
        // show
        uiReviewList();
        uiReviewMy();
    }
}
