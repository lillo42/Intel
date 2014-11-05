/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

/**
 * Interface criada para representar objetos que precisam
 * ouvir ao evento de atualização de informações de satélite.
 * A classe InfoSatelite possui uma lista com objetos que implementam
 * essa interface e os notifica quando ocorrem atualizações.
 * 
 * @author Vitor
 */
public interface ISatInfoListener {
    
    /**
     * Método tratador do evento de atualização de informações
     * de satélite.
     */
    void updateSatInfo();
    
}
