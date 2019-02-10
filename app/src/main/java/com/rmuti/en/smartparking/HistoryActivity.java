package com.rmuti.en.smartparking;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class HistoryActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_history);

        String point = "";

        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            point = bundle.getString("point");
        }

        if (!point.equals("")) {

        }
    }
}
