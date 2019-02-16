package com.rmuti.en.smartparking;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;

public class HistoryAdapter extends RecyclerView.Adapter<HistoryAdapter.HistoryViewHolder> {

    private ArrayList<HistoryModel> historyList;

    @NonNull
    @Override
    public HistoryViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_history, parent, false);
        return new HistoryViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull HistoryViewHolder viewHolder, int position) {
        HistoryModel data = historyList.get(position);
        viewHolder.bind(data);
    }

    @Override
    public int getItemCount() {
        if (historyList == null) {
            return 0;
        }
        return historyList.size();
    }

    public void setHistoryList(ArrayList<HistoryModel> historyList) {
        this.historyList = historyList;
        notifyDataSetChanged();
    }

    public void addHistory(HistoryModel history) {
        if (historyList == null) {
            historyList = new ArrayList<>();
        }
        historyList.add(history);
        notifyItemInserted(historyList.size() - 1);
    }

    class HistoryViewHolder extends RecyclerView.ViewHolder {

        private TextView tvTime;
        private TextView tvAction;

        private HistoryViewHolder(@NonNull View itemView) {
            super(itemView);

            tvTime = itemView.findViewById(R.id.tv_time);
            tvAction = itemView.findViewById(R.id.tv_action);
        }

        private void bind(HistoryModel history) {
            tvTime.setText(history.getTime());
            tvAction.setText(history.getAction());
        }
    }
}
