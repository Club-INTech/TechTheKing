package intech.detection;

import android.graphics.Bitmap;

public class InterfaceResults {
	public Bitmap m_imageDebug;
	public String m_positionsPions;
	
	public InterfaceResults(Bitmap imageDebug,String positionsPions){
		m_imageDebug = imageDebug;
		m_positionsPions = positionsPions;
	}
	
	public InterfaceResults(){
		m_imageDebug = null;
		m_positionsPions = "";
	}
}
