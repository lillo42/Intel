/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package excecoes;

/**
 * Classe usada para indicar um erro ao tentar abrir um arquivo de strings
 * NMEA.
 * 
 * @author Vitor
 */
public class LeituraArquivoException extends Exception{
    
    public LeituraArquivoException()
    {
        super("Ocorreu um erro ao tentar ler o arquivo.\nCheque o caminho.");
    }
}
