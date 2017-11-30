package com.jiuyan.detect;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;

public class ImageActivity extends AppCompatActivity {

    private static final int IMAGE_TYPE_NUMBER = 1000;

    private long mNetInstance = 0;
    private static final int SELECT_IMAGE = 1;
    private Bitmap mBitmap = null;
    private ImageView mBitmapView = null;
    private TextView mTextView = null;


    private String[] mImageTypes = new String[IMAGE_TYPE_NUMBER];

    private void readWords() throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(getAssets().open("synset_words.txt")));
        for (int i=0; i<IMAGE_TYPE_NUMBER; ++i) {
            mImageTypes[i] = bufferedReader.readLine();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_image);
        try {
            readWords();
        } catch (IOException e) {
            e.printStackTrace();
        }

        mNetInstance = PrestissimoTestInstance.createNetInstance(PrestissimoTestInstance.CPU_INT_ST, "/sdcard/prestissimo/squeezenet.prestissimo");

        ((Button) findViewById(R.id.imageSelect)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_PICK);
                intent.setType("image/*");
                startActivityForResult(intent, SELECT_IMAGE);
            }
        });

        mBitmapView = (ImageView)findViewById(R.id.imageView);
        mTextView = (TextView)findViewById(R.id.resultTest);
        ((Button) findViewById(R.id.imagePredict)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (null != mBitmap) {
                    float[] result = PrestissimoTestInstance.predict(mNetInstance, mBitmap);
                    int maxPos = 0;
                    float maxValue = result[0];
                    for (int i=1; i<result.length; ++i) {
                        if (maxValue < result[i]) {
                            maxValue = result[i];
                            maxPos = i;
                        }
                    }
                    String resultString = "Type = " + mImageTypes[maxPos] + ", prop = " + maxValue;
                    mTextView.setText(resultString);
                }
            }
        });
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();

        PrestissimoTestInstance.destroyNetInstance(mNetInstance);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && null != data) {
            Uri selectimage = data.getData();

            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inJustDecodeBounds = true;

            try {
                BitmapFactory.decodeStream(getContentResolver().openInputStream(selectimage), null, options);
                int REQUIREDSIZE = 450;
                int scale = 1;
                if (options.outHeight > REQUIREDSIZE) {
                    scale = (int)Math.pow(Math.ceil(Math.log(((double)(options.outHeight) / (double)REQUIREDSIZE))/Math.log(2.0)), 2.0);
                }

                options = new BitmapFactory.Options();
                options.inSampleSize = scale;
                Bitmap originBitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(selectimage), null, options);
                Bitmap realBitmap = Bitmap.createBitmap(227, 227, Bitmap.Config.ARGB_8888);
                Canvas canvas = new Canvas(realBitmap);
                canvas.drawBitmap(originBitmap, new Rect(0, 0, originBitmap.getWidth(), originBitmap.getHeight()), new Rect(0, 0, 227, 227), null);
                mBitmap = realBitmap;

                mBitmapView.setImageBitmap(mBitmap);



            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }

        }
    }
}
