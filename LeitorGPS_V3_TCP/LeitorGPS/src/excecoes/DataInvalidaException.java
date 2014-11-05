/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package excecoes;

/**
 * Classe usada para indicar que uma data deve possuir exatamente
 * seis dígitos, como no padrão NMEA
 * 
 * @author Vitor
 */
public class DataInvalidaException extends Exception{
    
    public DataInvalidaException()
    {
        super("O argumento String deve ter exatamente 6 dígitos.");
    }
}
