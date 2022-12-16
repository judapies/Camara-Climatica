/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Graficas;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JTable;
import jxl.Workbook;
import jxl.format.Alignment;
import jxl.format.Colour;
import jxl.read.biff.BiffException;
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
public class ExportarLectura {
    
    @SuppressWarnings("FieldMayBeFinal")
    private File file;
    @SuppressWarnings("FieldMayBeFinal")
    private List<JTable> tabla;
    @SuppressWarnings("FieldMayBeFinal")
    private List<String> nom_files;
    @SuppressWarnings("FieldMayBeFinal")
    private int cargar;

    public ExportarLectura(File file, List<JTable> tabla, List<String> nom_files,int cargar) throws Exception {
        this.file = file;
        this.tabla = tabla;
        this.nom_files = nom_files;
        this.cargar = cargar;
        
        if (nom_files.size() != tabla.size()) {
            throw new Exception("Error");
        }   
    }
    //este es el metodo para exportar los datos del Jtable
    
    public boolean export(){
        try {
            //informacion
            
            //titulos experimento
            final String[] Texperimento = {"Operador","No Identificación","Ciclo","Fecha",
                                         "Hora","Observaciones y/o Notas"};           
            
            //info. Experimento
            final String[] Iexperimento = { Batch.TextExperimentador.getText(), Batch.TextIdentificacion.getText(),
                Batch.TextBatch.getText(),Batch.TextFecha.getText(), Batch.TextHora.getText(),
                Batch.TextObser.getText()};
  
            File inputWorkbook;
            String input="PK.xls";
            inputWorkbook = new File(input);
            
            Workbook w1 = Workbook.getWorkbook(inputWorkbook);
   
            WritableWorkbook w = Workbook.createWorkbook(file, w1);
            //DataOutputStream out= new DataOutputStream(new FileOutputStream(file));
            //WritableWorkbook w= Workbook.createWorkbook(out);
            // Create a cell format for Times 16, bold and italic 
            WritableFont times16font = new WritableFont(WritableFont.ARIAL,
                                           9, WritableFont.BOLD, true); 
            WritableCellFormat times16format = new WritableCellFormat (times16font);
            // Create a cell format for Times 14, no bold and italic 
            WritableFont times14font = new WritableFont(WritableFont.ARIAL,
                                           9, WritableFont.NO_BOLD, true); 
            WritableCellFormat times14format = new WritableCellFormat (times14font);
            
            times16font.setColour(Colour.WHITE);
            //Estilo centrado y con bordes
            WritableCellFormat wcf1 = new WritableCellFormat();
            wcf1.setAlignment(Alignment.CENTRE);
            wcf1.setFont(times16font);
            wcf1.setBackground(Colour.DARK_BLUE2);
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
            final String[] encabezado = {"Fecha","Hora","Temperatura","Temperatura 2","Humedad"};
            
            
            //imprime informacion
            String informacion="Información";
            WritableSheet info= w.createSheet(informacion,1);
                //Establecemos los anchos de las columnas
                info.setColumnView(1, 30);
                info.setColumnView(2, 30);
                info.setColumnView(3, 60);
                info.setColumnView(4, 30);
                info.setColumnView(5, 30);
                info.setColumnView(6, 30);
                info.setColumnView(7, 30);
                info.setColumnView(8, 30);
                info.setColumnView(9, 30);
                
                //encabezado 
                info.addCell(new Label(3,1,"INFORMACION DEL CICLO",wcf1)); 
 
                info.addCell(new Label(1,4,"CICLO",wcf1)); 
                for (int i = 0; i < Texperimento.length ; i++) {
                    info.addCell(new Label(1,i+6,Texperimento[i],wcf1));   
                    info.addCell(new Label(2,i+6,Iexperimento[i],wcf2));
                }             
                
            for (int index=0; index<tabla.size(); index++){
                JTable table= tabla.get(index);
                WritableSheet s = w.getSheet(0);                

                //encabezado 
                for (int i = 0; i < table.getColumnCount(); i++) {
                    s.addCell(new Label(i,4,encabezado[i],wcf1));                    
                }
                
                for (int i = 0; i < table.getColumnCount(); i++) {
                    for (int j = 0; j < table.getRowCount(); j++) {
                        Object object= table.getValueAt(j, i);
                        s.addCell(new Label(i,j+5,String.valueOf(object),wcf2));
                        //if(i!=1 && i!=2 && !"null".equals(String.valueOf(object))){
                        //s.addCell(new jxl.write.Number(i,j+5, Double.valueOf((String) object),wcf2));
                        //}    
                    }
                }
            }
            w.write();
            w.close();
            return true;
        }
        catch (IOException | WriteException e) {
            return false;
        } catch (BiffException ex) {
            Logger.getLogger(Exportar.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }
}


    
   
    
    

