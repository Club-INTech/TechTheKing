package intech.detection;

import android.graphics.BitmapFactory;
import android.widget.ImageView;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.FrameLayout;

public class DebugCamera extends Activity {
	
	static {  
	    System.loadLibrary("opencv");  
	}  
	
	
	public native byte[] getSourceImage();
	
	private static final String TAG = "CameraDemo";
	CameraPreview preview; // <1>

	ImageView image;
	
    private EditText vueEntry;

    
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    
	    setContentView(R.layout.camera);
	    
	    //Association des champs.
        vueEntry=(EditText) findViewById(R.id.vueEntry);
        
        vueEntry.setText(String.valueOf(CameraHandler.getInstance().getVue()));
	    
	    
	    preview = new CameraPreview(this); // <3>
	    preview.setKeepScreenOn(true);
	    
	    image = (ImageView) findViewById(R.id.imageDebug);
	    
	    ((FrameLayout) findViewById(R.id.preview)).addView(preview); // <4>
	    
  	    
	    preview.setOnClickListener(new OnClickListener() {
	      public void onClick(View v) {
	    	
  			Intent intentPrisePhoto = new Intent(DebugCamera.this, PictureTaker.class);
			
  	    	CameraHandler.getInstance().setVue(Integer.parseInt(vueEntry.getText().toString()));
  	    	
			startActivityForResult(intentPrisePhoto , 0);
			
			
	      }
	    });
	    
	    
	    Log.d(TAG, "onCreate'd");
	}

	
    @Override 
    public void onActivityResult(int requestCode, int resultCode, Intent data) {

	  CameraHandler.getInstance().reconnect();
		
      switch(requestCode) { 
        case (0) : { 
          if (resultCode == Activity.RESULT_OK) {
        	  image.setImageBitmap(BitmapFactory.decodeByteArray(getSourceImage(), 0, getSourceImage().length));
          }
          break;
        } 
      } 
    }
    
    @Override
    public void onBackPressed(){
    	super.onDestroy();
    	Intent resultIntent = new Intent();
    	CameraHandler.getInstance().setVue(Integer.parseInt(vueEntry.getText().toString()));
    	setResult(Activity.RESULT_OK, resultIntent);
    	finish();
    }
	  
    
}

