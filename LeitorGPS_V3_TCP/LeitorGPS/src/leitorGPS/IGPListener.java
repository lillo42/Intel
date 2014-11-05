/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import gpSentences.GPSentence;

/**
 * Interface criada para representar um objeto capaz de ouvir o evento de
 * chegada de objetos GPSentence e processá-los de alguma forma.
 * 
 * @author Vitor
 */
public interface IGPListener {
    
    /**
     * Método que irá tratar a chegada do objeto GPSentence mapeado.
     * @param obj O objeto GPSentence mapeado.
     */
    void dadosRecebidos(GPSentence obj);
    
    /**
     * Método que irá tratar a chegada de uma string NMEA não mapeada.
     * @param obj A chegada de uma string NMEA não mapeada.
     */
    void stringRecebida(String obj);
}