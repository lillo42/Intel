/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package excecoes;

/**
 * Classe Usada para indicar que um objeto tempo deve possuir exatamente
 * seis dígitos.
 * 
 * @author Vitor
 */
public class TempoInvalidoException extends Exception{
    
    public TempoInvalidoException()
    {
        super("O argumento String deve ter exatamente 6 dígitos.");
    }
}
