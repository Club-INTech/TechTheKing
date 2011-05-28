package intech.detection;

import java.io.IOException;

import android.hardware.Camera;
import android.view.SurfaceHolder;

public class CameraHandler {
	
	private static CameraHandler instance;

	
	protected Camera m_camera;
	private boolean isPreviewStarted;
	
	private int m_seuil;
	private int m_morphRows;
	private int m_morphCols;
	
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
	
	public void setSeuil(int seuil){
		m_seuil = seuil;
	}
	
	public void setMorphRows(int morphRows){
		m_morphRows = morphRows;
	}
	
	public void setMorphCols(int morphCols){
		m_morphCols = morphCols;
	}
	
	public int getSeuil(){
		return m_seuil;
	}
	
	public int getMorphRows(){
		return m_morphRows;
	}
	
	public int getMorphCols(){
		return m_morphCols;
	}
	
	private CameraHandler(){
		isPreviewStarted = false;
		m_seuil=0;
		m_morphRows=0;
		m_morphCols=0;
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
