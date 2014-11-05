/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.*;
/**
 * Classe usada para ler strings NMEA de um dispositivo serial.
 * 
 * @author Vitor
 */
public class ComunicadorSerial implements IComunicador, SerialPortEventListener{

    private ArrayList<INMEAListener> observers = new ArrayList<INMEAListener>();
    private SerialPort sp;
    private String buffer;
    private byte[] byteBuffer;
    
    public ComunicadorSerial(String porta) throws SerialPortException
    {
        sp = new SerialPort(porta);
        sp.openPort();
        sp.setParams(9600, 8, 1, SerialPort.PARITY_NONE);
        sp.setEventsMask(SerialPort.MASK_RXCHAR);
        sp.addEventListener(this);
    }
    
    /**
     * Retorna a lista de portas seriais disponíveis.
     * @return A lista de portas seriais disponíveis no sistema.
     */
    public static String[] getPortasDisponiveis()
    {
        return SerialPortList.getPortNames();
    }
    
    /**
     * Encerra a conexão serial.
     */
    @Override
    public void fecharConexao()
    {
        if(sp != null && sp.isOpened())
        {
            try
            {
                sp.closePort();    
            }
            catch(SerialPortException ex)
            {
                Logger.getLogger(ComunicadorSerial.class.getName()).log(Level.SEVERE, null, ex);
            }
            
        }
    }
    
    /**
     * Adiciona um objeto a lista de ouvintes do evento de chegada de dados.
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
     * Método tratador dos eventos da porta serial.
     * @param evt O evento que será tratado. Apenas eventos de recepção de caractere são tratados.
     */
    @Override
    public void serialEvent(SerialPortEvent evt) {
        
        if(evt.isRXCHAR())
        {
            try 
            {
                byteBuffer = sp.readBytes();
                
                if(byteBuffer != null)
                {
                    for(int i = 0; i < byteBuffer.length; i++)
                    {
                        String c = Character.toString((char)byteBuffer[i]);
                        buffer += c;
                        
                        if(c.equals("\n"))
                        {
                            dadoRecebido(buffer);
                            buffer = "";
                        }
                    }
                }
               
            } 
            catch (SerialPortException ex) {
                Logger.getLogger(ComunicadorSerial.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

}
