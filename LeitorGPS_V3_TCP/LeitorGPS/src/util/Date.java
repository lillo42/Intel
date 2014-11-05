/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

import excecoes.DataInvalidaException;

/**
 * Classe criada para representar uma data simples vinda de uma string NMEA.
 * 
 * @author Vitor
 */
public class Date {
    
    private String dia, mes, ano;
    
    public Date(String data) throws DataInvalidaException
    {
        if(data.length() == 6)
            ano = data.substring(4,6);
        else if(data.length() == 8)
            ano = data.substring(4,8);
        else
            throw new DataInvalidaException();
        
        dia = data.substring(0,2);
        mes = data.substring(2,4);
    }

    /**
     * Retorna uma string formatada com os dados da data.
     * @return Uma string no formato dd/mm/aa
     */
    @Override
    public String toString() {
        return dia + "/" + mes + "/" + ano;
    }
    
    /**
     * Retorna o dia da data.
     * @return O dia da data.
     */
    public String getDia() {
        return dia;
    }

    /**
     * Retorna o mês da data.
     * @return O mês da data.
     */
    public String getMes() {
        return mes;
    }

    /**
     * Retorna o ano da data.
     * @return O ano da data.
     */
    public String getAno() {
        return ano;
    }
    
    
}
