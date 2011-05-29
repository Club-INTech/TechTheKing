package intech.detection;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainMenu extends Activity {

    private EditText portEntry;
    private EditText ssidEntry;
    private EditText wpaEntry;
    
    private int m_vue;
    
    private int m_port;
    private String m_ssid;
    private String m_wpa;
    
    private Bundle bundle;
    
    Button buttonClick;
    
    String m_res;
    
	  /** Called when the activity is first created. */
	  @Override
	  public void onCreate(Bundle savedInstanceState) {
    	
    	/*affichage de l'application*/
		
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        /*association des champs*/
        portEntry=(EditText) findViewById(R.id.portEntry);
        ssidEntry=(EditText) findViewById(R.id.ssidEntry);
        wpaEntry=(EditText) findViewById(R.id.wpaEntry);
        
        buttonClick = (Button) findViewById(R.id.lancer);
        
        /*Initialisation des constantes*/
        
        CameraHandler.getInstance().setVue(5);
        
        m_port = 42000;
        m_ssid = "Test";
        m_wpa = "passphrase";
        
        /*affichage des constantes.*/      
        portEntry.setText(String.valueOf(m_port));
        ssidEntry.setText(m_ssid);
        wpaEntry.setText(m_wpa);
        
        bundle = new Bundle();
        
    	 
    	
        buttonClick.setOnClickListener(new OnClickListener() {
  	      public void onClick(View v) {
  	    	  Bundle bundle = new Bundle();
  	    	  bundle.putInt("port", m_port);
  	    	  bundle.putString("ssid", m_ssid);
  	    	  bundle.putString("wpa", m_wpa);
  	    	  Intent intentStart = new Intent(MainMenu.this, Start.class);
  	    	  intentStart .putExtras(bundle);	
  	    	  startActivityForResult(intentStart , 0);
  	      }
  	    });
	  }
	  
	  
	  
	  // Cr�ation des menus
	  public boolean onCreateOptionsMenu(Menu menu) {
		  
	        MenuInflater inflater = getMenuInflater();
	        
	        inflater.inflate(R.menu.menu, menu);
	 
	        return true;
	   }
	  
      public boolean onOptionsItemSelected(MenuItem item) {
         
         switch (item.getItemId()) 
         {
            case R.id.debugWifi:
            	
            	Intent intentDebugWifi = new Intent(MainMenu.this, DebugWifi.class);		
            	bundle.putInt("port", m_port);
            	bundle.putString("ssid", m_ssid);
            	bundle.putString("wpa", m_wpa);
            	intentDebugWifi.putExtras(bundle);
    			startActivityForResult(intentDebugWifi, 0);
    			return true;
               
            case R.id.debugCamera:
            	
    			Intent intentDebugCamera = new Intent(MainMenu.this, DebugCamera.class);
    			intentDebugCamera.putExtras(bundle);
    			startActivityForResult(intentDebugCamera, 1);
                return true;
         }
         return false;
      }
      
      @Override 
      public void onActivityResult(int requestCode, int resultCode, Intent data) {    
        switch(requestCode) { 
          case (1) : { 
            if (resultCode == Activity.RESULT_OK) { 
                bundle.putInt("vue", data.getIntExtra("seuil", m_vue));
            } 
            break; 
          } 
        } 
      }
}


