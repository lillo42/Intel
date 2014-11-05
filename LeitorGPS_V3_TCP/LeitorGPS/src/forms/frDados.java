/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package forms;

import DAO.ControleAcesso;
import DAO.GPDAO;
import excecoes.LeituraArquivoException;
import gpSentences.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.ButtonGroup;
import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.JFileChooser;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JSlider;
import javax.swing.SpringLayout;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.LineBorder;
import jssc.SerialPortException;
import leitorGPS.ComunicadorSerial;
import leitorGPS.ISatInfoListener;
import leitorGPS.InfoSatelite;
import leitorGPS.MainClass;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryLabelPositions;
import org.jfree.chart.labels.ItemLabelAnchor;
import org.jfree.chart.labels.ItemLabelPosition;
import org.jfree.chart.labels.StandardCategoryItemLabelGenerator;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.BarRenderer;
import org.jfree.chart.renderer.category.LineAndShapeRenderer;
import org.jfree.data.Range;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.ui.RectangleInsets;
import org.jfree.ui.TextAnchor;
import org.openstreetmap.gui.jmapviewer.Coordinate;
import org.openstreetmap.gui.jmapviewer.JMapViewer;
import org.openstreetmap.gui.jmapviewer.JMapViewerDrone;
import org.openstreetmap.gui.jmapviewer.MemoryTileCache;
import org.openstreetmap.gui.jmapviewer.tilesources.OfflineOsmTileSource;
import util.SateliteModel;
import util.Util;
import util.Util.EnumEstadoForm;
import util.Util.EnumTipoConexao;
import util.Util.EnumTipoMensagem;

/**
 * Form principal da aplicação, onde as informações NMEA
 * são visualizadas.
 * Esse form também permite realizar a manutenção dos usuários do sistema.
 * 
 * @author Vitor
 */
public class frDados extends javax.swing.JFrame implements ActionListener, ISatInfoListener{
    
    EnumTipoConexao modoConexao;
    EnumEstadoForm estadoForm = EnumEstadoForm.Ocioso;

    DefaultCategoryDataset dadosGraficoSats = new DefaultCategoryDataset();
    DefaultCategoryDataset dadosGraficoVelox = new DefaultCategoryDataset();
    JFreeChart veloxPlot;
    JFreeChart plot;
    JPanel pnlSkyplot;
    JFXPanelMapa3D pnl3DMapFX;
    JMapViewerDrone mapOsm;
            
    /**
     * Instancia o form.
     */
    public frDados() 
    {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            //SwingUtilities.updateComponentTreeUI(this);
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException ex) {
            Logger.getLogger(frDados.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        initComponents();
        this.setLocationRelativeTo(null);
        
        inicializarTemas();
        initSatGrafico();
        initSatVelox();
        initMapPanel();
        init3DMapPanel();
        carregarListaUsers();
        initOsmPanel();
        
        cmbPortas.setModel(new DefaultComboBoxModel(ComunicadorSerial.getPortasDisponiveis()));
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                setVisible(true);
            }
        });
    }
    
    /**
     * Instancia o painel do Open Street Maps e aplica
     * configuracoes iniciais, como raio do perimetro e 
     * posicao inicial
     */
    private void initOsmPanel()
    {
        mapOsm = new JMapViewerDrone();
        mapOsm.setDisplayPosition(new Coordinate(-23.736142,-46.584126), 12);
        //mapOsm.setTileSource(new OfflineOsmTileSource("file:///D:/Arqs/Tiles2",1,19));
        
        mapOsm.getMapController().setRaio(sldRaioMarc.getValue());
        lblRaio.setText(new Integer(sldRaioMarc.getValue()).toString());
        pnlOsm.setLayout(new BorderLayout());
        pnlOsm.add(mapOsm,BorderLayout.CENTER);
    }
    
    /**
     * Lê todos os usuários cadastrados no banco e
     * coloca no listbox de usuários.
     */
    private void carregarListaUsers()
    {
        try
        {
            ArrayList<String> users = ControleAcesso.getUsers();
            DefaultListModel dfm = new DefaultListModel();
            
            for(String user : users)
                dfm.addElement(user);
            
            lstUsers.setModel(dfm);
        }
        catch(Exception erro)
        {
            Util.ExibirMensagem(Util.EnumTipoMensagem.Erro, erro.getMessage());
        }
        
    }
    
    /**
     * Inicializa o painel 3D.
     */
    private void init3DMapPanel()
    {   
        pnl3DMapFX = new JFXPanelMapa3D();
        pnlMapa3D.setLayout(new BorderLayout());
        pnlMapa3D.add(pnl3DMapFX);
        pnl3DMapFX.init();
    }
    
    /**
     * Inicializa o painel de mapa 2D.
     */
    private void initMapPanel()
    {
        pnlSkyplot = new SkyPlotPanel();
        pnlSkyplot.setBackground(Color.white);
        pnlSkyplot.setPreferredSize(new Dimension(400,400));
        
        pnlMapConteudo.setLayout(new BorderLayout());
        pnlMapConteudo.add(pnlSkyplot, BorderLayout.CENTER);
    }
    
    /**
     * Inicializa o gráfico de velocidade.
     */
    private void initSatVelox()
    {
        dadosGraficoVelox.clear();
        
        veloxPlot = ChartFactory.createLineChart("Velocidade", 
                "Hora", "Velocidade KM/h", dadosGraficoVelox,
                PlotOrientation.VERTICAL,
                false,
                true, false);
        
        veloxPlot.getTitle().setFont(Font.decode("arial-16"));
        veloxPlot.getTitle().setPadding(5,20,5,20);
        veloxPlot.setPadding(new RectangleInsets(10,10,0,10));
        
        ChartPanel cp = new ChartPanel(veloxPlot);
        cp.setBorder(LineBorder.createGrayLineBorder());
        
        LineAndShapeRenderer br = (LineAndShapeRenderer)veloxPlot.getCategoryPlot().getRenderer();
        
        br.setBaseItemLabelsVisible(true);
        br.setSeriesItemLabelFont(0, Font.decode("arial-12"));
        br.setBaseItemLabelGenerator(new StandardCategoryItemLabelGenerator());
        br.setBasePositiveItemLabelPosition(new ItemLabelPosition(
        ItemLabelAnchor.OUTSIDE12, TextAnchor.BASELINE_CENTER));
        
        veloxPlot.getCategoryPlot().getDomainAxis().setCategoryLabelPositions(CategoryLabelPositions.UP_45);
        //veloxPlot.getCategoryPlot().getRangeAxis().setRange(new Range(0,100), true,true);
        
        pnlChartSpeed.setLayout(new BorderLayout());
        pnlChartSpeed.add(cp,BorderLayout.CENTER);
    }
    
    /**
     * Inicializa o gráfico de intensidade de sinal dos satélites.
     */
    private void initSatGrafico()
    {
        dadosGraficoSats.clear();
        plot = ChartFactory.createBarChart("Intensidade do sinal", 
                "PRN", 
                "SNR", 
                dadosGraficoSats,
                PlotOrientation.VERTICAL,
                false,
                true,
                true);
        
        plot.getTitle().setFont(Font.decode("arial-16"));
        plot.getTitle().setPadding(5,20,5,20);
        plot.setPadding(new RectangleInsets(10,10,0,10));
        //plot.setBackgroundPaint(new Color(255,255,255,0));
        
        BarRenderer br = (BarRenderer)plot.getCategoryPlot().getRenderer();
        br.setSeriesPaint(0, Color.BLUE);
        br.setMaximumBarWidth(0.05);
        
        plot.getCategoryPlot().getRangeAxis().setRange(new Range(0,50), true,true);
        br.setBaseItemLabelsVisible(true);
        br.setSeriesItemLabelFont(0, Font.decode("arial-12"));
        br.setBaseItemLabelGenerator(new StandardCategoryItemLabelGenerator());
        br.setBasePositiveItemLabelPosition(new ItemLabelPosition(
                ItemLabelAnchor.OUTSIDE12, TextAnchor.BASELINE_CENTER));
        
        SpringLayout lm = new SpringLayout();
        ChartPanel cp = new ChartPanel(plot);
        cp.setBorder(LineBorder.createGrayLineBorder());
        
        lm.putConstraint(SpringLayout.EAST, pnlSatPlot, 10 , SpringLayout.EAST, cp);
        lm.putConstraint(SpringLayout.WEST, cp, 10 , SpringLayout.WEST, pnlSatPlot);
        lm.putConstraint(SpringLayout.SOUTH, pnlSatPlot, 10 , SpringLayout.SOUTH, cp);
        lm.putConstraint(SpringLayout.NORTH, cp, 10 , SpringLayout.NORTH, pnlSatPlot);
        
        pnlSatPlot.setLayout(lm);
        pnlSatPlot.add(cp);
    }
    
    /**
     * Carrega os temas disponíveis e adiciona ao menu de temas.
     */
    private void inicializarTemas()
    {
        String winLnF = UIManager.getSystemLookAndFeelClassName();
        
        ButtonGroup group = new ButtonGroup();
        for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
            
            JRadioButtonMenuItem mnuItem = new JRadioButtonMenuItem(info.getName());
            mnuItem.addActionListener(this);
            
            if(winLnF.equals(info.getClassName()))
                mnuItem.setSelected(true);
            
            group.add(mnuItem);
            mnuTemas.add(mnuItem);
        }
    }

    /**
     * Retorna o caminho do arquivo NMEA a ser lido.
     * @return O caminho do arquivo NMEA a ser lido.
     */
    public String getCaminho() {
        return txtCaminho.getText();
    }
    
    /**
     * Retorna a porta serial a ser lida.
     * @return A porta serial a ser lida.
     */
    public String getPortaSerial()
    {
        try{
            return cmbPortas.getSelectedItem().toString();
        }
        catch(Exception ex){
            return "";
        }
        
    }
    
    /**
     * Retorna o endereço do servidor com o qual se deseja conectar.
     * @return O endereço IP do servidor.
     */
    public String getEnderecoTCP()
    {
        return txtIp.getText();
    }
    
    /**
     * Retorna a porta TCP do servidor com o qual se deseja conectar.
     * @return A porta TCP do servidor.
     */
    public int getPortaTCP()
    {
        try{
            return Integer.parseInt(txtPorta.getText());
        }
        catch(Exception ex){
            Util.ExibirMensagem(EnumTipoMensagem.Erro, "Digite um valor numérico para a porta TCP.");
            txtPorta.setText("9999");
            txtPorta.requestFocus();
        }
        
        return 9999;
    }
    
    /**
     * Usado para escrever o total de registros num arquivo NMEA.
     * @param obj O total de registros.
     */
    public void setTotalRegistros(String obj)
    {
        lblTotal.setText(obj);
        barArquivos.setMaximum(Integer.parseInt(obj));
    }
    
    /**
     * Usado para escrever o total de registros lidos de um arquivo NMEA.
     * @param obj O total de registros lidos.
     */
    public void setRegistrosLidos(String obj)
    {
        lblLido.setText(obj);
        barArquivos.setValue(Integer.parseInt(obj));
    }
    
    /**
     * Devolve o texto na TextArea com todas as sentenças NMEA lidas.
     * @return Os dados no TextArea de dados brutos.
     */
    public String getRawData()
    {
        return txtRawData.getText();
    }
    
    /**
     * Adiciona uma string NMEA à TextArea de sentenças.
     * @param obj Os dados a serem adicionados.
     */
    public void atualizarRaw(String obj)
    {
        txtRawData.append(obj);
        
        if(chkAutoScrollRaw.isSelected())
            txtRawData.setCaretPosition(txtRawData.getDocument().getLength());
        
        int qtdLido = Integer.parseInt(lblLido.getText());
        qtdLido++;
        setRegistrosLidos(Integer.toString(qtdLido));
    }
    
    /**
     * Recebe uma sentença, checa seu tipo e atualiza a porção do form correspondente.
     * @param obj A sentença NMEA mapeada para um objeto.
     */
    public void atualizarForm(GPSentence obj)
    {
        if(obj instanceof GPRMC)
        {
            GPRMC objC = (GPRMC)obj;
            
            txtTime.setText(objC.getUtcTime().toString());
            txtLatitude.setText(objC.getLatitude() + " " + Character.toString(objC.getLatiChar()));
            txtLongitude.setText(objC.getLongitude()+ " " + Character.toString(objC.getLongChar()));
            txtVelox.setText(objC.getSpeed());
            txtDir.setText(objC.getDirecao());
            lblEstadoPosit.setText(objC.getDescricaoPosicao());
            
            jCalendarData.getDayChooser().setDay(Integer.parseInt(objC.getData().getDia()));
            jCalendarData.getMonthChooser().setMonth(Integer.parseInt(objC.getData().getMes()) - 1);
            jCalendarData.getYearChooser().setYear(Integer.parseInt(objC.getData().getAno()));
            
            pnl3DMapFX.setLatitude(Util.getLatitudeDegrees(objC.getLatitude(),objC.getLatiChar()));
            pnl3DMapFX.setLongitude(Util.getLongitudeDegrees(objC.getLongitude(),objC.getLongChar()));
            
            updateVeloxChart(objC);
        }
        else if(obj instanceof GPGSV)
        {
            //A sentença GSV e tratada pelo método updateSatInfo
            //da interface ISatInfoListener
            
            //GPGSV objEsp = (GPGSV)obj;
            //((SateliteModel)tabSats.getModel()).setDados(sats);
        }
    }
    
    /**
     * Atualiza o gráfico de histórico de velocidade.
     * @param objC O objeto com as informações de velocidade.
     */
    private void updateVeloxChart(GPRMC objC)
    {
        if(dadosGraficoVelox.getColumnCount()> 4)
                dadosGraficoVelox.removeColumn(0);
            
            dadosGraficoVelox.setValue(
                    Math.round(Util.knotsToKMh(Double.parseDouble(objC.getSpeed()))), 
                    "Hora", 
                    objC.getUtcTime().toString());
    }
    
    /**
     * Atualiza a tabela de satélites, o número de satélites vistos e usados
     * e os mapas de satélites.
     */
    @Override
    public void updateSatInfo() 
    {
        lblTotalSats.setText(Integer.toString(InfoSatelite.getQtdTotalSats()));
        lblSatsUsados.setText(Integer.toString(InfoSatelite.getQtdSatsFix()));
        ((SateliteModel)tabSats.getModel()).setDados(InfoSatelite.getSats());
        atualizarGraficoSats();
        pnlSkyplot.repaint();
        
        if(!chkPausar.isSelected())
            pnl3DMapFX.updateSats(chkLinhas.isSelected());
    }
    
    /**
     * Método responsável por atualizar o gráfico de intensidade de sinal.
     * Usa as informações da classe InfoSatelite.
     */
    private void atualizarGraficoSats()
    {
        
       dadosGraficoSats.clear();
        
        for(InfoSatelite sat : InfoSatelite.getSats())
        {
            try
            {
                dadosGraficoSats.setValue(Integer.parseInt(sat.getSnr()),
                                        "PRN", new Integer(Integer.parseInt(sat.getPrn())));
            }
            catch(Exception erro)
            {
                dadosGraficoSats.setValue(0,"PRN", new Integer(Integer.parseInt(sat.getPrn())));
            }
            
        }
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        grpModoConexao = new javax.swing.ButtonGroup();
        jfcArquivo = new javax.swing.JFileChooser();
        frCadastro = new javax.swing.JDialog();
        jScrollPane4 = new javax.swing.JScrollPane();
        lstUsers = new javax.swing.JList();
        btnCancelar = new javax.swing.JButton();
        jPanel10 = new javax.swing.JPanel();
        jLabel12 = new javax.swing.JLabel();
        txtLogin = new javax.swing.JTextField();
        jLabel13 = new javax.swing.JLabel();
        txtSenha = new javax.swing.JPasswordField();
        jPanel11 = new javax.swing.JPanel();
        btnAplicar = new javax.swing.JButton();
        btnRemover = new javax.swing.JButton();
        jSplitPane3 = new javax.swing.JSplitPane();
        jSplitPane4 = new javax.swing.JSplitPane();
        pnlSatPlot = new javax.swing.JPanel();
        jTabbedPane2 = new javax.swing.JTabbedPane();
        pnlMapTab = new javax.swing.JPanel();
        pnlMapConteudo = new javax.swing.JPanel();
        jPanel7 = new javax.swing.JPanel();
        pnlMapa3D = new javax.swing.JPanel();
        jPanel6 = new javax.swing.JPanel();
        chkLinhas = new javax.swing.JCheckBox();
        chkPausar = new javax.swing.JCheckBox();
        jPanel12 = new javax.swing.JPanel();
        pnlOsm = new javax.swing.JPanel();
        jPanel13 = new javax.swing.JPanel();
        sldRaioMarc = new javax.swing.JSlider();
        jLabel14 = new javax.swing.JLabel();
        btnAplicarMarcTmp = new javax.swing.JButton();
        btnCancelarMarcTmp = new javax.swing.JButton();
        btnRemoverTudo = new javax.swing.JButton();
        jLabel15 = new javax.swing.JLabel();
        lblRaio = new javax.swing.JLabel();
        jLabel16 = new javax.swing.JLabel();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jPanel1 = new javax.swing.JPanel();
        jCalendarData = new com.toedter.calendar.JCalendar();
        txtTime = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        txtVelox = new javax.swing.JTextField();
        jLabel2 = new javax.swing.JLabel();
        txtDir = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        lblEstadoPosit = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        pnlChartSpeed = new javax.swing.JPanel();
        txtLatitude = new javax.swing.JTextField();
        jLabel6 = new javax.swing.JLabel();
        txtLongitude = new javax.swing.JTextField();
        jPanel2 = new javax.swing.JPanel();
        jScrollPane3 = new javax.swing.JScrollPane();
        tabSats = new javax.swing.JTable();
        jLabel9 = new javax.swing.JLabel();
        lblTotalSats = new javax.swing.JLabel();
        lblTotalSats1 = new javax.swing.JLabel();
        lblSatsUsados = new javax.swing.JLabel();
        jPanel4 = new javax.swing.JPanel();
        chkAutoScrollRaw = new javax.swing.JCheckBox();
        jScrollPane2 = new javax.swing.JScrollPane();
        jScrollPane1 = new javax.swing.JScrollPane();
        txtRawData = new javax.swing.JTextArea();
        barArquivos = new javax.swing.JProgressBar();
        jLabel7 = new javax.swing.JLabel();
        lblLido = new javax.swing.JLabel();
        jLabel8 = new javax.swing.JLabel();
        lblTotal = new javax.swing.JLabel();
        btnSalvar = new javax.swing.JButton();
        jPanel3 = new javax.swing.JPanel();
        jPanel5 = new javax.swing.JPanel();
        jLabel10 = new javax.swing.JLabel();
        cmbPortas = new javax.swing.JComboBox();
        jPanel8 = new javax.swing.JPanel();
        jLabel11 = new javax.swing.JLabel();
        txtCaminho = new javax.swing.JTextField();
        btnSelecionarArquivo = new javax.swing.JButton();
        jPanel9 = new javax.swing.JPanel();
        btnConectar = new javax.swing.JButton();
        radSerial = new javax.swing.JRadioButton();
        radArquivo = new javax.swing.JRadioButton();
        radTcp = new javax.swing.JRadioButton();
        jPanel14 = new javax.swing.JPanel();
        jLabel17 = new javax.swing.JLabel();
        txtIp = new javax.swing.JTextField();
        jLabel18 = new javax.swing.JLabel();
        txtPorta = new javax.swing.JTextField();
        jMenuBar1 = new javax.swing.JMenuBar();
        mnuAcessos = new javax.swing.JMenu();
        mnuCadastroUser = new javax.swing.JMenuItem();
        mnuTemas = new javax.swing.JMenu();
        mnuAjuda = new javax.swing.JMenu();
        mnuSobre = new javax.swing.JMenuItem();

        frCadastro.setTitle("Manutenção de usuários");
        frCadastro.setMinimumSize(new java.awt.Dimension(403, 367));
        frCadastro.setModalityType(java.awt.Dialog.ModalityType.APPLICATION_MODAL);
        frCadastro.setResizable(false);

        lstUsers.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        lstUsers.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                lstUsersMouseClicked(evt);
            }
        });
        jScrollPane4.setViewportView(lstUsers);

        btnCancelar.setText("Cancelar");
        btnCancelar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCancelarActionPerformed(evt);
            }
        });

        jPanel10.setBorder(javax.swing.BorderFactory.createTitledBorder("Info"));

        jLabel12.setText("Login");

        jLabel13.setText("Senha");

        javax.swing.GroupLayout jPanel10Layout = new javax.swing.GroupLayout(jPanel10);
        jPanel10.setLayout(jPanel10Layout);
        jPanel10Layout.setHorizontalGroup(
            jPanel10Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel10Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel10Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(txtLogin)
                    .addGroup(jPanel10Layout.createSequentialGroup()
                        .addGroup(jPanel10Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel12)
                            .addComponent(jLabel13))
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(txtSenha))
                .addContainerGap())
        );
        jPanel10Layout.setVerticalGroup(
            jPanel10Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel10Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel12)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(txtLogin, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jLabel13)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(txtSenha, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel11.setBorder(javax.swing.BorderFactory.createTitledBorder("Ações"));

        btnAplicar.setText("Aplicar");
        btnAplicar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnAplicarActionPerformed(evt);
            }
        });

        btnRemover.setText("Remover");
        btnRemover.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnRemoverActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel11Layout = new javax.swing.GroupLayout(jPanel11);
        jPanel11.setLayout(jPanel11Layout);
        jPanel11Layout.setHorizontalGroup(
            jPanel11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel11Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(btnAplicar, javax.swing.GroupLayout.DEFAULT_SIZE, 94, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(btnRemover, javax.swing.GroupLayout.PREFERRED_SIZE, 87, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        jPanel11Layout.setVerticalGroup(
            jPanel11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel11Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnAplicar)
                    .addComponent(btnRemover))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout frCadastroLayout = new javax.swing.GroupLayout(frCadastro.getContentPane());
        frCadastro.getContentPane().setLayout(frCadastroLayout);
        frCadastroLayout.setHorizontalGroup(
            frCadastroLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(frCadastroLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane4, javax.swing.GroupLayout.PREFERRED_SIZE, 163, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(frCadastroLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel10, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel11, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, frCadastroLayout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(btnCancelar)))
                .addContainerGap())
        );
        frCadastroLayout.setVerticalGroup(
            frCadastroLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(frCadastroLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(frCadastroLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(frCadastroLayout.createSequentialGroup()
                        .addComponent(jPanel10, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel11, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 78, Short.MAX_VALUE)
                        .addComponent(btnCancelar))
                    .addComponent(jScrollPane4))
                .addContainerGap())
        );

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Visor NMEA - Vitor Fiorillo");

        jSplitPane3.setDividerLocation(400);
        jSplitPane3.setOneTouchExpandable(true);

        jSplitPane4.setDividerLocation(350);
        jSplitPane4.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        jSplitPane4.setOneTouchExpandable(true);

        javax.swing.GroupLayout pnlSatPlotLayout = new javax.swing.GroupLayout(pnlSatPlot);
        pnlSatPlot.setLayout(pnlSatPlotLayout);
        pnlSatPlotLayout.setHorizontalGroup(
            pnlSatPlotLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 510, Short.MAX_VALUE)
        );
        pnlSatPlotLayout.setVerticalGroup(
            pnlSatPlotLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        jSplitPane4.setBottomComponent(pnlSatPlot);

        pnlMapConteudo.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        javax.swing.GroupLayout pnlMapConteudoLayout = new javax.swing.GroupLayout(pnlMapConteudo);
        pnlMapConteudo.setLayout(pnlMapConteudoLayout);
        pnlMapConteudoLayout.setHorizontalGroup(
            pnlMapConteudoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 477, Short.MAX_VALUE)
        );
        pnlMapConteudoLayout.setVerticalGroup(
            pnlMapConteudoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 295, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout pnlMapTabLayout = new javax.swing.GroupLayout(pnlMapTab);
        pnlMapTab.setLayout(pnlMapTabLayout);
        pnlMapTabLayout.setHorizontalGroup(
            pnlMapTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlMapTabLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(pnlMapConteudo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        pnlMapTabLayout.setVerticalGroup(
            pnlMapTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlMapTabLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(pnlMapConteudo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );

        jTabbedPane2.addTab("Mapa 2D", pnlMapTab);

        pnlMapa3D.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        jPanel6.setBorder(javax.swing.BorderFactory.createTitledBorder("Opções"));

        chkLinhas.setSelected(true);
        chkLinhas.setText("Exibir linhas");

        chkPausar.setText("Pausar");

        javax.swing.GroupLayout jPanel6Layout = new javax.swing.GroupLayout(jPanel6);
        jPanel6.setLayout(jPanel6Layout);
        jPanel6Layout.setHorizontalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(chkLinhas)
                    .addComponent(chkPausar))
                .addContainerGap(52, Short.MAX_VALUE))
        );
        jPanel6Layout.setVerticalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(chkLinhas)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(chkPausar)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout pnlMapa3DLayout = new javax.swing.GroupLayout(pnlMapa3D);
        pnlMapa3D.setLayout(pnlMapa3DLayout);
        pnlMapa3DLayout.setHorizontalGroup(
            pnlMapa3DLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlMapa3DLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(296, Short.MAX_VALUE))
        );
        pnlMapa3DLayout.setVerticalGroup(
            pnlMapa3DLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlMapa3DLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(201, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout jPanel7Layout = new javax.swing.GroupLayout(jPanel7);
        jPanel7.setLayout(jPanel7Layout);
        jPanel7Layout.setHorizontalGroup(
            jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel7Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(pnlMapa3D, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        jPanel7Layout.setVerticalGroup(
            jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel7Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(pnlMapa3D, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );

        jTabbedPane2.addTab("Mapa 3D", jPanel7);

        pnlOsm.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        javax.swing.GroupLayout pnlOsmLayout = new javax.swing.GroupLayout(pnlOsm);
        pnlOsm.setLayout(pnlOsmLayout);
        pnlOsmLayout.setHorizontalGroup(
            pnlOsmLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 267, Short.MAX_VALUE)
        );
        pnlOsmLayout.setVerticalGroup(
            pnlOsmLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        jPanel13.setBorder(javax.swing.BorderFactory.createTitledBorder("Opções"));

        sldRaioMarc.setMajorTickSpacing(60);
        sldRaioMarc.setMaximum(200);
        sldRaioMarc.setMinimum(20);
        sldRaioMarc.setMinorTickSpacing(10);
        sldRaioMarc.setPaintLabels(true);
        sldRaioMarc.setPaintTicks(true);
        sldRaioMarc.setSnapToTicks(true);
        sldRaioMarc.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldRaioMarcStateChanged(evt);
            }
        });

        jLabel14.setText("Raio (m)");

        btnAplicarMarcTmp.setText("Aplicar");
        btnAplicarMarcTmp.setToolTipText("Torna perímetro temporário efetivo");
        btnAplicarMarcTmp.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnAplicarMarcTmpActionPerformed(evt);
            }
        });

        btnCancelarMarcTmp.setText("Cancelar");
        btnCancelarMarcTmp.setToolTipText("Remove perímetro temporário");
        btnCancelarMarcTmp.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCancelarMarcTmpActionPerformed(evt);
            }
        });

        btnRemoverTudo.setText("Remover marcadores");
        btnRemoverTudo.setToolTipText("Remove todos marcadores");
        btnRemoverTudo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnRemoverTudoActionPerformed(evt);
            }
        });

        jLabel15.setText("Raio:");

        lblRaio.setText("00");

        jLabel16.setText("m");

        javax.swing.GroupLayout jPanel13Layout = new javax.swing.GroupLayout(jPanel13);
        jPanel13.setLayout(jPanel13Layout);
        jPanel13Layout.setHorizontalGroup(
            jPanel13Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel13Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel13Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(sldRaioMarc, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
                    .addGroup(jPanel13Layout.createSequentialGroup()
                        .addComponent(btnAplicarMarcTmp, javax.swing.GroupLayout.DEFAULT_SIZE, 78, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(btnCancelarMarcTmp, javax.swing.GroupLayout.PREFERRED_SIZE, 82, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(btnRemoverTudo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel13Layout.createSequentialGroup()
                        .addGroup(jPanel13Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel14)
                            .addGroup(jPanel13Layout.createSequentialGroup()
                                .addComponent(jLabel15)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(lblRaio)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel16)))
                        .addGap(0, 0, Short.MAX_VALUE)))
                .addContainerGap())
        );
        jPanel13Layout.setVerticalGroup(
            jPanel13Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel13Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel14)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(sldRaioMarc, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel13Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel15)
                    .addComponent(lblRaio)
                    .addComponent(jLabel16))
                .addGap(38, 38, 38)
                .addGroup(jPanel13Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(btnCancelarMarcTmp)
                    .addComponent(btnAplicarMarcTmp, javax.swing.GroupLayout.Alignment.TRAILING))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(btnRemoverTudo)
                .addContainerGap(62, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout jPanel12Layout = new javax.swing.GroupLayout(jPanel12);
        jPanel12.setLayout(jPanel12Layout);
        jPanel12Layout.setHorizontalGroup(
            jPanel12Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel12Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(pnlOsm, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel13, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        jPanel12Layout.setVerticalGroup(
            jPanel12Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel12Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel12Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jPanel13, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(pnlOsm, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );

        jTabbedPane2.addTab("Open Street Maps", jPanel12);

        jSplitPane4.setLeftComponent(jTabbedPane2);
        jTabbedPane2.getAccessibleContext().setAccessibleName("Mapa2D");

        jSplitPane3.setRightComponent(jSplitPane4);

        jCalendarData.setEnabled(false);

        txtTime.setEditable(false);
        txtTime.setPreferredSize(new java.awt.Dimension(40, 20));

        jLabel1.setText("Hora UTC");

        txtVelox.setEditable(false);
        txtVelox.setPreferredSize(new java.awt.Dimension(40, 20));

        jLabel2.setText("Velocidade (Nós)");

        txtDir.setEditable(false);
        txtDir.setPreferredSize(new java.awt.Dimension(40, 20));

        jLabel3.setText("Direção");

        jLabel4.setText("Estado da posição:");

        lblEstadoPosit.setText("-");

        jLabel5.setText("Latitude");

        javax.swing.GroupLayout pnlChartSpeedLayout = new javax.swing.GroupLayout(pnlChartSpeed);
        pnlChartSpeed.setLayout(pnlChartSpeedLayout);
        pnlChartSpeedLayout.setHorizontalGroup(
            pnlChartSpeedLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        pnlChartSpeedLayout.setVerticalGroup(
            pnlChartSpeedLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 280, Short.MAX_VALUE)
        );

        txtLatitude.setEditable(false);

        jLabel6.setText("Longitude");

        txtLongitude.setEditable(false);

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(txtLatitude)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                .addComponent(txtTime, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 138, Short.MAX_VALUE)
                                .addComponent(txtVelox, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabel1, javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(jLabel2, javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(jLabel3, javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(txtDir, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                            .addComponent(jLabel5))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jCalendarData, javax.swing.GroupLayout.DEFAULT_SIZE, 230, Short.MAX_VALUE))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addComponent(jLabel4)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(lblEstadoPosit))
                            .addComponent(jLabel6))
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(txtLongitude)
                    .addComponent(pnlChartSpeed, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtTime, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(21, 21, 21)
                        .addComponent(jLabel2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtVelox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(jLabel3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtDir, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(jLabel5, javax.swing.GroupLayout.PREFERRED_SIZE, 14, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jCalendarData, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(txtLatitude, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jLabel6)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(txtLongitude, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(pnlChartSpeed, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel4)
                    .addComponent(lblEstadoPosit))
                .addContainerGap())
        );

        jTabbedPane1.addTab("RMC", jPanel1);

        tabSats.setAutoCreateRowSorter(true);
        tabSats.setModel(new util.SateliteModel());
        tabSats.setRowHeight(30);
        jScrollPane3.setViewportView(tabSats);

        jLabel9.setText("Total de Satélites / Satélites usados: ");

        lblTotalSats.setText("0");

        lblTotalSats1.setText("/");

        lblSatsUsados.setText("0");

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addComponent(jLabel9)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblTotalSats)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblTotalSats1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblSatsUsados)
                        .addGap(0, 123, Short.MAX_VALUE)))
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane3, javax.swing.GroupLayout.DEFAULT_SIZE, 590, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel9)
                    .addComponent(lblTotalSats)
                    .addComponent(lblTotalSats1)
                    .addComponent(lblSatsUsados))
                .addContainerGap())
        );

        jTabbedPane1.addTab("Satélites", jPanel2);

        chkAutoScrollRaw.setSelected(true);
        chkAutoScrollRaw.setText("Auto Scroll");

        txtRawData.setEditable(false);
        txtRawData.setColumns(20);
        txtRawData.setRows(5);
        jScrollPane1.setViewportView(txtRawData);

        jScrollPane2.setViewportView(jScrollPane1);

        jLabel7.setText("Registros lidos: ");

        lblLido.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
        lblLido.setText("0");

        jLabel8.setText("/");

        lblTotal.setText("0");

        btnSalvar.setText("Salvar");
        btnSalvar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSalvarActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 374, Short.MAX_VALUE)
                    .addComponent(barArquivos, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(jLabel7)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblLido)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel8)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblTotal)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(chkAutoScrollRaw)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(btnSalvar)))
                .addContainerGap())
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(chkAutoScrollRaw)
                    .addComponent(btnSalvar))
                .addGap(18, 18, 18)
                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 501, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel7)
                    .addComponent(lblLido)
                    .addComponent(jLabel8)
                    .addComponent(lblTotal))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(barArquivos, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        jTabbedPane1.addTab("Dados Brutos", jPanel4);

        jPanel5.setBorder(javax.swing.BorderFactory.createTitledBorder("Serial"));

        jLabel10.setText("Porta");

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(jLabel10)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(cmbPortas, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel10)
                .addGap(18, 18, 18)
                .addComponent(cmbPortas, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(25, Short.MAX_VALUE))
        );

        jPanel8.setBorder(javax.swing.BorderFactory.createTitledBorder("Arquivo"));

        jLabel11.setText("Caminho");

        txtCaminho.setText("gpsdata.txt");

        btnSelecionarArquivo.setText("Selecionar");
        btnSelecionarArquivo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSelecionarArquivoActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel8Layout = new javax.swing.GroupLayout(jPanel8);
        jPanel8.setLayout(jPanel8Layout);
        jPanel8Layout.setHorizontalGroup(
            jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel8Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel8Layout.createSequentialGroup()
                        .addComponent(jLabel11)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(txtCaminho)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel8Layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(btnSelecionarArquivo)))
                .addContainerGap())
        );
        jPanel8Layout.setVerticalGroup(
            jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel8Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel11)
                .addGap(18, 18, 18)
                .addComponent(txtCaminho, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(btnSelecionarArquivo)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel9.setBorder(javax.swing.BorderFactory.createTitledBorder("Conexão"));

        btnConectar.setText("Conectar");
        btnConectar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnConectarActionPerformed(evt);
            }
        });

        grpModoConexao.add(radSerial);
        radSerial.setText("Serial");

        grpModoConexao.add(radArquivo);
        radArquivo.setSelected(true);
        radArquivo.setText("Arquivo");

        grpModoConexao.add(radTcp);
        radTcp.setText("TCP");

        javax.swing.GroupLayout jPanel9Layout = new javax.swing.GroupLayout(jPanel9);
        jPanel9.setLayout(jPanel9Layout);
        jPanel9Layout.setHorizontalGroup(
            jPanel9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel9Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(radSerial)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(radArquivo)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(radTcp)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 72, Short.MAX_VALUE)
                .addComponent(btnConectar)
                .addContainerGap())
        );
        jPanel9Layout.setVerticalGroup(
            jPanel9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel9Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnConectar)
                    .addComponent(radSerial)
                    .addComponent(radArquivo)
                    .addComponent(radTcp))
                .addContainerGap())
        );

        jPanel14.setBorder(javax.swing.BorderFactory.createTitledBorder("TCP"));

        jLabel17.setText("Endereço IP");

        txtIp.setText("127.0.0.1");

        jLabel18.setText("Porta");

        txtPorta.setText("9999");

        javax.swing.GroupLayout jPanel14Layout = new javax.swing.GroupLayout(jPanel14);
        jPanel14.setLayout(jPanel14Layout);
        jPanel14Layout.setHorizontalGroup(
            jPanel14Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel14Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel14Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel14Layout.createSequentialGroup()
                        .addComponent(jLabel17)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED))
                    .addGroup(jPanel14Layout.createSequentialGroup()
                        .addComponent(jLabel18)
                        .addGap(50, 50, 50)))
                .addGroup(jPanel14Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(txtPorta)
                    .addComponent(txtIp, javax.swing.GroupLayout.DEFAULT_SIZE, 254, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanel14Layout.setVerticalGroup(
            jPanel14Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel14Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel14Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel17)
                    .addComponent(txtIp, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel14Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel18)
                    .addComponent(txtPorta, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jPanel14, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel9, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel8, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel5, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel9, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel5, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel8, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel14, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(147, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("Opções", jPanel3);

        jSplitPane3.setLeftComponent(jTabbedPane1);

        mnuAcessos.setText("Acessos");

        mnuCadastroUser.setText("Manutenção de usuários");
        mnuCadastroUser.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                mnuCadastroUserActionPerformed(evt);
            }
        });
        mnuAcessos.add(mnuCadastroUser);

        jMenuBar1.add(mnuAcessos);

        mnuTemas.setText("Temas");
        jMenuBar1.add(mnuTemas);

        mnuAjuda.setText("Ajuda");

        mnuSobre.setText("Sobre");
        mnuSobre.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                mnuSobreActionPerformed(evt);
            }
        });
        mnuAjuda.add(mnuSobre);

        jMenuBar1.add(mnuAjuda);

        setJMenuBar(jMenuBar1);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jSplitPane3, javax.swing.GroupLayout.DEFAULT_SIZE, 918, Short.MAX_VALUE)
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jSplitPane3)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /**
     * Responsável por conectar ou encerrar uma conexão de dados NMEA.
     * @param evt 
     */
    private void btnConectarActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnConectarActionPerformed
    
        //modoConexao = radArquivo.isSelected() ? EnumTipoConexao.Arquivo : EnumTipoConexao.Serial;
        if(radArquivo.isSelected())
            modoConexao = EnumTipoConexao.Arquivo;
        else if(radSerial.isSelected())
            modoConexao = EnumTipoConexao.Serial;
        else
            modoConexao = EnumTipoConexao.TCP;
        
        if(estadoForm == EnumEstadoForm.Ocioso)
        {
            try
            {
                MainClass.getSingleton().iniciar(modoConexao);
                limparForm();
                setEstadoForm(EnumEstadoForm.Conectado);
            }
            catch(LeituraArquivoException ex)
            {
                Util.ExibirMensagem(EnumTipoMensagem.Erro, ex.getMessage());
            }
            catch(SerialPortException ex)
            {
                Util.ExibirMensagem(EnumTipoMensagem.Erro, "Erro ao abrir porta serial.");
            }
            catch(IOException ex)
            {
                Util.ExibirMensagem(EnumTipoMensagem.Erro, "Erro ao abrir conexão com o servidor.");
            }
        }
        else
        {
            MainClass.getSingleton().encerrar();
            setEstadoForm(EnumEstadoForm.Ocioso);
        }
        
    }//GEN-LAST:event_btnConectarActionPerformed

    /**
     * Limpa os campos do form.
     */
    private void limparForm()
    {
        txtRawData.setText("");
        lblLido.setText("0");
        //lblTotal.setText("0");
        dadosGraficoSats.clear();
        dadosGraficoVelox.clear();
    }
    
    /**
     * Usado para selecionar um arquivo NMEA.
     * @param evt 
     */
    private void btnSelecionarArquivoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSelecionarArquivoActionPerformed
        
        if(jfcArquivo.showOpenDialog(null) == JFileChooser.APPROVE_OPTION)
        {
            txtCaminho.setText(jfcArquivo.getSelectedFile().getAbsolutePath());
        }
    }//GEN-LAST:event_btnSelecionarArquivoActionPerformed

    /**
     * Abre uma janela de About.
     * @param evt 
     */
    private void mnuSobreActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_mnuSobreActionPerformed
        Util.ExibirMensagem(EnumTipoMensagem.Informacao, "Criado por Vitor Fiorillo\nRA: 03212619-7");
    }//GEN-LAST:event_mnuSobreActionPerformed

   /**
    * Abre o dialog de manutenção de usuários.
    * @param evt 
    */ 
    private void mnuCadastroUserActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_mnuCadastroUserActionPerformed
        
        frCadastro.show();
        
    }//GEN-LAST:event_mnuCadastroUserActionPerformed

    /**
     * Esconde o dialog de manutenção de usuários.
     * @param evt 
     */
    private void btnCancelarActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCancelarActionPerformed
        frCadastro.setVisible(false);
    }//GEN-LAST:event_btnCancelarActionPerformed

    /**
     * Copia o usuário selecionado no ListBox para o TextBox de usuário,
     * para que seja editado.
     * @param evt 
     */
    private void lstUsersMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_lstUsersMouseClicked
        Object s = lstUsers.getSelectedValue();
        
        if(s != null)
           txtLogin.setText(s.toString());
    }//GEN-LAST:event_lstUsersMouseClicked

    /**
     * Insere ou atualiza um usuário no banco.
     * @param evt 
     */
    private void btnAplicarActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnAplicarActionPerformed
        
        try
        {
            boolean alteracao = ControleAcesso.exists(txtLogin.getText());
            ControleAcesso.setUser(txtLogin.getText(), txtSenha.getText());
            
            if(!alteracao)
            {
                carregarListaUsers();
                Util.ExibirMensagem(EnumTipoMensagem.Informacao, "Usuário incluído com sucesso.");
            }
            else
                Util.ExibirMensagem(EnumTipoMensagem.Informacao, "Usuário alterado com sucesso.");
        }
        catch(Exception erro)
        {
            Util.ExibirMensagem(EnumTipoMensagem.Erro, erro.getMessage());
        }
    }//GEN-LAST:event_btnAplicarActionPerformed

    /**
     * Remove o usuário selecionado no ListBox.
     * @param evt 
     */
    private void btnRemoverActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnRemoverActionPerformed
        try
        {
            Object s = lstUsers.getSelectedValue();
        
            if(s != null)
            {
               ControleAcesso.remover(s.toString());
               Util.ExibirMensagem(EnumTipoMensagem.Informacao, "Usuário removido com sucesso.");
               carregarListaUsers();
            }
        }
        catch(Exception erro)
        {
            Util.ExibirMensagem(EnumTipoMensagem.Erro, erro.getMessage());
        }
    }//GEN-LAST:event_btnRemoverActionPerformed

    /**
     * Salva as strings NMEA num arquivo de texto.
     * @param evt 
     */
    private void btnSalvarActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSalvarActionPerformed
        MainClass.getSingleton().salvarDados();
    }//GEN-LAST:event_btnSalvarActionPerformed

    /**
     * Atualiza o raio no controlador do mapa OSM
     * @param evt 
     */
    private void sldRaioMarcStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldRaioMarcStateChanged
        if(mapOsm == null)
            return;
        
        JSlider source = (JSlider)evt.getSource();
        mapOsm.getMapController().setRaio((float)source.getValue());
        mapOsm.getMapController().refreshCandidato();
        lblRaio.setText(new Integer(sldRaioMarc.getValue()).toString());
    }//GEN-LAST:event_sldRaioMarcStateChanged

    /**
     * Elimina o perimetro vermelho
     * @param evt 
     */
    private void btnCancelarMarcTmpActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCancelarMarcTmpActionPerformed
        mapOsm.eliminarCandidato();
        mapOsm.repaint();
    }//GEN-LAST:event_btnCancelarMarcTmpActionPerformed

    /**
     * Confirma a selecao do perimetro vermelho
     * @param evt 
     */
    private void btnAplicarMarcTmpActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnAplicarMarcTmpActionPerformed
        mapOsm.efetivarPerimetro();
        mapOsm.repaint();
    }//GEN-LAST:event_btnAplicarMarcTmpActionPerformed

    /**
     * Remove todos marcadores de perimetro no mapa
     * @param evt 
     */
    private void btnRemoverTudoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnRemoverTudoActionPerformed
        mapOsm.eliminarCandidato();
        mapOsm.eliminarPerimetro();
        mapOsm.repaint();
    }//GEN-LAST:event_btnRemoverTudoActionPerformed

    private void setEstadoForm(EnumEstadoForm estado)
    {
        estadoForm = estado;
        
        btnConectar.setText(estado == EnumEstadoForm.Conectado ? "Desconectar" : "Conectar");
        btnSelecionarArquivo.setEnabled(estado == EnumEstadoForm.Ocioso);
        cmbPortas.setEnabled(estado == EnumEstadoForm.Ocioso);
        radArquivo.setEnabled(estado == EnumEstadoForm.Ocioso);
        radSerial.setEnabled(estado == EnumEstadoForm.Ocioso);
        radTcp.setEnabled(estado == EnumEstadoForm.Ocioso);
        txtCaminho.setEditable(estado == EnumEstadoForm.Ocioso);
        txtIp.setEditable(estado == EnumEstadoForm.Ocioso);
        txtPorta.setEditable(estado == EnumEstadoForm.Ocioso);
        
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JProgressBar barArquivos;
    private javax.swing.JButton btnAplicar;
    private javax.swing.JButton btnAplicarMarcTmp;
    private javax.swing.JButton btnCancelar;
    private javax.swing.JButton btnCancelarMarcTmp;
    private javax.swing.JButton btnConectar;
    private javax.swing.JButton btnRemover;
    private javax.swing.JButton btnRemoverTudo;
    private javax.swing.JButton btnSalvar;
    private javax.swing.JButton btnSelecionarArquivo;
    private javax.swing.JCheckBox chkAutoScrollRaw;
    private javax.swing.JCheckBox chkLinhas;
    private javax.swing.JCheckBox chkPausar;
    private javax.swing.JComboBox cmbPortas;
    private javax.swing.JDialog frCadastro;
    private javax.swing.ButtonGroup grpModoConexao;
    private com.toedter.calendar.JCalendar jCalendarData;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel12;
    private javax.swing.JLabel jLabel13;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel16;
    private javax.swing.JLabel jLabel17;
    private javax.swing.JLabel jLabel18;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel10;
    private javax.swing.JPanel jPanel11;
    private javax.swing.JPanel jPanel12;
    private javax.swing.JPanel jPanel13;
    private javax.swing.JPanel jPanel14;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel7;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JPanel jPanel9;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JScrollPane jScrollPane4;
    private javax.swing.JSplitPane jSplitPane3;
    private javax.swing.JSplitPane jSplitPane4;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JTabbedPane jTabbedPane2;
    private javax.swing.JFileChooser jfcArquivo;
    private javax.swing.JLabel lblEstadoPosit;
    private javax.swing.JLabel lblLido;
    private javax.swing.JLabel lblRaio;
    private javax.swing.JLabel lblSatsUsados;
    private javax.swing.JLabel lblTotal;
    private javax.swing.JLabel lblTotalSats;
    private javax.swing.JLabel lblTotalSats1;
    private javax.swing.JList lstUsers;
    private javax.swing.JMenu mnuAcessos;
    private javax.swing.JMenu mnuAjuda;
    private javax.swing.JMenuItem mnuCadastroUser;
    private javax.swing.JMenuItem mnuSobre;
    private javax.swing.JMenu mnuTemas;
    private javax.swing.JPanel pnlChartSpeed;
    private javax.swing.JPanel pnlMapConteudo;
    private javax.swing.JPanel pnlMapTab;
    private javax.swing.JPanel pnlMapa3D;
    private javax.swing.JPanel pnlOsm;
    private javax.swing.JPanel pnlSatPlot;
    private javax.swing.JRadioButton radArquivo;
    private javax.swing.JRadioButton radSerial;
    private javax.swing.JRadioButton radTcp;
    private javax.swing.JSlider sldRaioMarc;
    private javax.swing.JTable tabSats;
    private javax.swing.JTextField txtCaminho;
    private javax.swing.JTextField txtDir;
    private javax.swing.JTextField txtIp;
    private javax.swing.JTextField txtLatitude;
    private javax.swing.JTextField txtLogin;
    private javax.swing.JTextField txtLongitude;
    private javax.swing.JTextField txtPorta;
    private javax.swing.JTextArea txtRawData;
    private javax.swing.JPasswordField txtSenha;
    private javax.swing.JTextField txtTime;
    private javax.swing.JTextField txtVelox;
    // End of variables declaration//GEN-END:variables

    /**
     * Aplica um tema às janelas da aplicação.
     * @param e Evento do menu de temas.
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        
        if(e.getSource() instanceof JMenuItem)
        {
            String name = ((JMenuItem)e.getSource()).getText();
            
            try {
                for(UIManager.LookAndFeelInfo info : UIManager.getInstalledLookAndFeels())
                {
                    if(info.getName().equals(name))
                    UIManager.setLookAndFeel(info.getClassName());
                    SwingUtilities.updateComponentTreeUI(this);
                    SwingUtilities.updateComponentTreeUI(jfcArquivo);
                    SwingUtilities.updateComponentTreeUI(frCadastro);
                }
                
            } catch (ClassNotFoundException ex) {
                Logger.getLogger(frDados.class.getName()).log(Level.SEVERE, null, ex);
            } catch (InstantiationException ex) {
                Logger.getLogger(frDados.class.getName()).log(Level.SEVERE, null, ex);
            } catch (IllegalAccessException ex) {
                Logger.getLogger(frDados.class.getName()).log(Level.SEVERE, null, ex);
            } catch (UnsupportedLookAndFeelException ex) {
                Logger.getLogger(frDados.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    /**
     * @author Vitor
     * Classe usada para desenhar um gráfico 2D dos satélites
     * em um JPanel.
     */
    class SkyPlotPanel extends JPanel
    {
        
        public SkyPlotPanel()
        {
            setDoubleBuffered(true);
        }
        
        /**
         * Método usado para ler as informações dos satélites
         * na classe InfoSatelites e desenhar um gráfico de elevação
         * e azimute correspondente.
         * @param g 
         */
        @Override
        public void paintComponent(Graphics g)
        {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D)g;
            
            int cx = getWidth() / 2;            //centro x e y
            int cy = getHeight() / 2;
            int menor = Math.min(cx, cy);       //pra desenhar com base na menor dimensao
            int r = 8 * menor / 10;             //raio
            
            g2.drawOval(cx - r, cy - r, r * 2, r * 2);
            g2.drawOval(cx - r / 2, cy - r / 2, r, r );
            
            //Desenhar letras ----------------------------------------
            int grau = -90;
            char[] letras = new char[] { 'N', 'L', 'S', 'O' };
            
            for(int i = 0; i < 4 ; i++)
            {
                int x = (int)Math.round(cx + (10 + r) * Math.cos(Math.toRadians(grau)));
                int y = (int)Math.round(cy + (10 + r) * Math.sin(Math.toRadians(grau)));
                
                g2.drawChars(new char[] {letras[i]}, 0, 1, x - 3, y + 3);
                
                grau += 90;
            }
            //Desenhar letras ----------------------------------------
            
            //Linhas auiliares ---------------------------------------
            g2.setColor(Color.GRAY);
            grau = 0;
            for(int i = 0; i < 4; i++)
            {
                int x = (int)Math.round(cx + r * Math.cos(Math.toRadians(grau)));
                int y = (int)Math.round(cy + r * Math.sin(Math.toRadians(grau)));
                int a = (int)Math.round(cx + r * Math.cos(Math.toRadians(grau + 180)));
                int b = (int)Math.round(cy + r * Math.sin(Math.toRadians(grau + 180)));
                
                g2.drawLine(x, y, a, b);
                
                grau += 45;
            }
            //Linhas auiliares ---------------------------------------
            
            for(InfoSatelite info : InfoSatelite.getSats())
            {
                //o desconto no azimute e porque o grau zero corresponde a direita
                //nao ao topo
                float elevacao;
                float azimute;
                try
                {
                    elevacao = (float)(Float.parseFloat(info.getElevacao()));
                    azimute = (float)Math.toRadians(Float.parseFloat(info.getAzimute()) - 90);
                }
                catch(Exception erro)
                {
                    //Alguns dispositivos não passam um dos dois dados, então
                    //não é possivel desenhar o satélite no mapa.
                    continue;
                }

                int satRaio = Math.round(Util.map(elevacao, 0f, 90f, r, 0));

                //formulas para ponto na borda do circulo
                //int x = cx + raio * cos(ang);
                //int y = cy + raio * sin(ang);

                int x = (int)Math.round(cx + satRaio * Math.cos(azimute));
                int y = (int)Math.round(cy + satRaio * Math.sin(azimute));
                
                g2.setColor(Color.BLACK);
                letras = info.getPrn().toCharArray();
                g2.drawChars(letras,0, letras.length, x + 6, y + 6);
                
                g2.setColor(Color.RED);
                if(info.isUsadoNoFix())
                {
                    g2.fillOval(x -5,y - 5, 10, 10);
                }
                else
                {
                    g2.drawOval(x -5,y - 5, 10, 10);
                }
            }
        }
    }
}
