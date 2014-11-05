/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package DAO;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Responsável por salvar uma string contendo todas
 * as sentenças recebidas na conexão
 * 
 * @author Vitor
 */
public class GPDAO 
{
    
    /**
     * 
     * @param dados A string com todos as sentenças a serem salvas
     * @throws IOException Caso ocorra algum erro na gravação
     */
    public static void salvar(String dados) throws IOException
    {
        Date d = new Date();
        String data = new SimpleDateFormat("dd-MM-yyyy_hh-mm-ss").format(d);
                
        File f = new File("dadosNMEA_" + data + ".txt");
        FileOutputStream fos = new FileOutputStream(f,false);
        
        try
        {
            byte[] vec = dados.getBytes();
            fos.write(vec);
        }
        finally
        {
            fos.close();
        }
    }
    
}
