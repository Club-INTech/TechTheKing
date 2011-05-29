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
	
    private EditText seuilEntry;
    private EditText morphRowsEntry;
    private EditText morphColsEntry;

    
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    
	    setContentView(R.layout.camera);
	    
	    //Association des champs.
        seuilEntry=(EditText) findViewById(R.id.seuilEntry);
        morphRowsEntry=(EditText) findViewById(R.id.morphRowsEntry);
        morphColsEntry=(EditText) findViewById(R.id.morphColsEntry);
        
        seuilEntry.setText(String.valueOf(CameraHandler.getInstance().getSeuil()));
        morphRowsEntry.setText(String.valueOf(CameraHandler.getInstance().getMorphRows()));
        morphColsEntry.setText(String.valueOf(CameraHandler.getInstance().getMorphCols()));
	    
	    
	    preview = new CameraPreview(this); // <3>
	    preview.setKeepScreenOn(true);
	    
	    image = (ImageView) findViewById(R.id.imageDebug);
	    
	    ((FrameLayout) findViewById(R.id.preview)).addView(preview); // <4>
	    
  	    
	    preview.setOnClickListener(new OnClickListener() {
	      public void onClick(View v) {
	    	
  			Intent intentPrisePhoto = new Intent(DebugCamera.this, PictureTaker.class);
			
  	    	CameraHandler.getInstance().setSeuil(Integer.parseInt(seuilEntry.getText().toString()));
  	    	CameraHandler.getInstance().setMorphRows(Integer.parseInt(morphRowsEntry.getText().toString()));
  	    	CameraHandler.getInstance().setMorphCols(Integer.parseInt(morphColsEntry.getText().toString()));
  	    	
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
    	CameraHandler.getInstance().setSeuil(Integer.parseInt(seuilEntry.getText().toString()));
    	CameraHandler.getInstance().setMorphRows(Integer.parseInt(morphRowsEntry.getText().toString()));
    	CameraHandler.getInstance().setMorphCols(Integer.parseInt(morphColsEntry.getText().toString()));
    	setResult(Activity.RESULT_OK, resultIntent);
    	finish();
    }
	  
    
}

