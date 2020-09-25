package com.rmuti.en.smartparking;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, ChildEventListener {

    private TextView no1, no2, no3, no4, no5, no6, no7, no8, no9, no10,
            no11, no12, no13, no14, no15, no16, no17, no18, no19, no20,
            no21, no22, no23, no24, no25, no26, no27, no28, no29, no30,
            no31, no32, no33, no34, no35, no36, no37, no38, no39, no40,
            no41, no42, no43, no44, no45, no46, no47, no48, no49, no50,
            no51, no52, no53, no54, no55, no56, no57, no58, no59, no60,
            no61, no62, no63, no64, no65, no66, no67, no68, no69, no70,
            no71, no72,
            blank, busy;

    private FrameLayout progressLayout;

    private DatabaseReference statusRef;

    private int blankValue = 0;
    private int busyValue = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setTitle(getString(R.string.app_name) + " RMUTI KKC");
        }

        progressLayout = findViewById(R.id.progress_layout);
        progressLayout.setVisibility(View.VISIBLE);

        no1 = findViewById(R.id.no_1);
        no2 = findViewById(R.id.no_2);
        no3 = findViewById(R.id.no_3);
        no4 = findViewById(R.id.no_4);
        no5 = findViewById(R.id.no_5);
        no6 = findViewById(R.id.no_6);
        no7 = findViewById(R.id.no_7);
        no8 = findViewById(R.id.no_8);
        no9 = findViewById(R.id.no_9);
        no10 = findViewById(R.id.no_10);
        no11 = findViewById(R.id.no_11);
        no12 = findViewById(R.id.no_12);
        no13 = findViewById(R.id.no_13);
        no14 = findViewById(R.id.no_14);
        no15 = findViewById(R.id.no_15);
        no16 = findViewById(R.id.no_16);
        no17 = findViewById(R.id.no_17);
        no18 = findViewById(R.id.no_18);
        no19 = findViewById(R.id.no_19);
        no20 = findViewById(R.id.no_20);
        no21 = findViewById(R.id.no_21);
        no22 = findViewById(R.id.no_22);
        no23 = findViewById(R.id.no_23);
        no24 = findViewById(R.id.no_24);
        no25 = findViewById(R.id.no_25);
        no26 = findViewById(R.id.no_26);
        no27 = findViewById(R.id.no_27);
        no28 = findViewById(R.id.no_28);
        no29 = findViewById(R.id.no_29);
        no30 = findViewById(R.id.no_30);
        no31 = findViewById(R.id.no_31);
        no32 = findViewById(R.id.no_32);
        no33 = findViewById(R.id.no_33);
        no34 = findViewById(R.id.no_34);
        no35 = findViewById(R.id.no_35);
        no36 = findViewById(R.id.no_36);
        no37 = findViewById(R.id.no_37);
        no38 = findViewById(R.id.no_38);
        no39 = findViewById(R.id.no_39);
        no40 = findViewById(R.id.no_40);
        no41 = findViewById(R.id.no_41);
        no42 = findViewById(R.id.no_42);
        no43 = findViewById(R.id.no_43);
        no44 = findViewById(R.id.no_44);
        no45 = findViewById(R.id.no_45);
        no46 = findViewById(R.id.no_46);
        no47 = findViewById(R.id.no_47);
        no48 = findViewById(R.id.no_48);
        no49 = findViewById(R.id.no_49);
        no50 = findViewById(R.id.no_50);
        no51 = findViewById(R.id.no_51);
        no52 = findViewById(R.id.no_52);
        no53 = findViewById(R.id.no_53);
        no54 = findViewById(R.id.no_54);
        no55 = findViewById(R.id.no_55);
        no56 = findViewById(R.id.no_56);
        no57 = findViewById(R.id.no_57);
        no58 = findViewById(R.id.no_58);
        no59 = findViewById(R.id.no_59);
        no60 = findViewById(R.id.no_60);
        no61 = findViewById(R.id.no_61);
        no62 = findViewById(R.id.no_62);
        no63 = findViewById(R.id.no_63);
        no64 = findViewById(R.id.no_64);
        no65 = findViewById(R.id.no_65);
        no66 = findViewById(R.id.no_66);
        no67 = findViewById(R.id.no_67);
        no68 = findViewById(R.id.no_68);
        no69 = findViewById(R.id.no_69);
        no70 = findViewById(R.id.no_70);
        no71 = findViewById(R.id.no_71);
        no72 = findViewById(R.id.no_72);
        blank = findViewById(R.id.tv_blank);
        busy = findViewById(R.id.tv_busy);

        no1.setOnClickListener(this);
        no2.setOnClickListener(this);
        no3.setOnClickListener(this);
        no4.setOnClickListener(this);
        no5.setOnClickListener(this);
        no6.setOnClickListener(this);
        no7.setOnClickListener(this);
        no8.setOnClickListener(this);
        no9.setOnClickListener(this);
        no10.setOnClickListener(this);
        no11.setOnClickListener(this);
        no12.setOnClickListener(this);
        no13.setOnClickListener(this);
        no14.setOnClickListener(this);
        no15.setOnClickListener(this);
        no16.setOnClickListener(this);
        no17.setOnClickListener(this);
        no18.setOnClickListener(this);
        no19.setOnClickListener(this);
        no20.setOnClickListener(this);
        no21.setOnClickListener(this);
        no22.setOnClickListener(this);
        no23.setOnClickListener(this);
        no24.setOnClickListener(this);
        no25.setOnClickListener(this);
        no26.setOnClickListener(this);
        no27.setOnClickListener(this);
        no28.setOnClickListener(this);
        no29.setOnClickListener(this);
        no30.setOnClickListener(this);
        no31.setOnClickListener(this);
        no32.setOnClickListener(this);
        no33.setOnClickListener(this);
        no34.setOnClickListener(this);
        no35.setOnClickListener(this);
        no36.setOnClickListener(this);
        no37.setOnClickListener(this);
        no38.setOnClickListener(this);
        no39.setOnClickListener(this);
        no40.setOnClickListener(this);
        no41.setOnClickListener(this);
        no42.setOnClickListener(this);
        no43.setOnClickListener(this);
        no44.setOnClickListener(this);
        no45.setOnClickListener(this);
        no46.setOnClickListener(this);
        no47.setOnClickListener(this);
        no48.setOnClickListener(this);
        no49.setOnClickListener(this);
        no50.setOnClickListener(this);
        no51.setOnClickListener(this);
        no52.setOnClickListener(this);
        no53.setOnClickListener(this);
        no54.setOnClickListener(this);
        no55.setOnClickListener(this);
        no56.setOnClickListener(this);
        no57.setOnClickListener(this);
        no58.setOnClickListener(this);
        no59.setOnClickListener(this);
        no60.setOnClickListener(this);
        no61.setOnClickListener(this);
        no62.setOnClickListener(this);
        no63.setOnClickListener(this);
        no64.setOnClickListener(this);
        no65.setOnClickListener(this);
        no66.setOnClickListener(this);
        no67.setOnClickListener(this);
        no68.setOnClickListener(this);
        no69.setOnClickListener(this);
        no70.setOnClickListener(this);
        no71.setOnClickListener(this);
        no72.setOnClickListener(this);


        statusRef = FirebaseDatabase.getInstance().getReference().child("status");
        statusRef.addChildEventListener(this);
    }


    @Override
    public void onClick(View view) {
        if (view instanceof TextView) {
            TextView point = ((TextView) view);
            onClickPoint(point.getText().toString());
        }
    }

    private void onClickPoint(String number) {
        if (number.length() == 1) {
            number = "0" + number;
        }
        Intent intent = new Intent(this, HistoryActivity.class);
        intent.putExtra("point", ("no" + number));
        startActivity(intent);
    }

    @Override
    public void onChildAdded(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {
        //Log.e("Database", "onChildAdded");
        if (progressLayout.getVisibility() != View.GONE) {
            progressLayout.setVisibility(View.GONE);
        }

        boolean isBlank = getValue(dataSnapshot.getValue(String.class));
        onPointChanged(dataSnapshot.getKey(), isBlank);
        if (isBlank) {
            blankValue++;
            blank.setText(String.valueOf(blankValue));
        } else {
            busyValue++;
            busy.setText(String.valueOf(busyValue));
        }
    }

    @Override
    public void onChildChanged(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {
        Log.e("Database", "onChildChanged");
        if (progressLayout.getVisibility() != View.GONE) {
            progressLayout.setVisibility(View.GONE);
        }
        boolean isBlank = getValue(dataSnapshot.getValue(String.class));
        onPointChanged(dataSnapshot.getKey(), isBlank);
        if (isBlank) {
            blankValue++;
            busyValue--;

        } else {
            blankValue--;
            busyValue++;
        }
        busy.setText(String.valueOf(busyValue));
        blank.setText(String.valueOf(blankValue));
    }

    @Override
    public void onChildRemoved(@NonNull DataSnapshot dataSnapshot) {

    }

    @Override
    public void onChildMoved(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {

    }

    @Override
    public void onCancelled(@NonNull DatabaseError databaseError) {

    }

    private boolean getValue(String value) {
        return value != null && value.equals("true");
    }

    private void onPointChanged(@Nullable String point, boolean isBlank) {
        if (point == null) {
            return;
        }

        if (point.equals("no01")) {
            updatePoint(no1, isBlank);
        } else if (point.equals("no02")) {
            updatePoint(no2, isBlank);
        } else if (point.equals("no03")) {
            updatePoint(no3, isBlank);
        } else if (point.equals("no04")) {
            updatePoint(no4, isBlank);
        } else if (point.equals("no05")) {
            updatePoint(no5, isBlank);
        } else if (point.equals("no06")) {
            updatePoint(no6, isBlank);
        } else if (point.equals("no07")) {
            updatePoint(no7, isBlank);
        } else if (point.equals("no08")) {
            updatePoint(no8, isBlank);
        } else if (point.equals("no09")) {
            updatePoint(no9, isBlank);
        } else if (point.equals("no10")) {
            updatePoint(no10, isBlank);
        } else if (point.equals("no11")) {
            updatePoint(no11, isBlank);
        } else if (point.equals("no12")) {
            updatePoint(no12, isBlank);
        } else if (point.equals("no13")) {
            updatePoint(no13, isBlank);
        } else if (point.equals("no14")) {
            updatePoint(no14, isBlank);
        } else if (point.equals("no15")) {
            updatePoint(no15, isBlank);
        } else if (point.equals("no16")) {
            updatePoint(no16, isBlank);
        } else if (point.equals("no17")) {
            updatePoint(no17, isBlank);
        } else if (point.equals("no18")) {
            updatePoint(no18, isBlank);
        } else if (point.equals("no19")) {
            updatePoint(no19, isBlank);
        } else if (point.equals("no20")) {
            updatePoint(no20, isBlank);
        } else if (point.equals("no21")) {
            updatePoint(no21, isBlank);
        } else if (point.equals("no22")) {
            updatePoint(no22, isBlank);
        } else if (point.equals("no23")) {
            updatePoint(no23, isBlank);
        } else if (point.equals("no24")) {
            updatePoint(no24, isBlank);
        } else if (point.equals("no25")) {
            updatePoint(no25, isBlank);
        } else if (point.equals("no26")) {
            updatePoint(no26, isBlank);
        } else if (point.equals("no27")) {
            updatePoint(no27, isBlank);
        } else if (point.equals("no28")) {
            updatePoint(no28, isBlank);
        } else if (point.equals("no29")) {
            updatePoint(no29, isBlank);
        } else if (point.equals("no30")) {
            updatePoint(no30, isBlank);
        } else if (point.equals("no31")) {
            updatePoint(no31, isBlank);
        } else if (point.equals("no32")) {
            updatePoint(no32, isBlank);
        } else if (point.equals("no33")) {
            updatePoint(no33, isBlank);
        } else if (point.equals("no34")) {
            updatePoint(no34, isBlank);
        } else if (point.equals("no35")) {
            updatePoint(no35, isBlank);
        } else if (point.equals("no36")) {
            updatePoint(no36, isBlank);
        } else if (point.equals("no37")) {
            updatePoint(no37, isBlank);
        } else if (point.equals("no38")) {
            updatePoint(no38, isBlank);
        } else if (point.equals("no39")) {
            updatePoint(no39, isBlank);
        } else if (point.equals("no40")) {
            updatePoint(no40, isBlank);
        } else if (point.equals("no41")) {
            updatePoint(no41, isBlank);
        } else if (point.equals("no42")) {
            updatePoint(no42, isBlank);
        } else if (point.equals("no43")) {
            updatePoint(no43, isBlank);
        } else if (point.equals("no44")) {
            updatePoint(no44, isBlank);
        } else if (point.equals("no45")) {
            updatePoint(no45, isBlank);
        } else if (point.equals("no46")) {
            updatePoint(no46, isBlank);
        } else if (point.equals("no47")) {
            updatePoint(no47, isBlank);
        } else if (point.equals("no48")) {
            updatePoint(no48, isBlank);
        } else if (point.equals("no49")) {
            updatePoint(no49, isBlank);
        } else if (point.equals("no50")) {
            updatePoint(no50, isBlank);
        } else if (point.equals("no51")) {
            updatePoint(no51, isBlank);
        } else if (point.equals("no52")) {
            updatePoint(no52, isBlank);
        } else if (point.equals("no53")) {
            updatePoint(no53, isBlank);
        } else if (point.equals("no54")) {
            updatePoint(no54, isBlank);
        } else if (point.equals("no55")) {
            updatePoint(no55, isBlank);
        } else if (point.equals("no56")) {
            updatePoint(no56, isBlank);
        } else if (point.equals("no57")) {
            updatePoint(no57, isBlank);
        } else if (point.equals("no58")) {
            updatePoint(no58, isBlank);
        } else if (point.equals("no59")) {
            updatePoint(no59, isBlank);
        } else if (point.equals("no60")) {
            updatePoint(no60, isBlank);
        } else if (point.equals("no61")) {
            updatePoint(no61, isBlank);
        } else if (point.equals("no62")) {
            updatePoint(no62, isBlank);
        } else if (point.equals("no63")) {
            updatePoint(no63, isBlank);
        } else if (point.equals("no64")) {
            updatePoint(no64, isBlank);
        } else if (point.equals("no65")) {
            updatePoint(no65, isBlank);
        } else if (point.equals("no66")) {
            updatePoint(no66, isBlank);
        } else if (point.equals("no67")) {
            updatePoint(no67, isBlank);
        } else if (point.equals("no68")) {
            updatePoint(no68, isBlank);
        } else if (point.equals("no69")) {
            updatePoint(no69, isBlank);
        } else if (point.equals("no70")) {
            updatePoint(no70, isBlank);
        } else if (point.equals("no71")) {
            updatePoint(no71, isBlank);
        } else if (point.equals("no72")) {
            updatePoint(no72, isBlank);
        }
    }

    private void updatePoint(TextView point, boolean isBlank) {

        if (isBlank) {
            point.setBackgroundResource(R.drawable.bg_point_free);
            //point.setTextColor(ContextCompat.getColor(getApplicationContext(), android.R.color.white));
        } else {
            point.setBackgroundResource(R.drawable.bg_point_busy);
            //point.setTextColor(ContextCompat.getColor(getApplicationContext(), android.R.color.white));
        }
    }
}
