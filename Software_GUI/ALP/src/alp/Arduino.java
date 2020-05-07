/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package alp;

import gnu.io.CommPortIdentifier;
import java.util.ArrayList;
import java.util.Enumeration;
import javax.swing.JButton;

/**
 *
 * @author Pau
 */
public class Arduino {
    private SerialRxTx arduino; 

    public Arduino(){
        
    }
    public ArrayList getPortList(){
        ArrayList portList = new ArrayList();
        Enumeration pList = CommPortIdentifier.getPortIdentifiers();
        while (pList.hasMoreElements()) {
            CommPortIdentifier cpi = (CommPortIdentifier) pList.nextElement();
            if (cpi.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                portList.add(cpi.getName());
            }
        }
        return portList; 
    }
    public void OpenCommArduino(String serialCOM, int data_rate) throws Exception{
        arduino = new SerialRxTx(serialCOM,data_rate);
    }
    
    public void CloseArduinoComm() throws Exception{
        arduino.close();
    }
    
    public void CommArduino(String value) throws Exception{
        System.out.println("Sendig command: " + value);

        arduino.sendData(value);
 
    }
    
    
}
