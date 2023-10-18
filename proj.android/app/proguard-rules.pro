-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference
-keep public class com.android.vending.licensing.ILicensingService

-keepattributes JavascriptInterface
-keep class * extends java.util.ListResourceBundle {
protected Object[][] getContents();
}
-keep public class com.google.android.gms.common.internal.safeparcel.SafeParcelable {
public static final *** NULL;
}
-keepnames @com.google.android.gms.common.annotation.KeepName class *
-keepclassmembernames class * {
@com.google.android.gms.common.annotation.KeepName *;
}
-keepnames class * implements android.os.Parcelable {
public static final ** CREATOR;
}

-keep public class **.R {
  public *;
}

-keep public class **.R$* {
  public *;
}

-keepclasseswithmembernames class * {
    native <methods>;
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

-keepclassmembers class * extends android.app.Activity {
   public void *(android.view.View);
}

-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

# Needed when building against the Marshmallow SDK
-dontwarn org.apache.http.**

-keepattributes Annotation
-keep class com.google.android.gms.ads.identifier.** { *; }
-keep class com.google.android.gms.** { *; }
-dontwarn com.google.android.gms.**

-keepattributes JavascriptInterface
-keepattributes SourceFile,LineNumberTable
-keep public class * extends java.lang.Exception
#maf
-keep public class com.maf.iab.MafActivity {
    *;
}
#cocos
-keep public class org.cocos2dx.lib.* {
    *;
}
#facebook
-keep public class com.facebook.* {
    *;
}
-keep public class com.facebook.ads.** {
    *;
}
#For mediation adapter
-keep class com.google.ads.mediation.unity.**{*;}


#unity
-keep class com.unity3d.ads.android.** {# Keep all classes in Unity Ads package
   *;
}
#appsflyer
-dontwarn com.android.installreferrer


-keepattributes Signature
-keepattributes *Annotation*
-keepattributes EnclosingMethod
-keepattributes InnerClasses
-keep class com.google.firebase.** { *; }

-keep class com.crashlytics.** { *; }
-dontwarn com.crashlytics.**


# mintegral
-keepattributes Signature
-keepattributes *Annotation*
-keep class com.mintegral.** {*; }
-keep interface com.mintegral.** {*; }
-keep class android.support.v4.** { *; }
-dontwarn com.mintegral.**
-keep class **.R$* { public static final int mintegral*; }
-keep class com.alphab.** {*; }
-keep interface com.alphab.** {*; }

# google ads
-keep class com.google.ads.** {*;}

# adx ads
-keep class com.bytedance.** {*;}
-keep class com.adxcorp.ads.** {*;}