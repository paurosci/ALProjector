/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package alp;

import java.io.File;
import java.util.ArrayList;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.Document;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.NodeList;
/**
 *
 * @author Pau
 */
public class XMLprocess {
    private final File xmlFile; 
    public String serialCOM; 
    public String baud_rate;
    public String spin_rate;
    public String spin_direction; 
    public ArrayList<String> special_char = new ArrayList<String>();
    
    //constuctor
    public XMLprocess(File xmlFile){
        this.xmlFile = xmlFile; 
        this.getXMLvalues(); 
    }
    
    private void getXMLvalues(){
        try {
                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder  = dbFactory.newDocumentBuilder();                        
                Document doc = dBuilder.parse(this.xmlFile);
                doc.getDocumentElement().normalize();
                //Get first node "Config_Values"
                NodeList nodeList = doc.getElementsByTagName(doc.getDocumentElement().getNodeName());
                              
                Node nNode = nodeList.item(0);
                if (nNode.getNodeType() == Node.ELEMENT_NODE) {
                     Element eElement = (Element) nNode;
                     this.serialCOM = eElement.getElementsByTagName("SerialId").item(0).getTextContent();
                     this.baud_rate = eElement.getElementsByTagName("Baud_Rate").item(0).getTextContent();
                     this.spin_rate = eElement.getElementsByTagName("Spin_Rate").item(0).getTextContent();
                     this.spin_direction = eElement.getElementsByTagName("Spin_Direction").item(0).getTextContent();                
                }

            }catch(Exception e){
                 e.printStackTrace();
            }             
    }    
}

