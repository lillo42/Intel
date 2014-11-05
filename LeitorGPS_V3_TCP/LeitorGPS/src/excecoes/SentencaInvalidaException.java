/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package excecoes;

/**
 * Classe usada para indicar que uma sentença obtida
 * não obedece ao formato NMEA.
 * 
 * @author Vitor
 */
public class SentencaInvalidaException extends Exception{
    
    public SentencaInvalidaException()
    {
        super("A sentença NMEA não atende ao formato do tipo especificado");
    }
}
