/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Graficas;

import daq.autoclave.FramePrincipal;
import daq.autoclave.Variables;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author JuanDavid
 */
public class HiloGrafica implements Runnable{
    
    public static ArrayList Tempx=new ArrayList<>();
    public static ArrayList Tempy=new ArrayList<>();
    public static ArrayList RPMx=new ArrayList<>();    
    public static double Tiempo=0;
    Graficar graph = new Graficar();
    public static boolean estadoAdquisicion=false;
    
    
    @Override
    public void run() {
        while(estadoAdquisicion){
            graph.graficar();
            try {
                Thread.sleep(Variables.Tmuestreo);
            } catch (InterruptedException ex) {
                Logger.getLogger(HiloGrafica.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
}
