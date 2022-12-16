/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Graficas;

import comunicacion.HiloComunicacion;
import daq.autoclave.FramePrincipal;
import static daq.autoclave.FramePrincipal.Grafica;
import static daq.autoclave.FramePrincipal.textTemp;
import static daq.autoclave.FramePrincipal.textTemp2;
import static daq.autoclave.FramePrincipal.txtDP;
import static daq.autoclave.FramePrincipal.txtPc;
import static daq.autoclave.FramePrincipal.txtPc1;
import daq.autoclave.Variables;
import java.awt.BasicStroke;
import java.util.ArrayList;
import java.util.List;
import java.awt.Color;
import java.awt.geom.Rectangle2D;
import static java.lang.Math.log10;
import java.util.Calendar;
import java.util.Formatter;
import java.util.GregorianCalendar;
import javax.swing.table.DefaultTableModel;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartMouseEvent;
import org.jfree.chart.ChartMouseListener;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.panel.CrosshairOverlay;
import org.jfree.chart.plot.Crosshair;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.general.DatasetUtilities;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.ui.RectangleEdge;
/**
 *
 * @author Free
 */
public class Graficar implements ChartMouseListener{
    public List<Double> datosTemperatura = new ArrayList<>();
    public List<Double> datosTemperatura2 = new ArrayList<>();
    public List<Double> datosHumedad = new ArrayList<>();
    public List<Double> datosTHumedad = new ArrayList<>();
    public List<Double> datosTiempo = new ArrayList<>();
    public double n=0, nMuestras=100;
    public int tamaño=0;
    public static DefaultTableModel modelo2;
    HiloComunicacion data = new HiloComunicacion();
    private ChartPanel chartPanel;
    private Crosshair xCrosshair;
    private Crosshair yCrosshair;
    private JFreeChart chart;
    
    public Graficar(){
        modelo2 = new DefaultTableModel();
        FramePrincipal.taba2.setModel(modelo2);
        String header[] = new String[] { "Fecha", "Hora", "Temperatura (°C)","Temperatura 2 (°C)","Humedad %"};
        modelo2.setColumnIdentifiers(header);
    }
        
    public void graficar(){
        data.LeerDato();
        datosTemperatura.add(data.getTemperatura());
        datosTemperatura2.add(data.getTemperatura2());
        datosHumedad.add(data.getHumedad());
        textTemp.setText("Temperatura:"+data.getTemperatura()+"°C ---");
        textTemp2.setText("Temperatura 2:"+data.getTemperatura2()+"°C ---");
        txtPc.setText("Humedad:"+data.getHumedad()+"% ---");
        txtPc1.setText("TH:"+data.getTHumedad()+"°C ---");
        txtDP.setText("DP:"+(data.getTemperatura()+(35.0*log10(data.getHumedad()/100.0)))+"°C ---");
        n++;
        crearTiempo();        
        CargaTabla();
        actualizarGrafica();
    }

    public void CargaTabla(){
        tamaño=datosTiempo.size();
        Calendar cal = new GregorianCalendar();
        int hora=cal.get(Calendar.HOUR_OF_DAY);
        int minuto=cal.get(Calendar.MINUTE);
        int segundo=cal.get(Calendar.SECOND);
        Formatter formato2H = new Formatter();  // Formato de la hora
        formato2H.format("%02d", hora);
        String hora2 = formato2H.toString();    // String de la hora dos cifras
        Formatter formato2M = new Formatter();
        formato2M.format("%02d", minuto);
        String minuto2 = formato2M.toString();  // String de minutos dos cifras
        Formatter formato2S = new Formatter();
        formato2S.format("%02d", segundo);
        String segundo2 = formato2S.toString(); // String de segundos dos cifras
        String []VectorLectura;
        VectorLectura = new String[10];
        VectorLectura[0]=cal.get(Calendar.DAY_OF_MONTH)+"/"+(cal.get(Calendar.MONTH)+1)+"/"+cal.get(Calendar.YEAR);
        VectorLectura[1]=(hora2+":"+minuto2+":"+segundo2);
        VectorLectura[2]=Double.toString((double)datosTemperatura.get(tamaño-1));
        VectorLectura[3]=Double.toString((double)datosTemperatura2.get(tamaño-1));        
        VectorLectura[4]=Double.toString((double)datosHumedad.get(tamaño-1));        
        modelo2.addRow(VectorLectura);
    }
    private void crearTiempo() {
        datosTiempo.clear();
        for (double i = 0; i < n; i++) {
            datosTiempo.add(i*((double)Variables.Tmuestreo/1000.0));
        }
    }

    private void mostrarDatos() {
        System.out.println(datosTiempo.size());
        System.out.println("T:\tTemp:\tPc:\tPp:");
        for (int i = 0; i < datosTiempo.size(); i++) {
            System.out.print(datosTiempo.get(i)+"\t");
            System.out.print(datosTemperatura.get(i)+"\t");
            System.out.print(datosTemperatura2.get(i)+"\t");
            System.out.print(datosHumedad.get(i)+"\t");
        }
    }

    private XYDataset crearDataset() {
        final XYSeries seriesTemp = new XYSeries("Temperatura");
        final XYSeries seriesTemp2 = new XYSeries("Temperatura 2");
        final XYSeries seriesHumedad = new XYSeries("Humedad");        
          
        if(datosTemperatura.isEmpty()){
            seriesTemp.clear();
            seriesTemp2.clear();
            seriesHumedad.clear();            
        }
        for (int i = 0; i < datosTemperatura.size(); i++) {
            seriesTemp.add(datosTiempo.get(i),datosTemperatura.get(i));
            seriesTemp2.add(datosTiempo.get(i),datosTemperatura2.get(i));
            seriesHumedad.add(datosTiempo.get(i),datosHumedad.get(i));            
        }

        final XYSeriesCollection dataset = new XYSeriesCollection();
        dataset.addSeries(seriesTemp);
        dataset.addSeries(seriesTemp2);
        dataset.addSeries(seriesHumedad);        
        
        return dataset;
    }

    private static JFreeChart crearChart(XYDataset dataset) {
        final JFreeChart chart = ChartFactory.createXYLineChart(
            "Datos Camara Climatica","Tiempo (Segundos)","Temperatura (°C) & Humedad(%)",dataset,PlotOrientation.VERTICAL,
            true,                     // include legend
            true,                     // tooltips
            false                     // urls
        );
        chart.setBackgroundPaint(Color.white);
        final XYPlot plot = chart.getXYPlot();
        plot.setBackgroundPaint(Color.lightGray);
        plot.setDomainGridlinePaint(Color.white);
        plot.setRangeGridlinePaint(Color.white);
        
        final XYLineAndShapeRenderer renderer = new XYLineAndShapeRenderer();
        renderer.setSeriesLinesVisible(0, true);   // línea continua en serie 0
        renderer.setSeriesLinesVisible(1, true);   // línea continua en serie 1
        renderer.setSeriesLinesVisible(2, true);   // línea continua en serie 2
        renderer.setSeriesLinesVisible(3, true);   // línea continua en serie 3
        renderer.setSeriesLinesVisible(4, true);   // línea continua en serie 4
        renderer.setSeriesShapesVisible(0, false);  // No muestra el punto en el valor en serie 0
        renderer.setSeriesShapesVisible(1, false);  // 1. muestra el punto en el valor en serie 1
        renderer.setSeriesShapesVisible(2, false);  // 1. muestra el punto en el valor en serie 2
        renderer.setSeriesShapesVisible(3, false);  // 1. muestra el punto en el valor en serie 3
        renderer.setSeriesShapesVisible(4, false);  // 1. muestra el punto en el valor en serie 4
        plot.setRenderer(renderer);
        final NumberAxis rangeAxis = (NumberAxis) plot.getRangeAxis();
        rangeAxis.setStandardTickUnits(NumberAxis.createIntegerTickUnits());
        return chart;
    }

    public void actualizarGrafica() {
        final XYDataset dataset = crearDataset();
        chart = crearChart(dataset);
        XYPlot plot = (XYPlot) chart.getPlot();
        plot.getRangeAxis().setRange(0, 20);
        plot.getRangeAxis().setAutoRange(true);
        chartPanel = new ChartPanel(chart);
        
        this.chartPanel = new ChartPanel(chart);
        this.chartPanel.addChartMouseListener(this);
        CrosshairOverlay crosshairOverlay = new CrosshairOverlay();
        this.xCrosshair = new Crosshair(Double.NaN, Color.GRAY, new BasicStroke(0f));
        this.xCrosshair.setLabelFont(xCrosshair.getLabelFont().deriveFont(20f));
        this.xCrosshair.setLabelBackgroundPaint(new Color(1f, 1f, 1f, 0f));
        this.xCrosshair.setLabelOutlineVisible(false);
        this.xCrosshair.setLabelVisible(true);
        
        this.yCrosshair = new Crosshair(Double.NaN, Color.GRAY, new BasicStroke(0f));
        this.yCrosshair.setLabelFont(xCrosshair.getLabelFont().deriveFont(20f));
        this.yCrosshair.setLabelBackgroundPaint(new Color(1f, 1f, 1f, 0f));
        this.yCrosshair.setLabelOutlineVisible(false);
        this.yCrosshair.setLabelVisible(true);
        crosshairOverlay.addDomainCrosshair(xCrosshair);
        crosshairOverlay.addRangeCrosshair(yCrosshair);
        chartPanel.addOverlay(crosshairOverlay);
        
        int ancho = FramePrincipal.panelGrafica.getWidth();
        int alto = FramePrincipal.panelGrafica.getHeight();
        //chartPanel.setPreferredSize(new java.awt.Dimension(ancho, alto));
        chartPanel.setPreferredSize(new java.awt.Dimension(ancho-40, alto-120));
        chartPanel.setMaximumSize(new java.awt.Dimension(ancho-40, alto-120));
        chartPanel.setMinimumSize(new java.awt.Dimension(ancho-40, alto-120));
        
        Grafica.setPreferredSize(new java.awt.Dimension(ancho, alto));
        Grafica.setMaximumSize(new java.awt.Dimension(ancho, alto));
        Grafica.setMinimumSize(new java.awt.Dimension(ancho, alto));
        FramePrincipal.Grafica.setContentPane(chartPanel);
        //setContentPane(chartPanel);
    }

    @Override
    public void chartMouseClicked(ChartMouseEvent event) {
        Rectangle2D dataArea = this.chartPanel.getScreenDataArea();
        JFreeChart chart = event.getChart();
        XYPlot plot = (XYPlot) chart.getPlot();
        ValueAxis xAxis = plot.getDomainAxis();
        double x = xAxis.java2DToValue(event.getTrigger().getX(), dataArea, 
                RectangleEdge.BOTTOM);
        double y = DatasetUtilities.findYValue(plot.getDataset(), 0, x);
        this.xCrosshair.setValue(x);
        this.yCrosshair.setValue(y);
    }

    @Override
    public void chartMouseMoved(ChartMouseEvent event) {
        
    }
}
