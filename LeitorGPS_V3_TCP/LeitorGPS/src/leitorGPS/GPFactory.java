/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import excecoes.LeituraArquivoException;
import excecoes.SentencaInvalidaException;
import java.util.ArrayList;
import gpSentences.*;
import java.io.IOException;
import jssc.SerialPortException;
/**
 * Classe responsável por intermediar comunicação entre classe principal e classes
 * receptoras de dados NMEA.
 * 
 * @author Vitor
 */
public class GPFactory implements INMEAListener{
    
    private IComunicador objComunicador;
    private ArrayList<IGPListener> observers = new ArrayList<>();
    
    /**
     * Inicia conexão de dados oriundos de um endereço e porta TCP
     * @param endereco O endereço de onde virão os dados
     * @param numPorta A porta do servidor de dados
     * @throws IOException Caso haja um erro ao tentar se conectar com o servidor no endereço especificado
     */
    public void iniciarTCP(String endereco, int numPorta) throws IOException
    {
        fecharConexoes();
        
        objComunicador = new ComunicadorTCP(endereco, numPorta);
        objComunicador.addListener(this);
    }
    
    /**
     * Inicia uma conexão de dados oriundos de dispositivo serial
     * @param porta A porta serial de onde vem os dados
     * @throws SerialPortException Caso ocorra algum erro de comunicação
     */
    public void iniciarGPS(String porta) throws SerialPortException
    {
        fecharConexoes();
        
        objComunicador = new ComunicadorSerial(porta);
        objComunicador.addListener(this);
    }
    
    /**
     * Inicia uma conexão de dados oriundos de um arquivo NMEA
     * @param caminho O caminho do arquivo
     * @throws LeituraArquivoException Caso ocorra algum erro na leitura do arquivo
     */
    public void iniciarArquivo(String caminho) throws LeituraArquivoException 
    {   
        fecharConexoes();
        
        try
        {
            objComunicador = new ComunicadorArquivo(caminho);
            objComunicador.addListener(this);
        }
        catch(Exception ex)
        {
            throw new LeituraArquivoException();
        }
        
    }

    /**
     * Fecha a conexão existente para que outra possa ser aberta.
     */
    public void fecharConexoes()
    {
        if(objComunicador != null)
            objComunicador.fecharConexao();
    }
    
    /**
     * Método que trata os dados recebidos das classes comunicadoras de strings NMEA.
     * A string é mapeada para um objeto GPSentence correspondente que é enviado para 
     * os ouvintes
     * @param dado A string recebida.
     */
    @Override
    public void dadoNMEARecebido(String dado) 
    {
        if(dado.charAt(0) != '$')
            return;
        
        String tipoSentenca = dado.substring(1,6);
        String vetorDados = dado.substring(7);
        GPSentence objSentenca = null;
        
        try
        {
            if(tipoSentenca.equals("GPRMC"))
            {
                objSentenca = new GPRMC(vetorDados);
            }
            else if(tipoSentenca.equals("GPGSV"))
            {
                objSentenca = new GPGSV(vetorDados);
            }
            else if(tipoSentenca.equals("GPGSA"))
            {
                objSentenca = new GPGSA(vetorDados);
            }
        }
        catch(SentencaInvalidaException sen)
        {
            System.out.println(sen.getMessage());
            System.out.println(dado);
        }
        
        for(IGPListener obj : observers)
        {
            if(objSentenca != null)
                obj.dadosRecebidos(objSentenca);
            
            obj.stringRecebida(dado);
        }
    }

    /**
     * Adiciona um objeto à lista de ouvintes do evento de chegada de sentença.
     * @param obj Um objeto que implemente a interface IGPListener
     */
    @Override
    public void addListener(IGPListener obj) 
    {
        if(!observers.contains(obj))
            observers.add(obj);
    }
}
