/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import DAO.ControleAcesso;
import DAO.GPDAO;
import excecoes.DadosUsuarioInvalidosException;
import excecoes.LeituraArquivoException;
import forms.frDados;
import forms.frLogin;
import gpSentences.GPGSA;
import gpSentences.GPGSV;
import gpSentences.GPSentence;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import jssc.SerialPortException;
import util.Util;
import util.Util.*;

/**
 * Classe principal da aplicação.
 * Abre o form de login e o form de dados.
 * É uma classe singleton, só possui uma instância.
 * 
 * @author Vitor
 */
public class MainClass implements IGPListener{
    
    private frDados fr;
    private frLogin frLogin;
    private GPFactory gpFac;
    private static MainClass mainClass;
    
    private int qtdLinhasFile = 0;
    
    public MainClass()
    {
        init();
        return;
        
        //frLogin = new frLogin();
        //frLogin.setVisible(true);
    }
    
    /**
     * Retorna o objeto singleton dessa classe.
     * @return O objeto singleton dessa classe.
     */
    public static MainClass getSingleton() {
        return mainClass;
    }
    
    /**
     * Inicia uma conexão de dados NMEA.
     * @param tipo O tipo da conexão. Serial ou Arquivo.
     * @throws LeituraArquivoException Caso ocorra erro de leitura de arquivo.
     * @throws SerialPortException Caso ocorra erro de comunicação serial.
     */
    public void iniciar(EnumTipoConexao tipo) throws LeituraArquivoException, SerialPortException, IOException
    {
        String caminho = fr.getCaminho();
        
        if(tipo == EnumTipoConexao.Arquivo)
        {
            qtdLinhasFile = contarLinhas(caminho);
            fr.setTotalRegistros(Integer.toString(qtdLinhasFile));
            gpFac.iniciarArquivo(caminho);
        }
        else if(tipo == EnumTipoConexao.Serial)
        {
            fr.setTotalRegistros("0");
            gpFac.iniciarGPS(fr.getPortaSerial());
        }
        else if(tipo == EnumTipoConexao.TCP)
        {
            fr.setTotalRegistros("0");
            gpFac.iniciarTCP(fr.getEnderecoTCP(), fr.getPortaTCP());
        }
    }
    
    /**
     * Encerra a conexão do objeto gpFactory.
     */
    public void encerrar()
    {
        gpFac.fecharConexoes();
    }
    
    /**
     * Chamado quando o usuário se autentica no sistema.
     * Abre o form de dados.
     */
    private void init()
    {
        fr = new frDados();
        InfoSatelite.addListener(fr);
        
        gpFac = new GPFactory();
        gpFac.addListener(this);
    }
    
    /**
     * Salva os dados da TextArea em um arquivo de texto na mesma
     * pasta da aplicação.
     */
    public void salvarDados()
    {
        final String dados = fr.getRawData();
        
        Thread t;
        
        t = new Thread(new Runnable(){ 
            @Override
            public void run()
            {
                try
                {
                    GPDAO.salvar(dados);
                    Util.ExibirMensagem(EnumTipoMensagem.Informacao,"Informações salvas com êxito.");
                }
                catch(Exception erro)
                {
                    Util.ExibirMensagem(EnumTipoMensagem.Erro,"Erro ao salvar o arquivo.");
                }
            }
        });
        
        t.start();
    }
    
    /**
     * Conta as linhas de um arquivo de texto.
     * @param file O arquivo de texto.
     * @return A quantidade de linhas.
     * @throws LeituraArquivoException Caso ocorra algum erro na leitura do arquivo.
     */
    private static int contarLinhas(String file) throws LeituraArquivoException
    {
        try
        {
            FileReader f = new FileReader(file);
            BufferedReader bf = new BufferedReader(f);

            int cont = 0;

            while(bf.readLine() != null)
                cont ++;

            return cont;

        }
        catch(Exception ex)
        {
            throw new LeituraArquivoException();
        }
    }
    
    /**
     * Método tratador do evento de chegada de objeto GPSentence.
     * A sentença pode ser enviada para a classe InfoSatelite, que
     * depois irá notificar o form, ou pode ser enviada diretamente
     * para o form de dados.
     * @param obj O objeto GPSentence.
     */
    @Override
    public void dadosRecebidos(GPSentence obj) 
    {
        if(obj instanceof GPGSV)
        {
            InfoSatelite.addSats((GPGSV)obj);
        }
        else if(obj instanceof GPGSA)
        {
            InfoSatelite.addSatFixInfo((GPGSA)obj);
        }
        else
        {
            fr.atualizarForm(obj);
        }
    }
    
    /**
     * Envia os dados NMEA brutos recebidos para o TextArea do form.
     * @param obj A string NMEA.
     */
    @Override
    public void stringRecebida(String obj)
    {
        fr.atualizarRaw(obj);
    }
    
    public static void main(String[] args)
    {   
        mainClass = new MainClass();
    }
    
    /**
     * Realiza tentativa de login.
     * @param login O login do usuário.
     * @param senha A senha do usuário.
     * @return True se o login teve sucesso.
     */
    public boolean logar(String login, String senha)
    {
        try 
        {
            if(ControleAcesso.isValid(login, senha))
            {
                init();
                return true;
            }
        } 
        catch (DadosUsuarioInvalidosException ex) 
        {
            return false;
        }
        
        return false;
    }
}
