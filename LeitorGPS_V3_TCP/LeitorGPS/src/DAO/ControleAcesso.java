/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package DAO;

import com.db4o.Db4o;
import com.db4o.ObjectContainer;
import com.db4o.ObjectSet;
import excecoes.DadosUsuarioInvalidosException;
import java.util.ArrayList;
import leitorGPS.UsuarioVO;

/**
 * Classe usada para forncer funcionalidade de autenticação ao sistema,
 * nesse caso, através do banco de dados Db4o.
 * 
 * @author Vitor
 */
public class ControleAcesso 
{
    /**
     * Abre o arquivo de usuários.
     * @return O arquivo de usuários aberto e pronto para uso.
     */
    private static ObjectContainer getCon()
    {
        ObjectContainer db = Db4o.openFile("passwd.yap");
        return db;
    }
    
    /**
     * Checa se usuário e senha correspondem a um usuário válido no banco
     * @param login O login do usuário
     * @param pass A senha do usuário
     * @return True se for válido
     * @throws DadosUsuarioInvalidosException Caso os valores passados não obedeçam ao requisito mínimo de três caracteres cada.
     */
    public static boolean isValid(String login, String pass) throws DadosUsuarioInvalidosException
    {
        ObjectContainer db = getCon();
        
        try
        {
            UsuarioVO user = new UsuarioVO(login, pass);

            if(db.queryByExample(user).hasNext())
                return true;
            else 
                return false;
        }
        finally
        {
            db.close();
        }
    }
    
    /**
     * Salva um usuário no banco, alterando ou inserindo, conforme o caso.
     * @param login O login do usuário
     * @param pass A senha do usuário
     * @throws DadosUsuarioInvalidosException Caso os valores passados não obedeçam ao requisito mínimo de três caracteres cada.
     */
    public static void setUser(String login, String pass) throws DadosUsuarioInvalidosException
    {
        
        ObjectContainer db = getCon();
        
        try
        {
            UsuarioVO user = new UsuarioVO();
            user.setLogin(login);

            ObjectSet<UsuarioVO> objs = db.queryByExample(user);

            if(objs.hasNext())
            {
                user = objs.next();
                user.setSenha(pass);
            }
            else
            {
                user.setSenha(pass);
            }

            db.store(user);
        }
        finally
        {
            db.close();
        }
        
    }
    
    /**
     * Checa se um usuário existe no banco.
     * @param login O login a ser checado
     * @return True se existir
     * @throws DadosUsuarioInvalidosException Caso os valores passados não obedeçam ao requisito mínimo de três caracteres cada.
     */
    public static boolean exists(String login) throws DadosUsuarioInvalidosException
    {
         ObjectContainer db = getCon();
        
        try
        {
            UsuarioVO user = new UsuarioVO();
            user.setLogin(login);

            ObjectSet<UsuarioVO> objs = db.queryByExample(user);

            if(objs.hasNext())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        finally
        {
            db.close();
        }
    }
    
    /**
     * Remove um usuário do banco
     * @param login O login a ser removido
     * @throws DadosUsuarioInvalidosException Caso os valores passados não obedeçam ao requisito mínimo de três caracteres cada.
     */
    public static void remover(String login) throws DadosUsuarioInvalidosException
    {
        ObjectContainer db = getCon();
        
        try
        {
            UsuarioVO user = new UsuarioVO();
            user.setLogin(login);

            ObjectSet<UsuarioVO> objs = db.queryByExample(user);

            if(objs.hasNext())
            {
                UsuarioVO del = objs.next();
                db.delete(del);
            }
        }
        finally
        {
            db.close();
        }
    }
    
    /**
     * Retorna uma lista com todos os usuários do banco
     * @return A lista com todos os usuários
     */
    public static ArrayList<String> getUsers()
    {
        ArrayList<String> users = new ArrayList<>();
        ObjectContainer db = getCon();
        
        try
        {
            ObjectSet<UsuarioVO> objs = db.queryByExample(null);
            while(objs.hasNext())
            {
                UsuarioVO user = objs.next();
                users.add(user.getLogin());
            }
            
            return users;
        }
        finally
        {
            db.close();
        }
    }   
}
