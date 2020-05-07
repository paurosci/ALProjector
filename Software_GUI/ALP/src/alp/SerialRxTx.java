/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package alp;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.swing.JOptionPane;

/**
 *
 * @author Pau
 */
public class SerialRxTx implements SerialPortEventListener{
    private OutputStream serialOut = null;
    private InputStream serialIn = null;
        //public BufferedReader input;
    public SerialPort port = null; 
    private final int data_rate; 
    private final String serialCOM;
    private String motor_rate =""; 
    
    public SerialRxTx(String serialCOM,int data_rate) throws Exception{
        this.serialCOM = serialCOM; 
        this.data_rate = data_rate; 
        this.initialize(); 
    }
    /**
     *
     */
       
    private void initialize() throws Exception{

        CommPortIdentifier portId;
        portId = CommPortIdentifier.getPortIdentifier(this.serialCOM);
        System.out.println(this.serialCOM);
        System.out.println(this.data_rate);
        port = (SerialPort) portId.open("Port COM", this.data_rate); 
        
        serialOut = port.getOutputStream();
        port.setSerialPortParams(this.data_rate, SerialPort.DATABITS_8, 
        SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        
        //create EventListener
        port.addEventListener((SerialPortEventListener) this);
        port.notifyOnDataAvailable(true);
        
        //initialize values
        serialIn = port.getInputStream();
        serialOut = port.getOutputStream();
    }
    
    public void close()throws Exception{
            port.removeEventListener();
            port.close();
            serialOut.close();
            serialIn.close();
    }

    public void sendData(String option){
        try{
            
            byte[] enc = new byte[option.length()]; 
            for(int i = 0; i<option.length();i++){
                enc[i]=(byte)option.charAt(i);
                System.out.println(i); 
            }
            
            ActivityRegister.getActivityRegister().addActivity("Sending command: "+option);
            serialOut.write(enc); 

            }catch(IOException ex){
            JOptionPane.showMessageDialog(null, "Not possible send data",
                    "Send data", JOptionPane.PLAIN_MESSAGE);
        }
    }

    @Override
    public synchronized void serialEvent(SerialPortEvent evt) { 
        if(evt.getEventType()==SerialPortEvent.DATA_AVAILABLE){
            try{
                             
                char dec; 
                int inputVal = (int)serialIn.read(); 
                
                if(inputVal == 13){
                    ActivityRegister.getActivityRegister().addSerialInputMR(motor_rate);
                    //System.out.println(motor_rate);
                    motor_rate = ""; 
                }
                
                else{
                    dec = (char)inputVal; 
                    motor_rate+=String.valueOf(dec); 
                }
                
            }catch(Exception e){
                System.err.println(e.toString() );
            }
        }

    }
}