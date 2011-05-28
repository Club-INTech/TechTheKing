package intech.detection;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.PowerManager;
import android.util.Log;

public class Start extends ConnexionWifi {
	
	private PowerManager.WakeLock m_wl;
	private final static String TAG = "Start Match";
	private MatchSocket matchSocket;
	
	private class MatchSocket extends ServerPions {
		
		protected MatchSocket(int port){
			super(port);
		}
		protected boolean begin() {
			while(true){
				Log.d(TAG, "Waiting for client...");
				accept();
				Log.d(TAG, "Reading...");
				String answer = read();
				Log.d(TAG, "Read : " + answer);
				if(answer.equals("pions")){
					Intent intentPrisePhoto = new Intent(Start.this, PictureTaker.class);
					startActivityForResult(intentPrisePhoto , 0);
				}
			}
		}

	}

	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		m_wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
		m_wl.acquire();
		CameraHandler.getInstance().onOpen();
		CameraHandler.getInstance().startPreview();
		matchSocket = new MatchSocket(42000);
		if(!isConnected){
			finish();
		}
		else{
			new Thread() {
				public void run() {
					matchSocket.begin();				
				}
			}.start();
		}
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		CameraHandler.getInstance().reconnect();
	    switch(requestCode) { 
	    	case (0) :
	    		if (resultCode == Activity.RESULT_OK) {
	    			String listePositions = data.getStringExtra("listePositions");
	    			Log.d(TAG,"Liste des positions : " + listePositions);
	    			matchSocket.write(listePositions);
			    	CameraHandler.getInstance().m_camera.startPreview();
			    	CameraHandler.getInstance().reconnect();
			    	m_wl.acquire();
	    		}
	    		else{
	    			isConnected=false;
	    		}
	    		break;
	    } 
	}

	@Override
	public void onBackPressed(){
	    	super.onDestroy();
	    	matchSocket.close();
	}
	
	
}