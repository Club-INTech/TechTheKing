package intech.detection;


import android.content.Context;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

class CameraPreview extends SurfaceView implements SurfaceHolder.Callback { // <1>

  SurfaceHolder mHolder;  // <2>
  
  CameraPreview(Context context) {
    super(context);

    // Install a SurfaceHolder.Callback so we get notified when the
    // underlying surface is created and destroyed.
    mHolder = getHolder();  // <4>
    mHolder.addCallback(this);  // <5>
    mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS); // <6>
  }

  // Called once the holder is ready
  public void surfaceCreated(SurfaceHolder holder) {  // <7>
    // The Surface has been created, acquire the camera and tell it where
    // to draw.
    
	CameraHandler.getInstance().onOpen();
    
    CameraHandler.getInstance().setPreviewDisplay(holder);
    
  }

  // Called when the holder is destroyed
  public void surfaceDestroyed(SurfaceHolder holder) {  // <14>
	  
  }

  // Called when holder has changed
  public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) { // <15>
	  CameraHandler.getInstance().m_camera.startPreview();
  }

}