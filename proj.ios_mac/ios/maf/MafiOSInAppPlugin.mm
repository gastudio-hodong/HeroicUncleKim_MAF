//
//  MafiOSInAppPlugin.m
//  InAppPurchase
//
//  Created by SanHeo on 2014. 5. 16..
//  Copyright (c) 2014년 Maf. All rights reserved.
//

#import "MafiOSInAppPlugin.h"
#import "MafGooglePlay.h"
#import "MafConfig.h"


@implementation MafiOSInAppPlugin

@synthesize strProductTempId;
@synthesize strProductTempIdWatch;

+ (MafiOSInAppPlugin*) sharediOSInAppPlugin
{
    static MafiOSInAppPlugin* pInstance;
    if(pInstance == NULL)
    {
        pInstance = [[MafiOSInAppPlugin alloc] init];
        [pInstance initVar];
        [pInstance initProduct];
    }
    return pInstance;
}

- (void) initVar {
    self.bInitProduct = NO;
    self.strProductTempId = nil;
    self.strProductTempIdWatch = nil;
}

- (BOOL) initProduct
{
    ///< 인앱 결제 시스템을 사용 가능한지 체크
    if( [SKPaymentQueue canMakePayments] == NO )
    {
        return NO;
    }
    
    if ( self.bInitProduct == YES )
    {
        return true;
    }
    
    ///< Product 결제 진행에 필요한 딜리게이트 등록
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    self.bInitProduct = YES;
    
    return true;
}

- (void) requestProductLoad:(NSMutableSet*)listProductId
{
    if (self.bInitProduct == NO)
    {
        return;
    }
    
    SKProductsRequest* request = [[SKProductsRequest alloc]initWithProductIdentifiers:listProductId];
    request.delegate = self;
    [request start];
}

- (void) requestProductPurchase:(NSString*)strProductId watchingId:(NSString*)strProductIdWatch
{
    if (self.bInitProduct == NO)
    {
        MafGooglePlay::CBReceivePurchase(MafGooglePlay::RESULT::FAIL, "", "");
        self.strProductTempId = nil;
        return;
    }
    
    /*
     구매완료후 오류발생으로 결제프로세스가 완료되지 않았을경우가 있으므로 결제시마다 체크한다.
     응답은 paymentQueueRestoreCompletedTransactionsFinished
     체크완료시 requestBuy 호출.
     */
    self.strProductTempId = strProductId;
    self.strProductTempIdWatch = strProductIdWatch;
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void) requestProductHas:(NSString*)strProductId watchingId:(NSString*)strProductIdWatch;
{
    /*
     0 : 오류 없음
     1 : 다른상품 결제 오류 있음.
     2 : 같은상품 결제 오류 있음.
     */
    
    //
    SKPaymentQueue* queue = [SKPaymentQueue defaultQueue];
    
    //
    int nResult = 0;
    if ( queue.transactions.count == 1 )
    {
        SKPaymentTransaction *transaction = queue.transactions.firstObject;
        if ( [[transaction payment].productIdentifier isEqualToString:strProductId] )
        {
            nResult = 2;
        }
        else
        {
            nResult = 1;
        }
    }
    else if ( queue.transactions.count > 1 )
    {
        nResult = 1;
    }
    
    MafGooglePlay::CBReceivePurchaseHas(nResult);
}

- (void) requestBuy
{
    NSSet* productIdentifiers = [NSSet setWithObject:self.strProductTempId];
    SKProductsRequest* request = [[SKProductsRequest alloc]initWithProductIdentifiers:productIdentifiers];
    request.delegate = self;
    [request start];
}

- (void) restoreProductData
{
    self.strProductTempId = nil;
    self.strProductTempIdWatch = nil;
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

///< 아이템 정보 요청 결과 callback
#pragma mark - SKProductsRequestDelegate
- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    for( SKProduct* product in response.products )
    {
        if( product == nil )
        {
            continue;
        }

        NSString *strPriceCurrencyCode = [product.priceLocale objectForKey:NSLocaleCurrencyCode];
        NSString *strPriceCurrencySymbol = [product.priceLocale objectForKey:NSLocaleCurrencySymbol];
        NSString *strPrice = [NSString stringWithFormat:@"%@%@", strPriceCurrencySymbol, product.price];
        
        // 가격 저장
        MafGooglePlay::CBReceivePurchaseLoad(std::string([product.productIdentifier UTF8String]), std::string([strPrice UTF8String]), std::string([strPriceCurrencyCode UTF8String]));
        
        // 결제 진행
        if ( self.strProductTempId != nil && [product.productIdentifier isEqualToString:self.strProductTempId])
        {
            //구매 요청
//            NSLog(@"결제한 아이템의 제목 : %@", product.localizedTitle );
//            NSLog(@"결제한 아이템의 설명 : %@", product.localizedDescription );
//            NSLog(@"결제한 아이템의 가격 : %@", product.price );
//            NSLog(@"결제한 아이템의 ID   : %@", product.productIdentifier );
            SKPayment* payment = [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }
    }
    
    [request release];
}

#pragma mark - SKPaymentTransactionObserver
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray*)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
                
            case SKPaymentTransactionStatePurchasing://< 서버에 거래 처리중
                break;
            case SKPaymentTransactionStatePurchased://< 구매 완료
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed://< 거래 실패 또는 취소
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored://< 재구매
                //NSLog( @"InAppPurchase paymentQueue:SKPaymentTransactionStateRestored" );
                break;
            case SKPaymentTransactionStateDeferred:
                break;
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    MafGooglePlay::CBReceivePurchase(MafGooglePlay::RESULT::FAIL, "", "");
    self.strProductTempId = nil;
    self.strProductTempIdWatch = nil;
}

-(void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    if ( strProductTempId == nil)
    {
        return;
    }
    
    if ( queue.transactions.count == 0 )
    {
        [self requestBuy];
    }
    else
    {
        SKPaymentTransaction *transaction = queue.transactions.firstObject;
        if ( queue.transactions.count == 1 && [[transaction payment].productIdentifier isEqualToString:strProductTempId] )
        {
            [self completeTransaction:transaction];
        }
        else
        {
            MafGooglePlay::CBReceivePurchase(MafGooglePlay::RESULT::FAIL, "", "");
            self.strProductTempId = nil;
            self.strProductTempIdWatch = nil;
        }
    }
}

#pragma mark -
- (void) completeTransaction:(SKPaymentTransaction *)transaction
{
//    NSLog(@"InAppPurchase completeTransaction");
//    NSLog(@"InAppPurchase Transaction Identifier : %@", transaction.transactionIdentifier );
//    NSLog(@"InAppPurchase Transaction Data : %@", transaction.transactionDate );
//    NSString *strReceipt = [[NSString alloc]initWithData:transaction.transactionReceipt encoding:NSUTF8StringEncoding];
//    NSLog(@"InAppPurchase Transaction transaction.transactionReceipt : \n%@", strReceipt );
    
    // Load the receipt from the app bundle.
    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receiptData = [NSData dataWithContentsOfURL:receiptURL];

    if (receiptData == nil)
    {
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
        
        /* No local receipt -- handle the error. */
        MafGooglePlay::CBReceivePurchase(MafGooglePlay::RESULT::FAIL, "", "");
        self.strProductTempId = nil;
        self.strProductTempIdWatch = nil;
    }
    else
    {
        NSString *receiptDataBase64 = [receiptData base64EncodedStringWithOptions:NSDataBase64EncodingEndLineWithLineFeed];
        MafGooglePlay::CBReceivePurchase(MafGooglePlay::RESULT::SUCCESS, "", std::string([receiptDataBase64 UTF8String]));
        self.strProductTempId = nil;
        self.strProductTempIdWatch = nil;
    }
}

- (void) restoreTransaction:(SKPaymentTransaction *)transaction
{
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void) failedTransaction:(SKPaymentTransaction *)transaction
{    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    MafGooglePlay::CBReceivePurchase(MafGooglePlay::RESULT::FAIL, "", "");
    self.strProductTempId = nil;
    self.strProductTempIdWatch = nil;
}
  
- (void) consumeTransaction:(NSString*)strProductId watchingId:(NSString*)strProductIdWatch;
{
    for (SKPaymentTransaction* transactionTemp in [[SKPaymentQueue defaultQueue] transactions] )
    {
        if ( [[transactionTemp payment].productIdentifier isEqualToString:strProductId] )
        {
            [[SKPaymentQueue defaultQueue] finishTransaction:transactionTemp];
        }
    }
}
 
- (void) consumeTransactionAll;
{
    for (SKPaymentTransaction* transactionTemp in [[SKPaymentQueue defaultQueue] transactions] )
    {
        [[SKPaymentQueue defaultQueue] finishTransaction:transactionTemp];
    }
}

- (void) InAppReview
{
    if (@available(iOS 10.3, *))
    {
       [SKStoreReviewController requestReview];
    }
    else
    {
        MafGooglePlay::CBReceiveInAppReview(MafGooglePlay::RESULT::FAIL);
    }
}

@end
