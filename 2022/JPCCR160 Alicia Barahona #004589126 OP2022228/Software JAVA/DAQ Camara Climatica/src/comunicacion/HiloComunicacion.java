/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package comunicacion;

import daq.autoclave.FramePrincipal;
import static daq.autoclave.Variables.cargo;
import jPicUsb.iface;
import javax.swing.JOptionPane;

/**
 *
 * @author DELL
 */
//public class HiloComunicacion implements Runnable{
public class HiloComunicacion{
    // Variables para comunicaciòn USB
    public static byte [] PC_Tx= new byte[64];
    public static int [] PC_Rx= new int[64];
    public static int TMuestreo=1000,IngresoDatos=0; 
    public static int tiempoespera=0;
    public double valorTemperatura,valorTemperatura2,valorHumedad,valorThumedad;
    
    
    public HiloComunicacion(){
        if(!cargo){
            cargarIface();
            cargo=true;
        }
    }
    
    public void cargarIface() {
        try {
            iface.load();
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, ""+e, "Error", JOptionPane.ERROR_MESSAGE);
        }
        iface.set_instance(0);
        iface.set_vidpid("vid_04d8&pid_000b");
    }
    
    public void LeerDato(){
        byte tmp[]=iface.QRead(32, 10);

        if(tmp.length!=0){
            FramePrincipal.BotonUSB.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Imagenes/USB-Verde.png")));
            FramePrincipal.BotonUSB.setToolTipText("USB Conectado");
            PC_Rx=ArrayByteToArrayInt(tmp);
            tiempoespera=0;
            
            valorTemperatura=PC_Rx[0]+(PC_Rx[2]/10.0000);
            valorTemperatura2=PC_Rx[3]+(PC_Rx[4]/10.0000);
            valorHumedad=PC_Rx[5]+(PC_Rx[6]/10.0000);
            valorThumedad=PC_Rx[13]+(PC_Rx[14]/10.0000);
            
            System.out.println("Temperatura:"+this.valorTemperatura);
            /*System.out.println("Pc:"+this.valorPresionCamara);
            System.out.println("Pp:"+this.valorPresionPreCamara);
                    */
            
        }else{
            FramePrincipal.BotonUSB.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Imagenes/USB-Rojo.png")));
            FramePrincipal.BotonUSB.setToolTipText("USB Desconectado");
            if(tiempoespera>=60){
                JOptionPane.showMessageDialog(null, "No hay comunicación\n Verifique la conexion USB", "Error", JOptionPane.ERROR_MESSAGE);
                tiempoespera=0;
            }
            tiempoespera++;
            //System.out.println("No hay comunicación");
        }
    }
    
    public static int ByteToInt8(byte x){
        int valor=x;
        if(valor<0)valor+=256;
        return(valor);
    }
    
    public int[] ArrayByteToArrayInt(byte[] arreglo){
        int tmp[]=new int[64];
        
        for (int i = 0; i < arreglo.length; i++) {
            tmp[i]=ByteToInt8(arreglo[i]);
        }
        
        return tmp;
    } 
    
    public double getTemperatura(){
        return this.valorTemperatura;
    }
    
    
/*
    @Override
    @SuppressWarnings("SleepWhileInLoop")
    public void run() {
        cargarIface();
        while(true){
            try {
                Thread.sleep(2000);
            } catch (InterruptedException ex) {
                Logger.getLogger(HiloComunicacion.class.getName()).log(Level.SEVERE, null, ex);
            }
            LeerDato();
        }
    }*/

    public double getTemperatura2() {
        return valorTemperatura2;
    }

    public double getHumedad() {
        return valorHumedad;
    }
    
    public double getTHumedad() {
        return valorThumedad;
    }
    
    
}
