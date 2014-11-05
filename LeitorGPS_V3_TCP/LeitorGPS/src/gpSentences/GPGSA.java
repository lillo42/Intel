/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package gpSentences;

import excecoes.SentencaInvalidaException;
import java.util.ArrayList;

/**
 * Classe para representar uma sentença GPGSA.
 * 
 * @author Vitor
 */
public class GPGSA extends GPSentence{
    
    private String modo;                //M manual, A automatico - modo do Fix
    private String tipoFix;             //1 sem fix, 2 fix 2d, 3 fix 3d
    private ArrayList<Integer> satFix;  //satelites usados no fix
    private String pdop,hdop,vdop;
    
    public GPGSA(String sentenca) throws SentencaInvalidaException
    {
        String[] dados = sentenca.split(",", -2);
        
        try
        {
            modo = dados[0];
            tipoFix = dados[1];
            satFix = new ArrayList<>();
            
            for(int i = 2; i < 14; i++)
            {
                try
                {
                    int satId = Integer.parseInt(dados[i]);
                    satFix.add(satId);
                }
                catch(Exception e)
                {
                    // se não converter é pq o campo não é usado
                    // portanto, nao usado no fix
                }
            }
            
            pdop = dados[14];
            hdop = dados[15];
            vdop = dados[16];
        }
        catch(Exception e)
        {
            throw new SentencaInvalidaException();
        }
    }

    /**
     * Retorna o modo de conexão
     * @return M para manual e A para automática
     */
    public String getModo() {
        return modo;
    }

    /**
     * Retorna o tipo de localização empregada
     * @return Retorna 1 para "sem localização", 2 para "localização 2D" e 3 para "localização 3D"
     */
    public String getTipoFix() {
        return tipoFix;
    }

    /**
     * Retorna uma lista com todos satélites contidos
     * nessa sentença.
     * @return A lista com os satélites usados no cálculo de localização.
     */
    public ArrayList<Integer> getSatFix() {
        return satFix;
    }

    /**
     * Retorna PDOP.
     * @return PDOP.
     */
    public String getPdop() {
        return pdop;
    }

    /**
     * Retorna HDOP.
     * @return HDOP.
     */
    public String getHdop() {
        return hdop;
    }

    /**
     * Retorna VDOP.
     * @return VDOP.
     */
    public String getVdop() {
        return vdop;
    }
    
    
}
