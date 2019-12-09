package com.rmuti.en.smartparking;

import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class HistoryActivity extends AppCompatActivity {

    private RecyclerView historyList;
    private HistoryAdapter adapter;
    private ProgressBar progressBar;
    private TextView tvError;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_history);

        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setTitle(R.string.parking_history);
        }
        historyList = findViewById(R.id.history_list);
        progressBar = findViewById(R.id.progress_bar);
        tvError = findViewById(R.id.tv_error);

        adapter = new HistoryAdapter();
        historyList.setLayoutManager(new LinearLayoutManager(getApplicationContext()));
        historyList.setAdapter(adapter);

        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            String point = bundle.getString("point");
            getSupportActionBar().setTitle((getString(R.string.parking_history) + " " + point));
            if (point != null && !point.isEmpty()) {
                setupDatabase(point);
            } else {
                historyList.setVisibility(View.GONE);
                progressBar.setVisibility(View.GONE);
                tvError.setVisibility(View.VISIBLE);
            }
        }
    }

    private void setupDatabase(String point) {
        progressBar.setVisibility(View.VISIBLE);
        tvError.setVisibility(View.GONE);

        DatabaseReference historyRef = FirebaseDatabase.getInstance().getReference().child("history").child(point);
        historyRef.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                Log.e("Database", "onDataChange");
                if (dataSnapshot.exists()) {
                    ArrayList<HistoryModel> list = new ArrayList<>();
                    for (DataSnapshot data : dataSnapshot.getChildren()) {
                        HistoryModel historyModel = data.getValue(HistoryModel.class);
                        list.add(historyModel);
                    }
                    adapter.setHistoryList(list);
                    progressBar.setVisibility(View.GONE);
                    tvError.setVisibility(View.GONE);
                    historyList.setVisibility(View.VISIBLE);

                } else {
                    historyList.setVisibility(View.GONE);
                    progressBar.setVisibility(View.GONE);
                    tvError.setText(R.string.no_history);
                    tvError.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
