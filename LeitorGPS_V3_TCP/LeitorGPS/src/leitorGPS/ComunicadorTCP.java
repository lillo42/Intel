/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.SerialPort;

/**
 *
 * @author vitor
 */
public class ComunicadorTCP implements IComunicador {

    private ArrayList<INMEAListener> observers = new ArrayList<>();
    private String host;
    private int portNum;
    private Socket socket;
    private boolean conFechada = false;
    
    private PrintWriter out;
    private BufferedReader in;
    
    private String buffer = "";
    private final String COD_FECHAR_CONEXAO = "fecharTCP";
    private Thread tcpThread;
    
    public ComunicadorTCP(String host, int portNum) throws IOException
    {
        this.host = host;
        this.portNum = portNum;
        socket = new Socket(host, portNum);
        
        out = new PrintWriter(socket.getOutputStream(),true);
        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        
        initThread();
    }
    
    private void initThread()
    {
        //Thread que recebe os dados da rede
        tcpThread = new Thread(){
          
            @Override
            public void run(){
                while(!conFechada)
                {
                    try {
                        while ((buffer = in.readLine()) != null) {
                            if (buffer.equals(COD_FECHAR_CONEXAO))
                            {
                                conFechada = true;
                                break;
                            }
                            
                            System.out.println("Li + " + buffer);
                            dadoRecebido(buffer);
                        }       
                    } 
                    catch (IOException ex) {
                        Logger.getLogger(ComunicadorTCP.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
                
                //Encerrar conexao
                if(socket != null && !socket.isClosed())   
                    try {
                        socket.close();
                    }
                    catch (IOException ex) {
                        Logger.getLogger(ComunicadorTCP.class.getName()).log(Level.SEVERE, null, ex);
                    }
            }
            
        };
        tcpThread.start();
    }
    
    @Override
    public void addListener(INMEAListener obj) {
        if(!observers.contains(obj))
            observers.add(obj);
    }

    @Override
    public void dadoRecebido(String dado) {
        for(INMEAListener obj : observers)
        {
            obj.dadoNMEARecebido(dado);
        }
    }

    @Override
    public void fecharConexao() {
        conFechada = true;
    }
    
}
