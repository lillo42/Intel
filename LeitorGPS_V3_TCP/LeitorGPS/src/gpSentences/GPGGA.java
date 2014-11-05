/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package gpSentences;

import excecoes.SentencaInvalidaException;
import util.Time;

/**
 * Classe para representar uma sentença GPGGA.
 * 
 * @author Vitor
 */
public class GPGGA {
    
    private Time utcTime;
    private String latitude;
    private String latiChar;
    private String longitude;
    private String longitudeChar;
    private String fixQuality;
    private int numSatelites;
    private String altitude;
    private String medidaAltitude;

    public GPGGA(String sentenca) throws SentencaInvalidaException
    {
        String[] dados = sentenca.split(",", -2);
        
        try
        {
            utcTime = new Time(dados[0]);
            latitude = dados[1];
            latiChar = dados[2];
            longitude = dados[3];
            longitudeChar = dados[4];
            fixQuality = dados[5];
            numSatelites = Integer.parseInt(dados[6]);
            altitude = dados[10];
            medidaAltitude = dados[11];
        }
        catch(Exception e)
        {
            throw new SentencaInvalidaException();
        }
    }
}
