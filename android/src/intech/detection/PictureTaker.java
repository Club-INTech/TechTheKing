package intech.detection;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.os.Bundle;
import android.util.Log;
import android.hardware.Camera.ShutterCallback;
import android.app.Activity;
import android.content.Intent;

public class PictureTaker extends Activity {

	static {  
	    System.loadLibrary("opencv");  
	}  
	public native boolean setSourceImage(int[] pixels, int width, int height);
	public native String trouverPions(int vue);
	
	private final static String  TAG = "Picture Taker";
	
	private int m_vue;
	
	ShutterCallback shutterCallback = new ShutterCallback() { // <6>
		    public void onShutter() {
		      Log.d(TAG, "onShutter'd");
		    }
		  };

		  // Handles data for raw picture
	PictureCallback rawCallback = new PictureCallback() { // <7>
		    public void onPictureTaken(byte[] data, Camera camera) {   
		    	Log.d(TAG, "onPictureTaken - raw");
		    }
		  };
	private PictureCallback m_jpegCallback = new PictureCallback() {
		
	    public void onPictureTaken(byte[] data, Camera camera){
	    	try{	
		    	Bitmap bitmap = BitmapFactory.decodeByteArray(data,0,data.length);
		    	
		    	
		    	
			    Matrix mat = new Matrix();
			    mat.postRotate(90);
			    Bitmap bitmapRotate = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), mat, true);
			    int width = bitmapRotate.getWidth();
			    int height = bitmapRotate.getHeight();
			    int[] pixels = new int[width * height];
			    bitmapRotate.getPixels(pixels, 0, width, 0, 0, width, height);
			    setSourceImage(pixels,width,height);
		    	
			    /*
			    //Pour le Galaxy S :
			    if(m_vue==5 || m_vue==3){
	    			}
		    	
		    	//Pour tous les autres
		    	else{
			    	int width = bitmap.getWidth();
			    	int height = bitmap.getHeight();
			    	int[] pixels = new int[width * height];
			    	bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
			    	setSourceImage(pixels,width,height);
		    	}
		    	*/
		    	String listePositions = trouverPions(m_vue);
		    	Intent resultIntent = new Intent();
		    	resultIntent.putExtra("listePositions", listePositions);
		    	setResult(Activity.RESULT_OK, resultIntent);
		    	finish();
		    	
		    }
		    catch (Exception e) { // <10>
		        e.printStackTrace();
		    }
		    finally {   	
		    }
		    	
		}
	};
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);

	    m_vue = CameraHandler.getInstance().getVue();

	    CameraHandler.getInstance().m_camera.takePicture(shutterCallback, rawCallback, m_jpegCallback);
	}
}
