/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

/**
 * Classe criada para representar um objeto capaz de ouvir ao evento
 * de chegada de dados NMEA brutos e processá-los de alguma forma.
 * 
 * @author Vitor
 */
public interface INMEAListener {
    
    /**
     * Método tratador do evento de chegada de dados.
     * @param dado Os dados NMEA que chegaram.
     */
    void dadoNMEARecebido(String dado);
    
    /**
     * As classes que implementam a interface INMEAListener devem
     * possuir uma lista com ouvintes do evento de chegada de dados.
     * @param obj Um objeto que implementa a interface IGPListener
     */
    void addListener(IGPListener obj);
}
