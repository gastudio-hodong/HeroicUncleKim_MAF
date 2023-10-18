package com.maf.iab;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

import androidx.core.app.NotificationCompat;


public class LocalNotificationPublisher extends BroadcastReceiver {

    public static String NOTIFICATION_ID = "maf_notifi";
    public static String NOTIFICATION = "notification";
    public static String NOTIFICATION_Title = "notification_title";
    public static String NOTIFICATION_Msg = "notification_Msg";


    @Override
    public void onReceive(Context context, Intent intent) {
        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

        Intent contentIntent = new Intent(context, MafActivity.class);

        int notificationId = intent.getIntExtra(NOTIFICATION_ID, 0);
        String notificationTitle = intent.getStringExtra(NOTIFICATION_Title);
        String notificationDesc = intent.getStringExtra(NOTIFICATION_Msg);

        if(notificationId == 0) return;
        if(notificationTitle == null) return;
        if(notificationTitle.length() == 0) return;

        PendingIntent contentPendintIntent;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S){
            contentPendintIntent = PendingIntent.getBroadcast(context, notificationId, contentIntent, PendingIntent.FLAG_IMMUTABLE);
        }else {
            contentPendintIntent = PendingIntent.getBroadcast(context, notificationId, contentIntent, PendingIntent.FLAG_UPDATE_CURRENT);
        }

        NotificationCompat.Builder builder;
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            builder = new NotificationCompat.Builder(context, "Maf Notifi Channel");
        }else{
            builder = new NotificationCompat.Builder(context.getApplicationContext());
        }
        builder.setContentTitle(notificationTitle);
        builder.setContentText(notificationDesc);
        builder.setSmallIcon(R.drawable.ic_notify_11);
        builder.setContentIntent(contentPendintIntent);
        builder.setAutoCancel(true);
        builder.setPriority(NotificationCompat.PRIORITY_DEFAULT);
        builder.setDefaults(NotificationCompat.DEFAULT_ALL);

        //Notification notification = intent.getParcelableExtra(NOTIFICATION);


        notificationManager.notify(notificationId, builder.build());

    }



}
