/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package alp;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Pau
 */
public class ActivityRegister {
    private static ActivityRegister myActivityRegister;
    private ArrayList Activities;
    private ArrayList SerialInputMR; 
    private final List Listeners = new ArrayList();
    private final List SerialInputMRListeners = new ArrayList();
    
    public static ActivityRegister getActivityRegister()
    {
        if (myActivityRegister == null)
        {
            myActivityRegister = new ActivityRegister();
        }
        return myActivityRegister;
    }

    public void addActivity(String Description)
    {
        if (Activities == null)
        {
            Activities = new ArrayList();
        }     
        Activities.add(Description);
        fireEvent(Description);
    }
    
    public void addSerialInputMR(String Description){
        if(SerialInputMR == null){
            SerialInputMR = new ArrayList(); 
        }
        SerialInputMR.add(Description); 
        fireSerialInputMREvent(Description);
    }
    
    public int getActivityRegisterLength()
    {
        return Activities.size();
    }
    
    public String GetDescription(int Pos)
    {
        return Activities.get(Pos).toString();
    }
    
    public synchronized void addEventListener(ActivityRegisterListener listener)  {    
        Listeners.add(listener);  
    }  
    public synchronized void removeEventListener(ActivityRegisterListener listener)   {    
        Listeners.remove(listener);  
    }   
    
    public synchronized void addSerialInputMREventListener (ActivityRegisterListener listener){
        SerialInputMRListeners.add(listener); 
    }
    
    public synchronized void removeSerialInputMREventListener(ActivityRegisterListener listener){
        SerialInputMRListeners.remove(listener); 
    }
// call this method whenever you want to notify  
//the event listeners of the particular event  
    private synchronized void fireEvent(String Description) 
    {    
        ActivityRegisterEvent event = new ActivityRegisterEvent(this);    
        event.Description = Description;
        System.out.println(event.Description);
        Iterator i = Listeners.iterator();    
        while(i.hasNext())  
        {      
            ((ActivityRegisterListener) i.next()).EventAdded(event);    
        }  
    }
    
    private synchronized void fireSerialInputMREvent(String Description){
        SerialInputMREvent event = new SerialInputMREvent(this);  
        event.Description = Description;
        Iterator i = SerialInputMRListeners.iterator();    
        while(i.hasNext()){
            ((SerialInputMRListener) i.next()).EventAdded(event);    
        }
        
    }
}
