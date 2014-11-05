/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

import excecoes.TempoInvalidoException;

/**
 * Classe simples para representar o tempo em uma string NMEA.
 * 
 * @author Vitor
 */
public class Time {
    
    private String hora, minuto, segundo;
    
    public Time(String time) throws TempoInvalidoException
    {
        if(time.length() != 6)
            throw new TempoInvalidoException();
        
        hora = time.substring(0,2);
        minuto = time.substring(2,4);
        segundo = time.substring(4,6);
    }

    /**
     * Retorna uma string formatada para esse objeto.
     * @return Uma string no formato hh:mm:ss
     */
    @Override
    public String toString() {
        return hora + ":" + minuto + ":" + segundo;
    }

    /**
     * Retorna a hora do objeto.
     * @return A hora do objeto.
     */
    public String getHora() {
        return hora;
    }

    /**
     * Retorna os minutos do objeto.
     * @return Os minutos do objeto.
     */ 
    public String getMinuto() {
        return minuto;
    }

    /**
     * Retorna os segundos do objeto.
     * @return Os segundos do objeto.
     */
    public String getSegundo() {
        return segundo;
    }    
    
}
