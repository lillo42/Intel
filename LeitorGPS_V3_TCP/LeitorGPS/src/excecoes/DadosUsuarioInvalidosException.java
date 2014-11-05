/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package excecoes;

/**
 * Classe usada para indicar que argumentos inválidos foram
 * passados para um objeto UsuarioVO
 * 
 * @author Vitor
 */
public class DadosUsuarioInvalidosException extends Exception{
    
    public DadosUsuarioInvalidosException()
    {
        super("O login e senha devem possuir ao menos três caracteres cada.");
    }
}
