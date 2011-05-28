package intech.detection;

import android.os.Bundle;
import android.util.Log;

public class DebugWifi extends ConnexionWifi {
	
	private final static String TAG = "Débug Wifi";
	private int m_port;
	
	private class DebugSocket extends ServerPions {
		
		protected DebugSocket(int port){
			super(port);
		}
		protected boolean begin() {
			String result;
			accept();
			result = read();
			write("Bien reçu");
			return result.equals("test");
		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		if(!isConnected){
			finish();
		}
		else{
			m_port = getIntent().getExtras().getInt("port");
			DebugSocket debugSocket = new DebugSocket(m_port);	
			if(debugSocket.begin()){
				showToast("Test réussi");
				finish();
			}
			else{
				showToast("Echec du test");
				finish();
			}
		}
	}
}
