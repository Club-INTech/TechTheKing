package intech.detection;

import java.io.IOException;

import android.hardware.Camera;
import android.view.SurfaceHolder;

public class CameraHandler {
	
	private static CameraHandler instance;

	
	protected Camera m_camera;
	private boolean isPreviewStarted;
	
	private int m_vue;
	
	public static CameraHandler getInstance() {
        if (null == instance) { // Premier appel
            synchronized(CameraHandler.class) {
                if (null == instance) {
                    instance = new CameraHandler();
                }
            }
        }
        return instance;
    }
	
	public void setVue(int vue){
		m_vue = vue;
	}
	
	public int getVue(){
		return m_vue;
	}
	
	private CameraHandler(){
		isPreviewStarted = false;
		m_vue = 5;
	}
	
	protected void onOpen(){
		if(m_camera==null){
			m_camera = Camera.open(); // <8>
			m_camera.setDisplayOrientation(90);
	    	Camera.Parameters parameters = m_camera.getParameters();
	    	parameters.setPictureSize(640, 480);  
	    	m_camera.setParameters(parameters);
		}
	}
	
	protected void setPreviewDisplay(SurfaceHolder holder){
		 try{
			 m_camera.setPreviewDisplay(holder);  // <9>
		 }
		 catch (IOException e) { // <13>
			 e.printStackTrace();
		 }
	}
	
	protected void startPreview(){
		if(!isPreviewStarted){
			m_camera.startPreview();
			isPreviewStarted=true;
		}
	}
	protected void reconnect(){
		try {
			m_camera.reconnect();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	protected void kill(){
		m_camera.stopPreview();
	  	m_camera.release();
	  	m_camera = null;
	}
}

