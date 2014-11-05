/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package gpSentences;

import excecoes.SentencaInvalidaException;
import java.util.ArrayList;
import leitorGPS.InfoSatelite;

/**
 * Classe usada para representar uma sentença GPGSV.
 * 
 * @author Vitor
 */
public class GPGSV extends GPSentence{
    
    private int qtdMessages;
    private int idMessage;
    private int totalSatsView;
    private ArrayList<InfoSatelite> sats = new ArrayList<InfoSatelite>();
    
    public GPGSV(String sentenca) throws SentencaInvalidaException
    {
        String[] dados = sentenca.split(",", -2);
        
        try
        {
            qtdMessages = Integer.parseInt(dados[0]);
            idMessage = Integer.parseInt(dados[1]);
            totalSatsView = Integer.parseInt(dados[2]);
            
            for(int i = 0; i < 4; i++)
            {
                //a cada quatro campos, um satelite novo
                int init = 3 + (i * 4);
                int end = 6 + (i * 4);    
                
                String satString = "";
                
                try
                {
                    for(int x = init; x < end + 1;x++)
                    {
                        satString += dados[x] + ",";
                    }
                }
                catch(Exception erro)
                {
                    continue;
                }
                
                InfoSatelite sat = satFromString(satString);
                
                if(sat != null)
                    sats.add(sat);
            }
        }
        catch(Exception erro)
        {
            throw new SentencaInvalidaException();
        }
    }
    
    /**
     * Recebe uma string com dados sobre um satélite e devolve um objeto correspondete.
     * @param dados A string com os dados do satélite.
     * @return Um objeto com os campos mapeados.
     */
    private InfoSatelite satFromString(String dados)
    {
        String[] vet = dados.split(",",-2);
        
        try
        {
            int idSat = Integer.parseInt(vet[0]);

            InfoSatelite sat = new InfoSatelite();
            sat.setPrn(Integer.toString(idSat));
            sat.setElevacao(vet[1]);
            sat.setAzimute(vet[2]);
            sat.setSnr(vet[3].split("[*]")[0]);
            
            return sat;
        }
        catch(Exception e)
        {
            return null;
        }
    }

    /**
     * Retorna o total de sentenças nesse ciclo.
     * @return O total de sentenças GPGSV nesse ciclo.
     */
    public int getQtdMessages() {
        return qtdMessages;
    }

    /**
     * Retorna o identificador da sentença nesse ciclo.
     * @return O identificador da sentença GPGSV nesse ciclo.
     */
    public int getIdMessage() {
        return idMessage;
    }

    /**
     * Retorna o total de satélites vistos pelo GPS.
     * @return O total de satélites vistos pelo GPS.
     */
    public int getTotalSatsView() {
        return totalSatsView;
    }

    /**
     * Retorna todos os satélites contidos nessa string
     * @return Uma lista com todos os satélites
     */
    public ArrayList<InfoSatelite> getSats() {
        return sats;
    }
    
    
}
