/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import java.io.FileInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Classe usada para ler Strings NMEA a partir de um arquivo.
 * 
 * @author Vitor
 */
public class ComunicadorArquivo implements IComunicador, Runnable{
    
    private ArrayList<INMEAListener> observers = new ArrayList<>();
    private String caminho;
    private Thread thread;
    private boolean podeRodar = true;
    
    public ComunicadorArquivo(String caminho)
    {
        this.caminho = caminho;
        thread = new Thread(this);
        thread.start();
    }
    
    /**
     * Usado para finalizar a thread de leitura.
     */
    @Override
    public void fecharConexao() 
    {
        podeRodar = false;
    }
    
    /**
     * Adiciona um objeto à lista de ouvintes do evento de chegada de dados.
     * @param obj Um objeto que implemente a interface INMEAListener
     */
    @Override
    public void addListener(INMEAListener obj) 
    {
        if(!observers.contains(obj))
            observers.add(obj);
    }

    /**
     * Avisa todos os ouvintes sobre o dado que chegou.
     * @param dado O dado que será enviado para os ouvintes.
     */
    @Override
    public void dadoRecebido(String dado) 
    {
        for(INMEAListener obj : observers)
        {
            obj.dadoNMEARecebido(dado);
        }
    }

    /**
     * Inicializa thread leitora do arquivo.
     */
    @Override
    public void run() 
    {
        File f = new File(caminho);
        FileInputStream fis = null;
        
        String buffer = "";
        try
        {
            fis = new FileInputStream(f);
            int content;
            
            while(podeRodar && (content = fis.read()) != -1)
            {
                buffer += (char)content;
                if((char)content == '\n')
                {
                    dadoRecebido(buffer);
                    buffer = "";
                    Thread.sleep(200);
                }
            }
        }
        catch(FileNotFoundException ex)
        {
             Logger.getLogger(ComunicadorArquivo.class.getName()).log(Level.SEVERE, null, ex);
        }
        catch (IOException | InterruptedException ex) 
        {
            Logger.getLogger(ComunicadorArquivo.class.getName()).log(Level.SEVERE, null, ex);
        }
        finally
        {
            if(fis != null)
            {
                try 
                {
                    fis.close();
                } 
                catch (IOException ex)
                {
                    Logger.getLogger(ComunicadorArquivo.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }
    
}
