package com.maf.iab;

import android.app.Activity;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.lifecycle.MutableLiveData;


import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.ConsumeParams;
import com.android.billingclient.api.ConsumeResponseListener;
import com.android.billingclient.api.ProductDetails;
import com.android.billingclient.api.ProductDetailsResponseListener;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchasesResponseListener;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.QueryProductDetailsParams;
import com.android.billingclient.api.QueryPurchasesParams;

import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsParams;
import com.android.billingclient.api.SkuDetailsResponseListener;
import com.google.common.collect.ImmutableList;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Currency;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class InAppPurchase implements PurchasesUpdatedListener
{
    public interface ResultInAppPurchase{
        public abstract void result(final int nResult, final String strOriginalJson, final String strSignature);
    }

    public interface ResultInAppPurchaseLoad {
        public abstract void result(final String strProductID, final String strPrice, final String strPriceCurrencyCode);
    }

    public interface ResultInAppPurchaseHas{
        public abstract void result(final int nResult);
    }


    private Activity mActivity = null;

    private BillingClient mBillingClient = null;
    private MutableLiveData<Map<String, ProductDetails>> listProductDetails = new MutableLiveData<>();

    private ResultInAppPurchase mResultPurchaseListener = null;
    private ResultInAppPurchaseLoad mResultPurchaseLoadListener = null;
    private ResultInAppPurchaseHas mResultPurchaseHasListener = null;


    private boolean mIsServiceConnected;
    private String mProductId = "";
    

    public InAppPurchase(Activity activity, ResultInAppPurchase resultPurchaseListener, ResultInAppPurchaseLoad resultPurchaseLoadListener, ResultInAppPurchaseHas resultPurchaseHasListener) {

        mActivity = activity;

        mResultPurchaseListener = resultPurchaseListener;
        mResultPurchaseLoadListener = resultPurchaseLoadListener;
        mResultPurchaseHasListener = resultPurchaseHasListener;

        mBillingClient = BillingClient.newBuilder(mActivity)
                .enablePendingPurchases()
                .setListener(this)
                .build();

        mBillingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(@NonNull BillingResult billingResult) {
                mIsServiceConnected = false;
                if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    mIsServiceConnected = true;
                }
            }

            @Override
            public void onBillingServiceDisconnected() {
                mIsServiceConnected = false;
            }
        });
    }

    public void destroy() {

        if (mBillingClient != null && mBillingClient.isReady()) {
            mBillingClient.endConnection();
            mBillingClient = null;
        }
    }

    private void startServiceConnection(final Runnable executeOnSuccess) {

        mBillingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(@NonNull BillingResult billingResult) {
                if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    mIsServiceConnected = true;
                    if (executeOnSuccess != null) {
                        executeOnSuccess.run();
                    }
                }
                else {
                    mResultPurchaseListener.result(-1, "","");
                }
            }

            @Override
            public void onBillingServiceDisconnected() {
                mResultPurchaseListener.result(-1, "","");
            }
        });
    }

    private void executeServiceConnection(Runnable runnable) {

        if (mIsServiceConnected) {
            runnable.run();
        } else {
            startServiceConnection(runnable);
        }
    }

    public void onPurchaseLoad(final String[] listProductId) {

        Runnable executeOnSuccess = new Runnable() {

            @Override
            public void run() {

                BillingResult resultFeatureSupported = mBillingClient.isFeatureSupported( BillingClient.FeatureType.PRODUCT_DETAILS );
                if ( resultFeatureSupported.getResponseCode() == BillingClient.BillingResponseCode.OK ) {
                    // Product list add
                    ArrayList<QueryProductDetailsParams.Product> products = new ArrayList<>();
                    if ( listProductId.length > 0 ) {
                        for(String productId : listProductId) {
                            products.add(QueryProductDetailsParams.Product.newBuilder().setProductId(productId).setProductType(BillingClient.ProductType.INAPP).build());
                        }
                    } else {
                        return;
                    }

                    ImmutableList<QueryProductDetailsParams.Product> listProduct = ImmutableList.of();
                    listProduct = ImmutableList.copyOf(products);

                    //
                    QueryProductDetailsParams queryProductDetailsParams = QueryProductDetailsParams.newBuilder().setProductList(listProduct).build();
                    mBillingClient.queryProductDetailsAsync(queryProductDetailsParams, new ProductDetailsResponseListener() {
                        @Override
                        public void onProductDetailsResponse(@NonNull BillingResult billingResult, @NonNull List<ProductDetails> list) {
                            if ( billingResult != null && billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK ) {

                                if ( list == null ) {
                                    listProductDetails.postValue(Collections.<String, ProductDetails>emptyMap());
                                } else {
                                    Map<String, ProductDetails> listProductDetailsNew = new HashMap<String, ProductDetails>();
                                    for (ProductDetails productDetails : list) {
                                        listProductDetailsNew.put(productDetails.getProductId(), productDetails);

                                        // price
                                        String currencyCode = productDetails.getOneTimePurchaseOfferDetails().getPriceCurrencyCode();
                                        String currencySymbol = Currency.getInstance(currencyCode).getSymbol();

                                        String price = "";
                                        if ( productDetails.getOneTimePurchaseOfferDetails().getPriceAmountMicros() % 1000000 > 0 )
                                        {
                                            double priceAmount = Double.parseDouble(String.valueOf(productDetails.getOneTimePurchaseOfferDetails().getPriceAmountMicros())) / 1000000.0;
                                            price = currencySymbol + priceAmount;
                                        }
                                        else
                                        {
                                            long priceAmount = productDetails.getOneTimePurchaseOfferDetails().getPriceAmountMicros() / 1000000;
                                            price = currencySymbol + priceAmount;
                                        }

                                        //
                                        mResultPurchaseLoadListener.result(productDetails.getProductId(), price, currencyCode);
                                    }
                                    listProductDetails.postValue(listProductDetailsNew);
                                }
                            } else {
                                // error 처리
                            }
                        }
                    });
                } else {
                    // Product list add
                    List<String> skuList = new ArrayList<>();
                    skuList.addAll(Arrays.asList(listProductId));

                    SkuDetailsParams skuDetailsParams = SkuDetailsParams.newBuilder().setSkusList(skuList).setType(BillingClient.SkuType.INAPP).build();
                    mBillingClient.querySkuDetailsAsync(skuDetailsParams, new SkuDetailsResponseListener() {
                        @Override
                        public void onSkuDetailsResponse(@NonNull BillingResult billingResult, @Nullable List<SkuDetails> list) {
                            if ( list == null )
                            {
                                return;
                            }

                            for (SkuDetails skyDatails : list) {
                                String currencyCode = skyDatails.getPriceCurrencyCode();
                                String currencySymbol = Currency.getInstance(currencyCode).getSymbol();

                                String price = "";
                                if ( skyDatails.getPriceAmountMicros() % 1000000 > 0 )
                                {
                                    double priceAmount = Double.parseDouble(String.valueOf(skyDatails.getPriceAmountMicros())) / 1000000.0;
                                    price = currencySymbol + priceAmount;
                                }
                                else
                                {
                                    long priceAmount = skyDatails.getPriceAmountMicros() / 1000000;
                                    price = currencySymbol + priceAmount;
                                }

                                mResultPurchaseLoadListener.result(skyDatails.getSku(), price, skyDatails.getPriceCurrencyCode());
                            }
                        }
                    });
                }
            }
        };

        executeServiceConnection(executeOnSuccess);
    }

    public void onPurchase(final String productId) {

        mProductId = productId;

        Runnable executeOnSuccess = new Runnable() {
            @Override
            public void run() {

                QueryPurchasesParams queryPurchasesParams = QueryPurchasesParams.newBuilder().setProductType(BillingClient.ProductType.INAPP).build();

                mBillingClient.queryPurchasesAsync(queryPurchasesParams, new PurchasesResponseListener() {
                    @Override
                    public void onQueryPurchasesResponse(@NonNull BillingResult billingResult, @NonNull List<Purchase> list) {

                        if ( billingResult != null && billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK && list != null ) {

                            for ( final Purchase purchase : list ) {
                                if ( purchase.getProducts().contains(productId.toLowerCase()) == true ) {

                                    BillingResult billingResultTemp = BillingResult.newBuilder().setResponseCode(BillingClient.BillingResponseCode.OK).build();
                                    onPurchasesUpdated(billingResultTemp, list);
                                    return;
                                }
                            }
                        }

                        BillingResult resultFeatureSupported = mBillingClient.isFeatureSupported( BillingClient.FeatureType.PRODUCT_DETAILS );
                        if ( resultFeatureSupported.getResponseCode() == BillingClient.BillingResponseCode.OK ) {
                            //
                            ProductDetails productDetails = null;
                            if (listProductDetails.getValue() != null) {
                                productDetails = listProductDetails.getValue().get(productId);
                            }

                            if (productDetails == null) {
                                mResultPurchaseListener.result(-1, "", "");
                                return;
                            }

                            List<BillingFlowParams.ProductDetailsParams> listParams = new ArrayList<>();
                            listParams.add(BillingFlowParams.ProductDetailsParams.newBuilder().setProductDetails(productDetails).build());

                            BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder().setProductDetailsParamsList(listParams).build();
                            BillingResult resultBillingFlow = mBillingClient.launchBillingFlow(mActivity, billingFlowParams);
                            if ( resultBillingFlow.getResponseCode() !=  BillingClient.BillingResponseCode.OK ) {
                                mResultPurchaseListener.result(-1, "", "");
                            }
                        } else {
                            //
                            List<String> skuList = new ArrayList<>();
                            skuList.add(productId);

                            SkuDetailsParams skuDetailsParams = SkuDetailsParams.newBuilder()
                                    .setSkusList(skuList)
                                    .setType(BillingClient.SkuType.INAPP)
                                    .build();

                            mBillingClient.querySkuDetailsAsync(skuDetailsParams, new SkuDetailsResponseListener() {
                                @Override
                                public void onSkuDetailsResponse(@NonNull BillingResult billingResult, @Nullable List<SkuDetails> list) {
                                    if ( list == null )
                                    {
                                        return;
                                    }

                                    for (SkuDetails skyDatails : list) {
                                        if ( skyDatails.getSku().equals(productId) == true) {
                                            BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                                                    .setSkuDetails(skyDatails)
                                                    .build();

                                            mBillingClient.launchBillingFlow(mActivity, billingFlowParams);
                                            return;
                                        }
                                    }

                                    mResultPurchaseListener.result(-1, "", "");
                                }
                            });
                        }
                    }
                });
            }
        };

        executeServiceConnection(executeOnSuccess);
	}

    public void onPurchaseHas(final String productId) {

        /*
        0 : 오류 없음
        1 : 다른상품 결제 오류 있음.
        2 : 같은상품 결제 오류 있음.
        */
        Runnable executeOnSuccess = new Runnable() {

            @Override
            public void run() {
                QueryPurchasesParams queryPurchasesParams = QueryPurchasesParams.newBuilder().setProductType(BillingClient.ProductType.INAPP).build();

                mBillingClient.queryPurchasesAsync(queryPurchasesParams, new PurchasesResponseListener() {
                    @Override
                    public void onQueryPurchasesResponse(@NonNull BillingResult billingResult, @NonNull List<Purchase> list) {

                        int nResult = 0;
                        if ( billingResult != null && billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK && list != null ) {

                            if ( list.size() > 0 ) {
                                nResult = 1;
                            }

                            for ( final Purchase purchase : list ) {
                                if ( purchase.getProducts().contains(productId.toLowerCase()) == true ) {
                                    nResult = 2;
                                    break;
                                }
                            }
                        }

                        mResultPurchaseHasListener.result(nResult);
                    }
                });
            }
        };
        executeServiceConnection(executeOnSuccess);
    }

    @Override
    public void onPurchasesUpdated(@NonNull BillingResult billingResult, @Nullable List<Purchase> list) {

        if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK && list != null) {
            //결제가 요청이 완료되면 호출되는 영역
            Log.d("Test", "Test BillingClient.BillingResponseCode.OK : " + mProductId);
            for (Purchase purchase : list) {
                //주문 정보 를 받아와서 처리하는 부분
                Log.d("Test", "Test List : " + purchase.getProducts().toString());
                if ( purchase.getProducts().contains(mProductId.toLowerCase()) == true ) {
                    mResultPurchaseListener.result(1, purchase.getOriginalJson(), purchase.getSignature());
                    return;
                }
            }

            mResultPurchaseListener.result(-1, "", "");
        } else if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.USER_CANCELED) {
            // 유저가 도중에 결제를 취소할 경우 호출되는 영역
            mResultPurchaseListener.result(0, "", "");
        } else if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED) {
            //
            mResultPurchaseListener.result(-1, "", "");
        } else {
            // 예기치 못한 결제 에러가 발생 되면 호출되는 영역
            mResultPurchaseListener.result(-1, "", "");
        }

    }

    public void consumePurchase(final String productId) {

        Runnable executeOnSuccess = new Runnable() {
            @Override
            public void run() {
                QueryPurchasesParams queryPurchasesParams = QueryPurchasesParams.newBuilder().setProductType(BillingClient.ProductType.INAPP).build();
                mBillingClient.queryPurchasesAsync(queryPurchasesParams, new PurchasesResponseListener() {
                    @Override
                    public void onQueryPurchasesResponse(@NonNull BillingResult billingResult, @NonNull List<Purchase> list) {
                        if ( billingResult != null && billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK && list != null ) {

                            for ( final Purchase purchase : list ) {
                                if ( purchase.getProducts().contains(productId.toLowerCase()) == false ) {
                                    continue;
                                }

                                ConsumeParams consumeParams = ConsumeParams.newBuilder().setPurchaseToken(purchase.getPurchaseToken()).build();
                                mBillingClient.consumeAsync(consumeParams, new ConsumeResponseListener() {
                                    @Override
                                    public void onConsumeResponse(@NonNull BillingResult billingResult, @NonNull String s) {

                                    }
                                });
                            }
                        }
                    }
                });
            }
        };
        executeServiceConnection(executeOnSuccess);
    }

    public void consumePurchaseAll() {
        Runnable executeOnSuccess = new Runnable() {

            @Override
            public void run() {
                QueryPurchasesParams queryPurchasesParams = QueryPurchasesParams.newBuilder().setProductType(BillingClient.ProductType.INAPP).build();
                mBillingClient.queryPurchasesAsync(queryPurchasesParams, new PurchasesResponseListener() {
                    @Override
                    public void onQueryPurchasesResponse(@NonNull BillingResult billingResult, @NonNull List<Purchase> list) {
                        if ( billingResult != null && billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK && list != null ) {

                            for ( final Purchase purchase : list ) {
                                ConsumeParams consumeParams = ConsumeParams.newBuilder().setPurchaseToken(purchase.getPurchaseToken()).build();
                                mBillingClient.consumeAsync(consumeParams, new ConsumeResponseListener() {
                                    @Override
                                    public void onConsumeResponse(@NonNull BillingResult billingResult, @NonNull String s) {

                                    }
                                });
                            }
                        }
                    }
                });
            }
        };
        executeServiceConnection(executeOnSuccess);
    }
}