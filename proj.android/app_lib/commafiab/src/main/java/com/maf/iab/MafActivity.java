package com.maf.iab;

import android.Manifest;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.content.res.Resources.NotFoundException;
import android.graphics.Color;
import android.media.AudioManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.provider.Settings;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.text.Html;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.appsflyer.AFInAppEventParameterName;
import com.appsflyer.AFInAppEventType;
import com.appsflyer.AppsFlyerLib;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.tasks.Task;
import com.google.android.play.core.review.ReviewInfo;
import com.google.android.play.core.review.ReviewManager;
import com.google.android.play.core.review.ReviewManagerFactory;
import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;

import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.Player;
import com.google.android.gms.games.PlayersClient;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.dynamiclinks.DynamicLink;
import com.google.firebase.dynamiclinks.FirebaseDynamicLinks;
import com.google.firebase.dynamiclinks.PendingDynamicLinkData;
import com.google.firebase.dynamiclinks.ShortDynamicLink;;
import com.google.firebase.messaging.FirebaseMessaging;
import com.google.firebase.remoteconfig.FirebaseRemoteConfig;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigSettings;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import java.io.IOException;
import java.security.MessageDigest;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;


public class MafActivity extends Cocos2dxActivity  {

	private static FirebaseAnalytics mFirebaseAnalytics;
	private static FirebaseRemoteConfig mFirebaseRemoteConfig;

	static MafActivity mainApp = null;
	public static final String TAG = "MafActivity";

	static AppEventsLogger fbLogger = null;
	static boolean bFacebookInit = false;

	private static InAppPurchase mInAppPurchase = null;
	private static GoogleSignInClient mGoogleSignInClient = null;
	static boolean DEBUG_BUILD = false;

	private static final int RC_SIGN_IN = 9001;
	private static final int RC_GAME = 9002;
	private static final int RC_SEND_SMS = 1000;

	private boolean _isDestroy = false;
	private boolean _isResume = false;
	private long _nSpeedTimePrev = 0;
	private long _nSpeedCount = 0;

	private static boolean _isContinueCreateDeeplink = false;

	private static NotificationChannel notificationChannel;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.setEnableVirtualButton(false);
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		mainApp = this;

		// in app
		mInAppPurchase = new InAppPurchase(mainApp, purchaseResultListener, purchaseResultLoadListener, purchaseResultHasListener);

        // speed heak check
		checkSpeedHeak();

		createNotificationToken();
		createNotificationChannel();
	}

	public static String getKeyHash() {

		String key = "";

		try {
			if(Build.VERSION.SDK_INT >= 28) {
				final PackageInfo packageInfo = mainApp.getPackageManager().getPackageInfo(mainApp.getPackageName(), PackageManager.GET_SIGNING_CERTIFICATES);
				final Signature[] signatures = packageInfo.signingInfo.getApkContentsSigners();
				final MessageDigest md = MessageDigest.getInstance("SHA");
				for (Signature signature : signatures) {
					md.update(signature.toByteArray());
					key = new String(Base64.encode(md.digest(), Base64.DEFAULT));
				}

			}else{
				PackageInfo packageInfo = mainApp.getPackageManager().getPackageInfo(mainApp.getPackageName(), PackageManager.GET_SIGNATURES);
				for (Signature signature : packageInfo.signatures)
				{
					byte[] signatureBytes = signature.toByteArray();
					MessageDigest md = MessageDigest.getInstance("SHA");
					md.update(signature.toByteArray());
					key = Base64.encodeToString(md.digest(), Base64.DEFAULT);
				}
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		key = key.replaceAll("(\r\n|\r|\n|\n\r)", "");
		return key;
	}

	public static boolean CheckPlayServices() {
		int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(mainApp);
		if (resultCode != ConnectionResult.SUCCESS) {
			if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
				GooglePlayServicesUtil.getErrorDialog(resultCode, mainApp,
						PLAY_SERVICES_RESOLUTION_REQUEST).show();
			} else {
				Log.i(TAG, "This device is not supported.");
				//finish();
			}
			return false;
		}
		return true;
	}

	public static void SendEmail(String strEmail) {

		Intent emailIntent = new Intent(Intent.ACTION_SENDTO, Uri.fromParts("mailto", strEmail, null));

		String strTitle = null;
		try {
			strTitle = mainApp.getPackageName() + ", ver : " + mainApp.getPackageManager().getPackageInfo(mainApp.getPackageName(), 0).versionName;
			strTitle += ", " + Build.MODEL;
		} catch (NotFoundException e) {
			e.printStackTrace();
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		emailIntent.putExtra(Intent.EXTRA_SUBJECT, strTitle);
		//emailIntent.putExtra(Intent.EXTRA_TEXT, strModel);
		mainApp.startActivity(Intent.createChooser(emailIntent, "Send email..."));
	}

	public static void SendEmail(String strEmail, String strSubject, String strBody) {

		strBody = strBody.replace("=", ":");
		strBody = strBody.replace("\n", "<br/>");

		Intent emailIntent = new Intent(Intent.ACTION_SENDTO);
		emailIntent.setData(Uri.parse("mailto:"));
		emailIntent.putExtra(Intent.EXTRA_EMAIL, new String[] { strEmail });
		emailIntent.putExtra(Intent.EXTRA_SUBJECT, strSubject);
		emailIntent.putExtra(Intent.EXTRA_TEXT, Html.fromHtml(strBody));
		mainApp.startActivity(Intent.createChooser(emailIntent, "Send email..."));
	}

	public static void ShareSNS(String strText) {

		Intent shareIntent = new Intent();
		shareIntent.setAction(Intent.ACTION_SEND);
		shareIntent.putExtra(Intent.EXTRA_TEXT, strText);

		shareIntent.setType("text/plain");
		mainApp.startActivity(shareIntent);

	}

	public static void SetDebugMode(boolean bDebug) {
		DEBUG_BUILD = bDebug;
	}



	@Override
	protected void onStart() {
		super.onStart();

	}

	@Override
	protected void onStop() {

		super.onStop();
	}

	@Override
	protected void onResume() {

		super.onResume();

		receiveDeepLink();

		//
		if (bFacebookInit)
			AppEventsLogger.activateApp(getApplication());

		_isResume = true;
	}
	@Override
	protected void onPause() {

		super.onPause();

		_isResume = false;
	}

	@Override
	protected void onDestroy() {

		super.onDestroy();

		if (mInAppPurchase != null) {
			mInAppPurchase.destroy();
		}

		_isDestroy = true;
	}

	@Override
	protected void onActivityResult(int request, int response, Intent data) {
		super.onActivityResult(request, response, data);

		if ( request == RC_SIGN_IN )
		{
			GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
			if ( result != null && result.isSuccess())
			{
				GoogleSignInAccount signedInAccount = result.getSignInAccount();

				PlayersClient playersClient = Games.getPlayersClient(mainApp, signedInAccount);
				playersClient.getCurrentPlayer().addOnSuccessListener(new com.google.android.gms.tasks.OnSuccessListener<Player>() {
					@Override
					public void onSuccess(Player player) {
						String strId = player.getPlayerId();
						if ( strId != null )
						{
							onGoogleGamePlayerId(strId);
						}
						else
						{
							onGoogleGamePlayerId("");
						}
					}
				});
				playersClient.getCurrentPlayer().addOnFailureListener(new com.google.android.gms.tasks.OnFailureListener() {
					@Override
					public void onFailure(@NonNull Exception e) {
						onGoogleGamePlayerId("");

						GoogleSignInClient signInClient = GoogleSignIn.getClient(mainApp,  GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN);
						signInClient.signOut().addOnCompleteListener(mainApp, new com.google.android.gms.tasks.OnCompleteListener<Void>() {
							@Override
							public void onComplete(@NonNull com.google.android.gms.tasks.Task<Void> task) {
							}
						});
					}
				});
			}
			else
			{
				Cocos2dxHelper.setBoolForKey("SignInCancelled", false);
				onGoogleGamePlayerId("");
			}
		}
		else if ( request == RC_GAME )
		{
			if ( response == 10001 )
			{
				Cocos2dxHelper.setBoolForKey("SignInCancelled", false);
			}
		}else if(request == RC_SEND_SMS)
		{
			_isContinueCreateDeeplink = false;
		}

	}

	public void checkSpeedHeak() {

		Thread thread = new Thread(new Runnable() {

			@Override
			public void run() {

				while (_isDestroy == false) {

					if ( _isResume == false ) {
						try {Thread.sleep(1000);} catch (InterruptedException e) {}
						_nSpeedCount = 0;
						_nSpeedTimePrev = 0;
						continue;
					}

					if ( _nSpeedTimePrev == 0 )
						_nSpeedTimePrev = System.currentTimeMillis();

					long diff = System.currentTimeMillis() - _nSpeedTimePrev - 2000;
					_nSpeedTimePrev = System.currentTimeMillis();
					if ( diff > 100 )
					{
						_nSpeedCount++;
					}
					else
					{
						_nSpeedCount = --_nSpeedCount < 0 ? 0 : _nSpeedCount;
					}

					//Log.d("Test", "Test Speed Heak diff : " + diff + ", count : " + _nSpeedCount);
					if ( _nSpeedCount > 5 )
					{
						runOnUiThread(new Runnable() {

							@Override
							public void run() {
								Toast.makeText(MafActivity.this, "어플리케이션이 정상 동작되지 않습니다.\n게임이 종료됩니다", Toast.LENGTH_SHORT).show();
							}
						});

						try {Thread.sleep(1000);} catch (InterruptedException e) {}
						moveTaskToBack(true);
						android.os.Process.killProcess(android.os.Process.myPid());

						finish();
						break;
					}

					try {Thread.sleep(2000);} catch (InterruptedException e) {}

				}
			}
		});
		thread.setDaemon(true);
		thread.start();
	}


	public static void ShowAlert(final String title, final String msg, final String btnText) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				AlertDialog.Builder alert = new AlertDialog.Builder(mainApp, AlertDialog.THEME_HOLO_LIGHT);
				alert.setTitle(title);
				alert.setMessage(msg);
				alert.setNeutralButton(btnText, new OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub	    				
					}
				});
				alert.create().show();
			}
		});

	}

	public static void ShowToast(final String msg, final boolean bLong) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if (bLong)
					Toast.makeText(mainApp, msg, Toast.LENGTH_LONG).show();
				else
					Toast.makeText(mainApp, msg, Toast.LENGTH_SHORT).show();

			}
		});

	}



	public static String GetLanguage() {
		String str = "";
		Locale locale = mainApp.getResources().getConfiguration().locale;
		str = locale.getLanguage();

		return str;
	}


	static final int RESULT_SUCCESS = 1;
	static final int RESULT_FAIL = -1;
	static final int RESULT_CANCEL = 0;
	static List<String> restoreList = null;



	public static void Purchase(final String strProductID) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if ( mInAppPurchase == null ) {
					mInAppPurchase = new InAppPurchase(mainApp, purchaseResultListener, purchaseResultLoadListener, purchaseResultHasListener);
				}

				mInAppPurchase.onPurchase(strProductID);
			}
		});
	}

	public static void PurchaseLoad(final String[] listProductId) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if ( mInAppPurchase != null ) {
					mInAppPurchase.onPurchaseLoad(listProductId);
				}
			}
		});
	}

	public static void PurchaseHas(final String strProductID) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if ( mInAppPurchase != null ) {
					mInAppPurchase.onPurchaseHas(strProductID);
				}
			}
		});
	}

	public static void PurchaseConsume(final String strProductID) {
		if ( mInAppPurchase != null ) {
			mInAppPurchase.consumePurchase(strProductID);
		}
	}

	public static void PurchaseConsumeAll() {
		if ( mInAppPurchase != null ) {
			mInAppPurchase.consumePurchaseAll();
		}
	}


	@SuppressWarnings("JniMissingFunction")
	public static native void NativePurchaseResultCB(int nResult, String responseData, String signature);
	public static native void NativePurchaseResultLoadCB(String strProductID, String strPrice, String strPriceCurrencyCode);
	public static native void NativePurchaseResultHasCB(int nResult);
	public static native void NativeInAppReviewCB(int nResult);

	static InAppPurchase.ResultInAppPurchase purchaseResultListener = new InAppPurchase.ResultInAppPurchase() {

		@Override
		public void result(final int nResult, final String strOriginalJson, final String strSignature) {
			// TODO Auto-generated method stub

			if (nResult == RESULT_SUCCESS) {
				if (DEBUG_BUILD)
					Toast.makeText(mainApp, "success", Toast.LENGTH_SHORT).show();
			} else if (nResult == RESULT_CANCEL) {
				if (DEBUG_BUILD)
					Toast.makeText(mainApp, "cancel", Toast.LENGTH_SHORT).show();
			} else {
				if (DEBUG_BUILD)
					Toast.makeText(mainApp, "fail", Toast.LENGTH_SHORT).show();
			}

//			Log.e(TAG, "getOriginalJson : "+purchase.getOriginalJson());
//			Log.e(TAG, "getSignature : "+purchase.getSignature());

			Cocos2dxHelper.runOnGLThread(new Runnable() {

				@Override
				public void run() {
					String responseData = "";
					String signature = "";
					if (nResult == RESULT_SUCCESS) {
						responseData = strOriginalJson;
						signature = strSignature;
					}
					NativePurchaseResultCB(nResult, responseData, signature);
				}
			});
		}
	};

	static InAppPurchase.ResultInAppPurchaseLoad purchaseResultLoadListener = new InAppPurchase.ResultInAppPurchaseLoad() {
		@Override
		public void result(final String strProductID, final String strPrice, final String strPriceCurrencyCode) {
			Cocos2dxHelper.runOnGLThread(new Runnable() {

				@Override
				public void run() {
					NativePurchaseResultLoadCB(strProductID, strPrice, strPriceCurrencyCode);
				}
			});
		}
	};

	static InAppPurchase.ResultInAppPurchaseHas purchaseResultHasListener = new InAppPurchase.ResultInAppPurchaseHas() {
		@Override
		public void result(final int nResult) {
			Cocos2dxHelper.runOnGLThread(new Runnable() {

				@Override
				public void run() {
					NativePurchaseResultHasCB(nResult);
				}
			});
		}
	};

	@SuppressWarnings("JniMissingFunction")
	public static native void NativeGoogleGamePlayerId(String nResult);
	public static void onGoogleGamePlayerId(final String result)
	{
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				NativeGoogleGamePlayerId(result);
			}
		});
	}
	public static void SetupGooglePlus() {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if (!CheckPlayServices())
					return;

				GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN).build();
				mGoogleSignInClient = GoogleSignIn.getClient(mainApp, gso);
			}
		});
	}

	public static void LoginGooglePlus() {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if ( !CheckPlayServices() || mGoogleSignInClient == null )
				{
					onGoogleGamePlayerId("");
					return;
				}


				boolean bLogin = Cocos2dxHelper.getBoolForKey("SignInCancelled", true);
				if ( bLogin == true )
				{
					try {
						Intent signInIntent = mGoogleSignInClient.getSignInIntent();
						mainApp.startActivityForResult(signInIntent, RC_SIGN_IN);
					} catch (Exception e) {
						onGoogleGamePlayerId("");
					}
				}
				else
				{
					onGoogleGamePlayerId("");
				}
			}
		});
	}

	public static void ReLoginGooglePlus() {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				if ( !CheckPlayServices() || mGoogleSignInClient == null )
				{
					onGoogleGamePlayerId("");
					return;
				}

				try {
					Intent signInIntent = mGoogleSignInClient.getSignInIntent();
					mainApp.startActivityForResult(signInIntent, RC_SIGN_IN);
				} catch (Exception e) {
					onGoogleGamePlayerId("");
				}
			}
		});
	}

	public static boolean IsSignedIn() {

		return GoogleSignIn.getLastSignedInAccount(mainApp) != null;
	}

	public static void UpdateLeaderBoardScore(final String LB_ID, final int score) {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getLeaderboardsClient(mainApp, signedInAccount).submitScore(LB_ID, score);
				}
			}
		});
	}

	public static void ShowLeaderBoard(final String LB_ID) {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getLeaderboardsClient(mainApp, signedInAccount)
							.getLeaderboardIntent(LB_ID)
							.addOnSuccessListener(new com.google.android.gms.tasks.OnSuccessListener<Intent>() {
								@Override
								public void onSuccess(Intent intent) {

									try{
										mainApp.startActivityForResult(intent, RC_GAME);
									}catch (ActivityNotFoundException e){ }
								}
							});
				}
			}
		});
	}

	public static void ShowLeaderBoardAll() {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getLeaderboardsClient(mainApp, signedInAccount)
							.getAllLeaderboardsIntent()
							.addOnSuccessListener(new com.google.android.gms.tasks.OnSuccessListener<Intent>() {
								@Override
								public void onSuccess(Intent intent) {

									try{
										mainApp.startActivityForResult(intent, RC_GAME);
									}catch (ActivityNotFoundException e){ }
								}
							});
				}

			}
		});
	}

	public static void ShowAchievements() {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getAchievementsClient(mainApp, signedInAccount)
							.getAchievementsIntent()
							.addOnSuccessListener(new com.google.android.gms.tasks.OnSuccessListener<Intent>() {
								@Override
								public void onSuccess(Intent intent) {

									try{
										mainApp.startActivityForResult(intent, RC_GAME);
									}catch (ActivityNotFoundException e){ }
								}
							});
				}

			}
		});
	}

	public static void UnlockAchievement(final String ACH_ID) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getAchievementsClient(mainApp, signedInAccount).unlock(ACH_ID);
				}

			}
		});
	}

	public static void IncrementAchievement(final String ACH_ID, final int point) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getAchievementsClient(mainApp, signedInAccount).increment(ACH_ID, point);
				}

			}
		});
	}

	public static void IncrementImmediateAchievement(final String ACH_ID, final int point) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				GoogleSignInAccount signedInAccount = GoogleSignIn.getLastSignedInAccount(mainApp);
				if ( signedInAccount != null )
				{
					Games.getAchievementsClient(mainApp, signedInAccount).incrementImmediate(ACH_ID, point);
				}

			}
		});
	}

	private void storeRegistrationId(Context context, String regId) {
		final SharedPreferences prefs = getGCMPreferences(context);
		int appVersion = getAppVersion(context);
		Log.i(TAG, "Saving regId on app version " + appVersion);
		SharedPreferences.Editor editor = prefs.edit();
		editor.putString(PROPERTY_REG_ID, regId);
		editor.putInt(PROPERTY_APP_VERSION, appVersion);
		editor.commit();
	}

	private SharedPreferences getGCMPreferences(Context context) {
		// This sample app persists the registration ID in shared preferences, but
		// how you store the regID in your app is up to you.
		return getSharedPreferences(MafActivity.class.getSimpleName(),
				Context.MODE_PRIVATE);
	}

	private static int getAppVersion(Context context) {
		try {
			PackageInfo packageInfo = context.getPackageManager()
					.getPackageInfo(context.getPackageName(), 0);
			return packageInfo.versionCode;
		} catch (NameNotFoundException e) {
			// should never happen
			throw new RuntimeException("Could not get package name: " + e);
		}
	}

	public static final String PROPERTY_REG_ID = "registration_id";
	private static final String PROPERTY_APP_VERSION = "appVersion";
	private final static int PLAY_SERVICES_RESOLUTION_REQUEST = 9000;

	public static String getDeviceID() {
//		if (ActivityCompat.checkSelfPermission(mainApp, Manifest.permission.READ_PHONE_STATE) != PackageManager.PERMISSION_GRANTED) {
//			return "";
//		}
//
//		final TelephonyManager tm = (TelephonyManager) mainApp.getSystemService(Context.TELEPHONY_SERVICE);
//
//		final String tmDevice = "" + tm.getDeviceId();
//
//		return tmDevice.toString();

		// READ_PHONE_STATE 삭제로 android_id 사용
		// 추후 nas를 사용할때는 getDeviceId() 필요
		String android_id = Settings.Secure.getString(mainApp.getApplicationContext().getContentResolver(), Settings.Secure.ANDROID_ID);

		return android_id;
	}

	public static native void NativeAdsId(String strAdsID);
	public static void getAdsID() {
		AsyncTask.execute(new Runnable() {
			@Override
			public void run() {
				try {
					AdvertisingIdClient.Info advertisingIdInfo = AdvertisingIdClient.getAdvertisingIdInfo(mainApp);
					if (!advertisingIdInfo.isLimitAdTrackingEnabled()){

						NativeAdsId(advertisingIdInfo.getId());
					}

				} catch (Exception e) { }
			}
		});
	}


	public static void OpenUrl(String strUrl) {

		Uri uri = Uri.parse(strUrl);
		Intent intent = new Intent(Intent.ACTION_VIEW, uri);
		mainApp.startActivity(intent);
	}

	public static void FacebookInit() {
		try {
			FacebookSdk.sdkInitialize(mainApp);
			fbLogger = AppEventsLogger.newLogger(mainApp);
			bFacebookInit = true;
		} catch (Exception e) {}
	}

	public static void FacebookLog(String strEvent, String strParam, int nValue) {
		if(fbLogger == null) {
			return;
		}

		if ( strParam.length() != 0 ) {
			Bundle params = new Bundle();
			params.putInt(strParam, nValue);
			fbLogger.logEvent(strEvent, params);
		}
		else {
			fbLogger.logEvent(strEvent);
		}
	}

	public static void PostFacebook(String strFileUrl, String strTitle, String strLinkUrl) {
	}

	public static void OpenFacebookPage(String pageUrl) {

		try {
			Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("fb://page/" + pageUrl));
			if ( intent.resolveActivity(mainApp.getPackageManager()) == null )
			{
				intent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://m.facebook.com/" + pageUrl));
			}
			mainApp.startActivity(intent);
		} catch (Exception e) { }
	}

	public static void InAppReview() {

		if (android.os.Build.VERSION.SDK_INT < 21) {
			NativeInAppReviewCB(RESULT_FAIL);
			return;
		}

		final ReviewManager inAppReviewManager = ReviewManagerFactory.create(mainApp);
		Task<ReviewInfo> request = inAppReviewManager.requestReviewFlow();
		request.addOnCompleteListener(task -> {
			try {
				if (task.isSuccessful()) {
					ReviewInfo reviewInfo = task.getResult();
					Task<Void> flow = inAppReviewManager.launchReviewFlow(mainApp, reviewInfo);
					flow.addOnCompleteListener(task2 -> {

						if (task2.isSuccessful()) {
							NativeInAppReviewCB(RESULT_SUCCESS);
						} else {
							NativeInAppReviewCB(RESULT_FAIL);
						}
					});
				} else {
					NativeInAppReviewCB(RESULT_FAIL);
				}
			} catch (Exception ex) {
				NativeInAppReviewCB(RESULT_FAIL);
			}
		});
	}

	@SuppressWarnings("JniMissingFunction")
	public static String GetGameVersion() {

		String version = "0";
		try {
			PackageInfo i = mainApp.getPackageManager().getPackageInfo(mainApp.getPackageName(), 0);
			version = i.versionName;
		} catch (NameNotFoundException e) {
		}
		return version;
	}

	public static String GetOSVersion() {

		return Build.VERSION.RELEASE.toString();
	}

	public static String GetDeviceModel() {
		return Build.MODEL;
	}

	public static String GetDeviceInstallStore() {

		String store = "NONE";

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
			try {
				android.content.pm.InstallSourceInfo info = mainApp.getPackageManager().getInstallSourceInfo(mainApp.getPackageName());
				if ( info != null ) {
					store = info.getInitiatingPackageName();
				}
			} catch (PackageManager.NameNotFoundException e) {}
		} else {
			store = mainApp.getPackageManager().getInstallerPackageName(mainApp.getPackageName());
		}

		if ( store == null ) {
			store = "NONE";
		}

		return store;
	}

	public static void ShowUpdateAlert(final String strTitle, final String strMessage, final String strConfirm, final String strShortcut) {

		mainApp.runOnUiThread(new Runnable() {
			public void run() {

				AlertDialog.Builder alert = new AlertDialog.Builder(mainApp, AlertDialog.THEME_HOLO_LIGHT);

				alert.setTitle(strTitle);
				alert.setMessage(strMessage);
				alert.setPositiveButton(strShortcut, new OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub						
						String market = "market://details?id=";
						market += mainApp.getPackageName();
						Uri uri = Uri.parse(market);
						Intent intent = new Intent(Intent.ACTION_VIEW, uri);
						mainApp.startActivity(intent);
					}
				});
				alert.setNegativeButton(strConfirm, new OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub	    				
					}
				});
				alert.create().show();
			}
		});
	}

	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
		AudioManager audio = (AudioManager)this.getSystemService(Context.AUDIO_SERVICE);
		switch (keyCode) {
			case KeyEvent.KEYCODE_VOLUME_UP:
				audio.adjustStreamVolume(AudioManager.STREAM_MUSIC,AudioManager.ADJUST_RAISE,AudioManager.FLAG_SHOW_UI);
				return true;
			case KeyEvent.KEYCODE_VOLUME_DOWN:
				audio.adjustStreamVolume(AudioManager.STREAM_MUSIC,AudioManager.ADJUST_LOWER,AudioManager.FLAG_SHOW_UI);
				return true;
		}
		return super.onKeyDown(keyCode, event);
    }


	public static final int CHECK_PERMISSION = 0;
	public static final int CHECK_PERMISSION_GET_ACCOUNTS = 1;
	public static final int CHECK_PERMISSION_WRITE_EXTERNAL_STORAGE = 2;
	public static final int CHECK_PERMISSION_READ_PHONE_STATE = 3;
	public static final int CHECK_PERMISSION_SYSTEM_ALERT_WINDOW = 4;


	public static void CheckPermission(final int permissionType[], final String msg){
		final int size = permissionType.length;
		final String[] permission = new String[size];

		for (int i=0; i<size; i++){
			if (permissionType[i] == CHECK_PERMISSION_GET_ACCOUNTS)
				permission[i] = android.Manifest.permission.GET_ACCOUNTS;
			else if (permissionType[i] == CHECK_PERMISSION_WRITE_EXTERNAL_STORAGE)
				permission[i] = android.Manifest.permission.WRITE_EXTERNAL_STORAGE;
			else
				permission[i] = android.Manifest.permission.READ_PHONE_STATE;
		}

		boolean bPermission = true;
		boolean bAsk = false;
		for (int i=0; i<size; i++){
			if (ContextCompat.checkSelfPermission(mainApp, permission[i]) != PackageManager.PERMISSION_GRANTED){
				bPermission = false;
				if (ActivityCompat.shouldShowRequestPermissionRationale(mainApp, permission[i])){
					bAsk = true;
				}
			}
		}

		if (!bPermission) {
			// 이 권한을 필요한 이유를 설명해야하는가?
			if (bAsk) {
				mainApp.runOnUiThread(new Runnable() {
					public void run() {
						AlertDialog.Builder alert = new AlertDialog.Builder(mainApp,AlertDialog.THEME_HOLO_LIGHT);
						alert.setTitle(mainApp.getResources().getString(R.string.app_name));
						alert.setMessage(msg);
						alert.setNeutralButton("확인", new OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								ActivityCompat.requestPermissions(mainApp, permission, CHECK_PERMISSION);
							}
						});
						alert.create().show();
					}});
			} else {
				ActivityCompat.requestPermissions(mainApp, permission, CHECK_PERMISSION);
			}
		}
	}

	public static boolean IsPermission(final int permissionType) {

		String permission = "";
		if (permissionType == CHECK_PERMISSION_GET_ACCOUNTS)
			permission = android.Manifest.permission.GET_ACCOUNTS;
		else if (permissionType == CHECK_PERMISSION_WRITE_EXTERNAL_STORAGE)
			permission = android.Manifest.permission.WRITE_EXTERNAL_STORAGE;
		else if (permissionType == CHECK_PERMISSION_SYSTEM_ALERT_WINDOW)
			permission = Manifest.permission.SYSTEM_ALERT_WINDOW;
		else
			permission = android.Manifest.permission.READ_PHONE_STATE;

		boolean bPermission = false;
		if (ContextCompat.checkSelfPermission(mainApp, permission) == PackageManager.PERMISSION_GRANTED) {
			bPermission = true;
		}

		return bPermission;
	}

	@Override
	public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
		switch (requestCode) {
			case CHECK_PERMISSION:
				if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

				} else {
					//ShowToast("권한 사용을 동의해주셔야 원할한 이용이 가능합니다.", false);
				}
				return;
		}
	}

	//
	enum E_ADS_RESULT {SUCCESS, FAIL, BACKGROUND_SKIP, LOAD};


	// Google Mediation var
	private static String admobIdRewardedAd = "";
	private static String admobIdInterstitial = "";
	private static String admobIdBanner = "";

	private static String admobIdTopBanner = "";


	private static com.adxcorp.ads.RewardedAd admobRewardedAd = null;
	private static com.adxcorp.ads.InterstitialAd admobInterstitialAd = null;
	private static com.adxcorp.ads.BannerAd admobBannerView = null;

	private static com.adxcorp.ads.BannerAd admobTopBannerView = null;

	private static boolean admobRewardedAdLoad = false;
	private static boolean admobInterstitialAdLoad = false;
	private static boolean admobRewardFinish = false;


	@SuppressWarnings("JniMissingFunction")
	public static native void NativeGoogleAdsInitCB();
	public static native void NativeGoogleAdsRewardCB(int nResult);
	public static void onGoogleAdsRewardCB(final int result)
	{
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				NativeGoogleAdsRewardCB(result);
			}
		});
	}

	public static void initAds() {

		admobRewardedAdLoad = false;
		admobInterstitialAdLoad = false;
		admobRewardFinish = false;

		// ADX 초기화 관련 설정
		com.adxcorp.util.ADXLogUtil.setLogEnable(true);
		com.adxcorp.ads.ADXConfiguration adxConfiguration = new com.adxcorp.ads.ADXConfiguration.Builder()
				.setAppId("62a808516da6b100010001ff")
				.setGdprType(com.adxcorp.ads.ADXConfiguration.GdprType.POPUP_LOCATION)
				.build();

		com.adxcorp.ads.ADXSdk.getInstance().initialize((Activity)mainApp, adxConfiguration, new com.adxcorp.ads.ADXSdk.OnInitializedListener() {
			@Override
			public void onCompleted(boolean result, com.adxcorp.gdpr.ADXGDPR.ADXConsentState adxConsentState) {
				// 광고 초기화 완료
				NativeGoogleAdsInitCB();
			}
		});
	}

	// Admob reward video
	public static void initRewardedAd(final String strId) {
		admobIdRewardedAd = strId;
		mainApp.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				admobRewardedAd = new com.adxcorp.ads.RewardedAd(mainApp, admobIdRewardedAd);
				admobRewardedAd.setRewardedAdListener(new com.adxcorp.ads.RewardedAd.RewardedAdListener() {
					@Override
					public void onAdLoaded() {
						admobRewardedAdLoad = false;
					}

					@Override
					public void onAdError(int i) {
						admobRewardedAdLoad = false;
					}

					@Override
					public void onAdClicked() {

					}

					@Override
					public void onAdImpression() {

					}

					@Override
					public void onAdClosed() {
						if ( admobRewardFinish == true ) {
							onGoogleAdsRewardCB(E_ADS_RESULT.SUCCESS.ordinal());
						} else {
							onGoogleAdsRewardCB(E_ADS_RESULT.BACKGROUND_SKIP.ordinal());
						}
						loadRewardedAd();
					}

					@Override
					public void onAdRewarded() {
						admobRewardFinish = true;
					}

					@Override
					public void onAdFailedToShow() {
						onGoogleAdsRewardCB(E_ADS_RESULT.LOAD.ordinal());
						loadRewardedAd();
					}
				});

				//
				loadRewardedAd();
			}
		});
	}

	public static boolean isLoadRewardedAd() {
		boolean bLoad = false;
		if ( admobRewardedAd != null && admobRewardedAd.isLoaded() == true ) {
			bLoad = true;
		}
		return bLoad;
	}

	public static void loadRewardedAd() {
		if ( admobRewardedAdLoad == true ) {
			return;
		}
		admobRewardedAdLoad = true;
		mainApp.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (admobRewardedAd != null)
					admobRewardedAd.loadAd();
			}
		});
	}

	public static void showRewardedAd() {
		//
		admobRewardFinish = false;

		mainApp.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (admobRewardedAd != null && admobRewardedAd.isLoaded()) {
					admobRewardedAd.show();
				} else {
					onGoogleAdsRewardCB(E_ADS_RESULT.LOAD.ordinal());
					loadRewardedAd();
				}
			}
		});
	}

	// Admob interstitial
	public static void initInterstitial(final String strId) {
		admobIdInterstitial =  strId;
		mainApp.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				admobInterstitialAd = new com.adxcorp.ads.InterstitialAd(mainApp, admobIdInterstitial);
				admobInterstitialAd.setInterstitialListener(new com.adxcorp.ads.InterstitialAd.InterstitialListener() {
					@Override
					public void onAdLoaded() {
						admobInterstitialAdLoad = false;
					}

					@Override
					public void onAdError(int i) {
						admobInterstitialAdLoad = false;
					}

					@Override
					public void onAdClicked() {

					}

					@Override
					public void onAdImpression() {

					}

					@Override
					public void onAdClosed() {
						loadInterstitial();
					}

					@Override
					public void onAdFailedToShow() {
						loadInterstitial();
					}
				});

				//
				loadInterstitial();
			}
		});
	}

	static void loadInterstitial() {
		if ( admobInterstitialAdLoad == true ) {
			return;
		}
		admobInterstitialAdLoad = true;
		mainApp.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (admobInterstitialAd != null)
					admobInterstitialAd.loadAd();
			}
		});
	}

	public static void showInterstitial() {

		mainApp.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (admobInterstitialAd != null && admobInterstitialAd.isLoaded()) {
					admobInterstitialAd.show();
				} else {
					loadInterstitial();
				}
			}
		});
	}

	// Admob banner
	public static void initBannerView(final String strId) {
		admobIdBanner =  strId;
		mainApp.runOnUiThread(new Runnable() {
			public void run() {
				admobBannerView = new com.adxcorp.ads.BannerAd(mainApp,admobIdBanner, com.adxcorp.ads.common.AdConstants.BANNER_AD_SIZE.AD_SIZE_320x50);

				FrameLayout.LayoutParams layout_params = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
				layout_params.gravity = Gravity.CENTER_HORIZONTAL| Gravity.BOTTOM;
				admobBannerView.setLayoutParams(layout_params);

				mainApp.mFrameLayout.addView(admobBannerView);
				admobBannerView.setVisibility(View.GONE);
				admobBannerView.disableAutoRefresh();

				//
				admobBannerView.loadAd();
			}
		});
	}

	public static void showBannerView() {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {
				if (  admobBannerView != null ) {
					admobBannerView.setVisibility(View.VISIBLE);
				}
			}
		});
	}

	public static void hideBannerView() {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {
				if ( admobBannerView != null )  {
					admobBannerView.setVisibility(View.GONE);
					admobBannerView.loadAd();
				}
			}
		});
	}

	public static void initTopBannerView(final String strId) {
		admobIdTopBanner =  strId;
		mainApp.runOnUiThread(new Runnable() {
			public void run() {
				admobTopBannerView = new com.adxcorp.ads.BannerAd(mainApp,admobIdTopBanner, com.adxcorp.ads.common.AdConstants.BANNER_AD_SIZE.AD_SIZE_320x50);

				FrameLayout.LayoutParams layout_params = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
				layout_params.gravity = Gravity.CENTER_HORIZONTAL| Gravity.TOP;
				admobTopBannerView.setLayoutParams(layout_params);

				mainApp.mFrameLayout.addView(admobTopBannerView);
				admobTopBannerView.setVisibility(View.GONE);
				admobTopBannerView.disableAutoRefresh();

				//
				admobTopBannerView.loadAd();
			}
		});
	}

	public static void showTopBannerView() {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {
				if (  admobTopBannerView != null ) {
					admobTopBannerView.setVisibility(View.VISIBLE);
				}
			}
		});
	}

	public static void hideTopBannerView() {
		mainApp.runOnUiThread(new Runnable() {
			public void run() {
				if ( admobTopBannerView != null )  {
					admobTopBannerView.setVisibility(View.GONE);
					admobTopBannerView.loadAd();
				}
			}
		});
	}








	/*
	//game page id chunenkishi_yasuhiro
	//<data android:host="moneyhero_japan" android:scheme="mafgames" /> mafgames://moneyhero_japan
    NSString *const officialCommunityUrlString       = @"https://web.lobi.co/special/community-lp/chunenkishi_yasuhiro?backScheme=moneyhero:";
    NSString *const officialCommunityUrlSchemeString = @"lobi://game_community?gameId=chunenkishi_yasuhiro&backScheme=moneyhero:";
    //    NSString *const officialCommunityUrlSchemeString = @"Lobi://public_groups_tree?category=카테고리ID&backScheme=게임을 실행하기 위한 scheme";    
    // Lobi初回起動時にゲームページヘ遷移するためにペーストボードを利用する。
    UIPasteboard.generalPasteboard.string = @"{\"co.lobi.pasteboard.urlscheme\":\"lobi://game_community?gameId=chunenkishi_yasuhiro\"}";
	 * */
	// Lobiチームよりご案内した<ゲームページID>と<ゲームを起動するためのスキーム>を設定してください。
	private static final String LOBI_GAMEPAGE_ID = "chunenkishi_yasuhiro";

	public static void OpenLobiCommunity() throws IOException {
	    PackageManager packageManager = mainApp.getPackageManager();
	    Intent launchIntent = packageManager.getLaunchIntentForPackage("com.kayac.nakamap");

	    final String uri;
	    if (launchIntent == null) {
	        // Lobi is not installed
	       uri = String.format("https://web.lobi.co/special/community-lp/%s?backScheme=mafgames://moneyhero_japan", LOBI_GAMEPAGE_ID);
	    } else {
	        // Lobi is installed
	       uri = String.format("lobi://game_community?gameId=%s&backScheme=mafgames://moneyhero_japan", LOBI_GAMEPAGE_ID);
	    }
	    mainApp.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(uri)));

	}


	public static void InitFirebaseAnalytics(){
		mFirebaseAnalytics = FirebaseAnalytics.getInstance(mainApp);
	}

	public static void LogEvent(String event, String param, int val){

		if ( param.length() != 0 ) {
			Bundle params = new Bundle();
			params.putInt(param, val);
			mFirebaseAnalytics.logEvent(event, params);
		}
		else {
			mFirebaseAnalytics.logEvent(event, null);
		}
	}

	public static void LogEvent(String event, String param, String val){
		
		if ( param.length() != 0 ) {
			Bundle params = new Bundle();
			params.putString(param, val);
			mFirebaseAnalytics.logEvent(event, params);
		}
		else {
			mFirebaseAnalytics.logEvent(event, null);
		}
	}


	public static void InitAppsFlyer(final String devkey){
			mainApp.runOnUiThread(new Runnable() {
			public void run() {
				AppsFlyerLib.getInstance().start(mainApp.getApplication(), devkey);
				//AppsFlyerLib.getInstance().startTracking(mainApp.getApplication(),devkey);
			}});
	}

	public static void AppsFlyerTrackEventPurchase(String productId, String price, String currency){

		String newPrice = price.replaceFirst(",","");

		AppsFlyerLib.getInstance().setCurrencyCode(currency);

		Map<String, Object> eventValue = new HashMap<String, Object>();
		eventValue.put(AFInAppEventParameterName.REVENUE,newPrice);
		eventValue.put(AFInAppEventParameterName.CONTENT_TYPE,"InApp");
		eventValue.put(AFInAppEventParameterName.CONTENT_ID,productId);
		eventValue.put(AFInAppEventParameterName.CURRENCY,currency);
		AppsFlyerLib.getInstance().logEvent(mainApp, AFInAppEventType.PURCHASE, eventValue);

	}

	public static void AppsFlyerTrackEvent(String strEventName) {

		Map<String,Object> eventValues = new HashMap<>();
		AppsFlyerLib.getInstance().logEvent(mainApp, strEventName, eventValues);
	}

	public static void InitRemoteConfig(){
		mFirebaseRemoteConfig = FirebaseRemoteConfig.getInstance();
		FirebaseRemoteConfigSettings configSettings = new FirebaseRemoteConfigSettings.Builder()
				.setMinimumFetchIntervalInSeconds(3600)
				.build();
		mFirebaseRemoteConfig.setConfigSettingsAsync(configSettings);
		mFirebaseRemoteConfig.fetchAndActivate()
				.addOnCompleteListener(mainApp, new com.google.android.gms.tasks.OnCompleteListener<Boolean>() {
					@Override
					public void onComplete(@NonNull com.google.android.gms.tasks.Task<Boolean> task) {
						if (task.isSuccessful()) {

						}
					}
				});
	}

	public static String GetRemoteConfigValueString(String key){
		return mFirebaseRemoteConfig.getString(key);
	}
	public static int GetRemoteValueInt(String key){
		return (int)mFirebaseRemoteConfig.getLong(key);
	}
	public static boolean GetRemoteValueBool(String key){
		return mFirebaseRemoteConfig.getBoolean(key);
	}
	public static boolean isRemoteConfigValue(String key){
		return mFirebaseRemoteConfig.getAll().containsKey(key);
	}

	private static void createNotificationToken()
	{
		FirebaseMessaging.getInstance().getToken().addOnCompleteListener(new com.google.android.gms.tasks.OnCompleteListener<String>() {
			@Override
			public void onComplete(@NonNull com.google.android.gms.tasks.Task<String> task) {
				if (!task.isSuccessful()) {
					Log.w(TAG, "Fetching FCM registration token failed", task.getException());
					return;
				}

				// Get new FCM registration token
				String token = task.getResult();
				if ( token != null ) {
					try {
						if ( getContext() == null )
						{
							return;
						}

						SharedPreferences settings = getContext().getSharedPreferences("Cocos2dxPrefsFile", 0);
						SharedPreferences.Editor editor = settings.edit();
						editor.putString("token", token);
						editor.apply();
					} catch (Exception e) {}
				}
			}
		});
	}

	private static void createNotificationChannel()
	{
		if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
			NotificationManager notificationManager = (NotificationManager)MafActivity.mainApp.getApplicationContext().getSystemService(MafActivity.mainApp.NOTIFICATION_SERVICE);
			notificationChannel = new NotificationChannel(
					"Maf Notifi Channel",
					"Bongsik Channel",
					NotificationManager.IMPORTANCE_DEFAULT
			);

			notificationChannel.setDescription("Bongsik Push Notification");
			notificationChannel.enableLights(true);
			notificationChannel.setLightColor(Color.GREEN);
			notificationChannel.enableVibration(true);
			notificationChannel.setVibrationPattern(new long[]{100, 200, 100, 200});
			notificationChannel.setLockscreenVisibility(Notification.VISIBILITY_PRIVATE);
			notificationManager.createNotificationChannel(notificationChannel);
		}
	}

	public static void SendLocalNotifiaction(int notiId, String notiTitle, String notiDesc, int timeTerm)
	{
		long futureInMillis = SystemClock.elapsedRealtime() + timeTerm * 1000;
		Intent intent = new Intent(mainApp, LocalNotificationPublisher.class);
		intent.putExtra(LocalNotificationPublisher.NOTIFICATION_ID, notiId);
		intent.putExtra(LocalNotificationPublisher.NOTIFICATION_Msg, notiDesc);
		intent.putExtra(LocalNotificationPublisher.NOTIFICATION_Title, notiTitle);

		PendingIntent p1;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S){
			p1 = PendingIntent.getBroadcast(mainApp.getApplicationContext(), notiId, intent, PendingIntent.FLAG_IMMUTABLE);
		}else {
			p1 = PendingIntent.getBroadcast(mainApp.getApplicationContext(), notiId, intent, PendingIntent.FLAG_UPDATE_CURRENT);
		}

		AlarmManager a = (AlarmManager) mainApp.getSystemService(Context.ALARM_SERVICE);
		a.set(AlarmManager.ELAPSED_REALTIME_WAKEUP, futureInMillis, p1);
	}

	public static void deleteNotification(int notiId)
	{
		AlarmManager alarmManager = (AlarmManager) mainApp.getApplicationContext().getSystemService(Context.ALARM_SERVICE);
		PendingIntent pendingIntent;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S){
			pendingIntent = PendingIntent.getBroadcast(mainApp.getApplicationContext(), notiId, new Intent(mainApp, LocalNotificationPublisher.class), PendingIntent.FLAG_IMMUTABLE);
		}else {
			pendingIntent = PendingIntent.getBroadcast(mainApp.getApplicationContext(), notiId, new Intent(mainApp, LocalNotificationPublisher.class), PendingIntent.FLAG_UPDATE_CURRENT);
		}
		alarmManager.cancel(pendingIntent);
	}

	///for dynamic link
	public static native void NativeReceiveDeepLinkParam(String key);
	public static void receiveDeepLink(){
		FirebaseDynamicLinks.getInstance().getDynamicLink(mainApp.getIntent())
				.addOnSuccessListener(mainApp, new com.google.android.gms.tasks.OnSuccessListener<PendingDynamicLinkData>() {
					@Override
					public void onSuccess(PendingDynamicLinkData pendingDynamicLinkData) {
						Uri deepLink = null;
						if(pendingDynamicLinkData != null){
							deepLink = pendingDynamicLinkData.getLink();

							Set<String> paramKeys = deepLink.getQueryParameterNames();

							Iterator value = paramKeys.iterator();
							String result = "";
							while(value.hasNext())
							{
								String key = value.next().toString();
								Log.d("DeepLinkTest", "Receive " + key + " : " + deepLink.getQueryParameter(key));
								//NativeReceiveDeepLinkParam(key, deepLink.getQueryParameter(key));
								result = result + "&" + key +"=" + deepLink.getQueryParameter(key);
							}

							Log.d("DeepLinkTest", "Receive : " + result);

							NativeReceiveDeepLinkParam(result);

							Log.d("DeepLinkTest", "Receive : " + deepLink.toString());


							//send To Client & req Friend
						}
					}
				}).addOnFailureListener(mainApp, new com.google.android.gms.tasks.OnFailureListener() {
					@Override
					public void onFailure(@NonNull Exception e) {
						Log.w("DeepLinkTest", "getDynamicLink:onFailure", e);
					}
				}
		);
	}

	public static void createDeepLink(String paramName, String paramValue){
		if(_isContinueCreateDeeplink == true) return;

		_isContinueCreateDeeplink = true;

		Log.d("DeepLinkTest", "CreateDeepLink");
		String fullLink = "https://play.google.com/store/apps/details?id=com.maf.moneyhero&utm_campaign=createDeepLink&utm_source=moneyhero&os=android";
		if(paramName.length() > 0 && paramValue.length() > 0){
			fullLink = String.format("%s&%s=%s", fullLink, paramName, paramValue);
			Log.d("DeepLinkTest", "Create readey : " + fullLink);
		}

		com.google.android.gms.tasks.Task<ShortDynamicLink> shortLinkTask = FirebaseDynamicLinks.getInstance().createDynamicLink()
									.setLink(Uri.parse(fullLink))
									.setDomainUriPrefix("https://moneyhero.page.link")
									.setAndroidParameters(new DynamicLink.AndroidParameters.Builder().build())
									.setIosParameters(new DynamicLink.IosParameters.Builder("com.maf.moneyhero").build())
									.buildShortDynamicLink()
									.addOnCompleteListener(mainApp, new com.google.android.gms.tasks.OnCompleteListener<ShortDynamicLink>() {
										@Override
										public void onComplete(@NonNull com.google.android.gms.tasks.Task<ShortDynamicLink> task) {
											if(task.isSuccessful()){
												Uri shortLink = task.getResult().getShortLink();
												Uri flowchartLink = task.getResult().getPreviewLink();

												Log.d("DeepLinkTest", "Create : " + shortLink.toString());
												Log.d("DeepLinkTest", "Create : " + flowchartLink.toString());

												showSendIntent(shortLink.toString());
											}else{
												//Error
											}
										}
									});
	}

	public static void showSendIntent(String link){
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_SEND);
		intent.setType("text/plain");
		intent.putExtra(Intent.EXTRA_TEXT, String.format("%s", link));

		mainApp.startActivityForResult(Intent.createChooser(intent, ""), RC_SEND_SMS);
	}

	public static void addClippboard(final String text)
	{
		Runnable runnable = new Runnable() {
			@Override
			public void run() {
				ClipboardManager clipboard = (ClipboardManager)mainApp.getSystemService(Context.CLIPBOARD_SERVICE);
				ClipData clip = ClipData.newPlainText("ID", text);
				clipboard.setPrimaryClip(clip);
			}
		};
		mainApp.runOnUiThread(runnable);

	}
}
	
	


