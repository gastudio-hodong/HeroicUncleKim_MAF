//
//  MafiOSInAppPlugin.h
//  InAppPurchase
//
//  Created by SanHeo on 2014. 5. 16..
//  Copyright (c) 2014년 Maf. All rights reserved.
//
#import <StoreKit/StoreKit.h>


@interface MafiOSInAppPlugin : NSObject<
///< 상품 정보를 얻어올 때 쓰는 딜리게이트
SKProductsRequestDelegate,
///< 상품 구매 관련 옵저버
SKPaymentTransactionObserver,UIAlertViewDelegate>{

    NSString *strProductTempId;
    NSString *strProductTempIdWatch;
}

@property (retain, nonatomic) NSString *strProductTempId;
@property (retain, nonatomic) NSString *strProductTempIdWatch;
@property (readwrite, nonatomic) BOOL bInitProduct;

+ (MafiOSInAppPlugin*) sharediOSInAppPlugin;

- (void) initVar;
- (BOOL) initProduct;
- (void) requestProductLoad:(NSMutableSet*)listProductId;
- (void) requestProductPurchase:(NSString*)strProductId watchingId:(NSString*)strProductIdWatch;
- (void) requestProductHas:(NSString*)strProductId watchingId:(NSString*)strProductIdWatch;
- (void) requestBuy;
- (void) restoreProductData;


- (void) completeTransaction:(SKPaymentTransaction*)transaction;
- (void) restoreTransaction:(SKPaymentTransaction*)transaction;
- (void) failedTransaction:(SKPaymentTransaction*)transaction;


- (void) consumeTransaction:(NSString*)strProductId watchingId:(NSString*)strProductIdWatch;
- (void) consumeTransactionAll;

- (void) InAppReview;

@end



