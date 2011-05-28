package intech.detection;

import android.app.Activity;
import android.os.Bundle;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.content.Context;
import java.util.List;
import android.net.wifi.ScanResult;
import android.util.Log;
import android.net.ConnectivityManager;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiInfo;
import android.view.Gravity;
import android.widget.Toast;

public class ConnexionWifi extends Activity {
	private String ssid;
	private String wpa_psk;

	private WifiManager wifiManager;
	private WifiConfiguration tmpConfig;
	private ConnectivityManager connectivityManager;
	private String raisonEchec;
	
	protected boolean isConnected;
	
	private final String TAG = "Connexion Wifi";
	
	
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);		
		
		raisonEchec = "";
		isConnected = false;
		
		Bundle bundle = getIntent().getExtras();
	    wpa_psk = bundle.getString("wpa");
	    ssid = bundle.getString("ssid");
	    
		

        //Active le wifi si il n'est pas activ�.
    	activateWifi();
    	
    	//Connection au r�seau wifi ssid;
        if(associateWifi()){ 
        	Log.d(TAG, "Association Réussie");
            	
            //Attente que la connection soit effectivement effectuée.
        	waitToConnect();
        	Log.d(TAG, "Connection Réussie");
        	isConnected = true;
        	showToast("Connection à " + ssid + " réussie!");
        	
        	//Affichage de quelques informations de debug
        	printInfos();
    		
    		
    	}
        else{
        showToast("Echec de la connexion : " + raisonEchec);
        }
	}
	
    protected void showToast(String msg ){
        Toast toast = Toast.makeText(ConnexionWifi.this, msg, Toast.LENGTH_SHORT);
        toast.setGravity(Gravity.CENTER, 0, 0);
        toast.show();
    }
    
	private void activateWifi(){
		wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
    	connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);

    	if(wifiManager.getWifiState()==WifiManager.WIFI_STATE_ENABLED){
		}
		else{
			showToast("Wifi non activé. Activation..");
			if(wifiManager.setWifiEnabled(true)){
				while(wifiManager.getWifiState()==WifiManager.WIFI_STATE_ENABLING);
				if(wifiManager.getWifiState()==WifiManager.WIFI_STATE_UNKNOWN){
					raisonEchec = "Impossible d'activer le wifi...";
				}
				else{
			        try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
			else{
				raisonEchec = "Impossible d'activer le wifi...";
			}
		}
	}

    private boolean associateWifi(){
    	List<ScanResult> hotSpots = wifiManager.getScanResults();
    	for(ScanResult hotSpot: hotSpots) {
    		String hotSpotSsid = hotSpot.SSID;
    		Log.d(TAG,hotSpotSsid);
    		if(hotSpotSsid.equals(ssid)) {
    			tmpConfig = new WifiConfiguration();
    			tmpConfig.SSID ="\"" + ssid + "\"";
    			tmpConfig.preSharedKey = "\"" + wpa_psk + "\"";
    			tmpConfig.hiddenSSID = false;
    			tmpConfig.status = WifiConfiguration.Status.ENABLED;
    			tmpConfig.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
    			tmpConfig.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
    			
    			int netId =wifiManager.addNetwork(tmpConfig);
    			if(netId<0){
    				raisonEchec = "Echec de création du profil temporaire : ( SSID = " + tmpConfig.SSID + ")";
    				return false;
    			}
    			else{
    			}
    			return wifiManager.enableNetwork(netId,false);
    		}
    	}
    	raisonEchec = "Réseau " + ssid + " non trouvé.";
    	return false;
    }

    
    private void waitToConnect(){
    	WifiInfo wifiInfo = wifiManager.getConnectionInfo();
    	while(wifiInfo.getSupplicantState()!=SupplicantState.COMPLETED);
    }
    
    private void printInfos(){
    	Log.d(TAG,wifiManager.getConnectionInfo().toString());
    	Log.d(TAG, wifiManager.getDhcpInfo().toString());
		Log.d(TAG, connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).toString());
    }
}
