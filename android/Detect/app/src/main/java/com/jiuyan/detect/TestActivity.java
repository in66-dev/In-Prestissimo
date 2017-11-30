package com.jiuyan.detect;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

/**
 * Created by jxt on 17-10-30.
 */

public class TestActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);
        final TextView tv = (TextView)findViewById(R.id.textView);

        final Button bu = (Button) findViewById(R.id.button);
        bu.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String testResult = PrestissimoTestInstance.test(PrestissimoTestInstance.CPU_INT_ST);
                tv.setText(testResult);
            }
        });
        final Button tv1 = (Button) findViewById(R.id.button1);
        tv1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String testResult = PrestissimoTestInstance.test(PrestissimoTestInstance.CPU_FLOAT_ST);
                tv.setText(testResult);
            }
        });
        final Button tv2 = (Button) findViewById(R.id.button2);
        tv2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String testResult = PrestissimoTestInstance.test(PrestissimoTestInstance.CPU_INT_MT);
                tv.setText(testResult);
            }
        });
        final Button tv3 = (Button) findViewById(R.id.button3);
        tv3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String testResult = PrestissimoTestInstance.test(PrestissimoTestInstance.CPU_FLOAT_MT);
                tv.setText(testResult);
            }
        });
    }

}
