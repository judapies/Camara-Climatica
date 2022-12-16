/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Graficas;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;
import javax.swing.JTable;
import jxl.Workbook;
import jxl.format.Alignment;
import jxl.format.Colour;
import jxl.write.Label;
import jxl.write.WritableCellFormat;
import jxl.write.WritableFont;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import jxl.write.WriteException;

/**
 *
 * @author Ivan
 */
public class Exportar {
     
    private File file;
    private List<JTable> tabla;
    private List<String> nom_files;
 
    public Exportar(File file, List<JTable> tabla, List<String> nom_files) throws Exception {
        this.file = file;
        this.tabla = tabla;
        this.nom_files = nom_files;
         
        if (nom_files.size() != tabla.size()) {
            throw new Exception("Error");
        }   
    }
    //este es el metodo para exportar los datos del Jtable
     
    public boolean export(){
        try {
            DataOutputStream out= new DataOutputStream(new FileOutputStream(file));
            WritableWorkbook w= Workbook.createWorkbook(out);
            // Create a cell format for Times 16, bold and italic 
            WritableFont times16font = new WritableFont(WritableFont.TIMES,
                                           16, WritableFont.BOLD, true); 
            WritableCellFormat times16format = new WritableCellFormat (times16font);
            // Create a cell format for Times 14, no bold and italic 
            WritableFont times14font = new WritableFont(WritableFont.TIMES,
                                           14, WritableFont.NO_BOLD, true); 
            WritableCellFormat times14format = new WritableCellFormat (times14font);
                 
            //Estilo centrado y con bordes
            WritableCellFormat wcf1 = new WritableCellFormat();
            wcf1.setAlignment(Alignment.CENTRE);
            wcf1.setFont(times16font);
            wcf1.setBackground(Colour.BLUE);
            wcf1.setBorder(jxl.format.Border.ALL,
                           jxl.format.BorderLineStyle.THIN,
                           jxl.format.Colour.GRAY_80);   
             
            //Estilo centrado y con bordes
            WritableCellFormat wcf2 = new WritableCellFormat();
            wcf2.setAlignment(Alignment.CENTRE);
            wcf2.setFont(times14font);
            wcf2.setBorder(jxl.format.Border.ALL,
                           jxl.format.BorderLineStyle.THIN,
                           jxl.format.Colour.GRAY_80);
 
            //Primera columna
            final String[] encabezado = {"Acido","Base","Foam","Medio",
                                      "CO2","OD","Agitacion",
                                      "Temperatura","pH", "desvio","Ganancia",
                                        "TCiclo", "Offset Temp",
                                        "Banda Agi", "Vel. Respu", "Offset Agi","Media Agi","Media Temp",
                                        "Offset pH","Media pH","Offset OD","Media OD","Offset CO2","Media CO2",
                                        "TAcido", "TBase","TFoam","Banda"};
             
                 
            for (int index=0; index<tabla.size(); index++){
                JTable table= tabla.get(index);
                WritableSheet s= w.createSheet(nom_files.get(index),0);
                //Establecemos los anchos de las columnas
                s.setColumnView(1, 30);
                s.setColumnView(2, 25);
                s.setColumnView(3, 25);
                s.setColumnView(4, 25);
                s.setColumnView(5, 25);
                s.setColumnView(6, 25);
                s.setColumnView(7, 25);
                s.setColumnView(8, 25);
                s.setColumnView(9, 25);
                s.setColumnView(10, 25);
                s.setColumnView(11, 25);
                s.setColumnView(12, 25);
                s.setColumnView(13, 25);
                s.setColumnView(14, 25);
                s.setColumnView(15, 25);
                s.setColumnView(16, 25);
                s.setColumnView(17, 25);
                s.setColumnView(18, 25);
                s.setColumnView(19, 25);
                s.setColumnView(20, 25);
                s.setColumnView(21, 25);
                s.setColumnView(22, 25);
                s.setColumnView(23, 25);
                s.setColumnView(24, 25);
                s.setColumnView(25, 25);
                s.setColumnView(26, 25);
                s.setColumnView(27, 25);
                s.setColumnView(28, 25);
                
                //encabezado 
                for (int i = 0; i < table.getColumnCount(); i++) {
                    s.addCell(new Label(i+1,1,encabezado[i],wcf1));                    
                }
                 System.out.println(""+table.getColumnCount());
                 System.out.println(""+table.getRowCount());
                for (int i = 0; i < table.getColumnCount(); i++) {
                    for (int j = 0; j < table.getRowCount(); j++) {
                        Object object= table.getValueAt(j, i);
                        s.addCell(new Label(i+1,j+2,String.valueOf(object),wcf2));
                        if(i!=1 && i!=2 && !"null".equals(String.valueOf(object))){
                        s.addCell(new jxl.write.Number(i+1,j+2, Double.valueOf((String) object),wcf2));
                        }    
                    }
                }
            }
            w.write();
            w.close();
            return true;
        }
        catch (IOException | WriteException e) {
            return false;
        }
    }
}
 
