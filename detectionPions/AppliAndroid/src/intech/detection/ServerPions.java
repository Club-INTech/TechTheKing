package intech.detection;

import java.io.*;
import java.nio.ByteBuffer;
import java.net.*;
import java.nio.channels.SocketChannel;
import java.nio.channels.WritableByteChannel;
import android.util.Log;

abstract public class ServerPions{

	final static int TIMEOUT_TIME=2000;
	ServerSocket m_serverSocket;
	Socket m_clientSocket;
    private ByteBuffer m_readBuffer;
    private final static String TAG="Serveur Pions";
    
    protected ServerPions(int port){
    	 try {
			m_serverSocket = new ServerSocket(port);
		} catch (IOException e) {
			e.printStackTrace();
		}
    }

    abstract protected boolean begin();
    
    
    protected void createStreams(){
    	
    }
    
    protected void accept(){
    	try{
    		Log.d(TAG,"Waiting for client...");
    	    m_clientSocket = m_serverSocket.accept();
    	    Log.d(TAG,"Client accepted...");
    	    
    	}
    	catch (IOException e) {
    	    Log.d(TAG, "Accept failed");
    	    System.exit(-1);
    	}
    	catch (NullPointerException e) {
    		Log.d(TAG, "Server not created");
    	}
    }
    
    protected void write (String msg){
    	try {
			PrintWriter out = new PrintWriter(
			        m_clientSocket.getOutputStream(), true);
			out.println(msg);
			out.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
    protected String read(){
    	Log.d(TAG,"reading... ");
    	try {
    		BufferedReader m_in = new BufferedReader(
			        new InputStreamReader(
			            m_clientSocket.getInputStream()));
	    	String result=m_in.readLine();
	    	Log.d(TAG,"read : " + result);
			return result;
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
    }
    
    protected void close(){
    	try {
			m_serverSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}
