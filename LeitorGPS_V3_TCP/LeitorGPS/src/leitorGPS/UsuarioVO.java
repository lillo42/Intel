/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import excecoes.DadosUsuarioInvalidosException;

/**
 * Clase usada para representar um usuário.
 * 
 * @author Vitor
 */
public class UsuarioVO {
    
    private String login;
    private String senha;

    public UsuarioVO(String login, String senha) throws DadosUsuarioInvalidosException {
        setLogin(login);
        setSenha(senha);
    }
    
    public UsuarioVO()
    {
    }

    /**
     * Retorna o login do usuário.
     * @return O login do usuário.
     */
    public String getLogin() {
        return login;
    }

    /**
     * Define o login do usuário.
     * @param login O login do usuário.
     * @throws DadosUsuarioInvalidosException Caso o login não obedeça o requisito mínimo de caracteres.
     */
    public void setLogin(String login) throws DadosUsuarioInvalidosException {
        
        if(login.trim().length() < 3)
            throw new DadosUsuarioInvalidosException();
        else
            this.login = login.trim();
    }

    /**
     * Retorna a senha do usuário.
     * @return A senha do usuário.
     */
    public String getSenha() {
        return senha;
    }

    /**
     * Define a senha do usuário.
     * @param senha A senha do usuário.
     * @throws DadosUsuarioInvalidosException Caso a senha não obedeça o requisito mínimo de caracteres.
     */
    public void setSenha(String senha) throws DadosUsuarioInvalidosException {
        if(senha.trim().length() < 3)
            throw new DadosUsuarioInvalidosException();
        this.senha = senha.trim();
    }
    
    
}
