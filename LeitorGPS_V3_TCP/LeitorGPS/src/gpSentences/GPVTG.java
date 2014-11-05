/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package gpSentences;

import excecoes.SentencaInvalidaException;

/**
 * Classe usada para representar uma sentença GPVTG.
 *
 * @author Vitor
 */
public class GPVTG extends GPSentence{
    
    private String anguloT;
    private String medidaAnguloT;
    private String anguloM;
    private String medidaAnguloG;
    private String veloxN, campoN;
    private String veloxK, campoK;

    public GPVTG(String sentenca) throws SentencaInvalidaException 
    {
        String[] dados = sentenca.split(",", -2);
        
        try
        {
            this.anguloT = dados[0];
            this.medidaAnguloT = dados[1];
            this.anguloM = dados[2];
            this.medidaAnguloG = dados[3];
            this.veloxN = dados[4];
            this.campoN = dados[5];
            this.veloxK = dados[6];
            this.campoK = dados[7];
        }
        catch (Exception erro)
        {
            throw new SentencaInvalidaException();
        }
        
        
    }

    public String getAnguloT() {
        return anguloT;
    }

    public String getMedidaAnguloT() {
        return medidaAnguloT;
    }

    public String getAnguloM() {
        return anguloM;
    }

    public String getMedidaAnguloG() {
        return medidaAnguloG;
    }

    public String getVeloxN() {
        return veloxN;
    }

    public String getCampoN() {
        return campoN;
    }

    public String getVeloxK() {
        return veloxK;
    }

    public String getCampoK() {
        return campoK;
    }
    
    
    
}
