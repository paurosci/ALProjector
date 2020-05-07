/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package alp;

/**
 *
 * @author Pau
 */
public class ActivityRegisterEvent extends java.util.EventObject{
    public String Description;
    //here's the constructor             
    public ActivityRegisterEvent(Object source) {                 
        super(source);             
    } 
}
