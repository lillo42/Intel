/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

/**
 * Interface criada para abstrair o conceito de receptor de dados NMEA.
 * Dessa forma, foi possível criar um ComunicadorSerial e um ComunicadorArquivo,
 * e seria possível criar classes para obter dados de outras fontes, como a internet.
 * 
 * @author Vitor
 */
public interface IComunicador {
    
    /**
     * Os objetos que implementam a interface IComunicador devem possuir uma
     * lista com objetos ouvintes, para que sejam notificados no evento de chegada
     * de dados.
     * @param obj Um objeto que implementa a interface INMEAListener
     */
    void addListener(INMEAListener obj);
    
    /**
     * Método que irá notificar os ouvintes.
     * @param dado Dado que será enviado aos ouvintes.
     */
    void dadoRecebido(String dado);
    
    /**
     * Método que é chamado para encerrar uma conexão, caso exista.
     */
    void fecharConexao();
    
}
