/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package NetworkController;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author lillo
 */
public class TcpClient implements Runnable
{
    private final int port;
    private final String ip;
    
    private final PrintWriter out;
    private final DataInputStream in;
    
    private final Socket socket;
    
    private final String COD_FECHA_CONEXAO = "fechaTCP";
    
    private boolean podeFechar = false;
    
    private final ArrayList<IReciveInternal> listRecive = new ArrayList<>();
    
    private final int tamanho = 2416404;
    public TcpClient(String ip, int port) throws IOException
    {
        this.ip = ip;
        this.port = port;
        
        socket = new Socket(ip,port);
        
        out = new PrintWriter(socket.getOutputStream(),true);
        in = new DataInputStream(socket.getInputStream());
    }

    @Override
    public void run()
    {
        int lenght = 0;
        byte[] dados;
        ArrayList<Byte> listByte = new ArrayList<>();
        while(!podeFechar)
        {
            try
            {
                dados = null;
                /* byte[] by = new byte[1];
                for(int len; (len = in.read(by)) > 0;)
                listByte.add(by[0]);
                dados = new byte[listByte.size()];
                for(int i = 0; i < listByte.size();i++)
                dados[i] = listByte.get(i);*/
                lenght = in.available();
                if(lenght <= 0 )
                {
                    if(!listByte.isEmpty())
                    {
                        dados = new byte[listByte.size()];
                        for(int i = 0; i < listByte.size();i++)
                            dados[i] = listByte.get(i);
                        invokeOnReceive(dados);
                        listByte.clear();
                    }
                    continue;
                }
                
                dados =  new byte[lenght];
                in.readFully(dados,0,lenght);
                for(int  i = 0;i < dados.length;i++)
                    listByte.add(dados[i]);
            }
            catch (IOException ex)
            {
                Logger.getLogger(TcpClient.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        
        encerraConexao();
    }
    
    public void fechaConexao()
    {
        this.podeFechar = true;
    }
    
    private void encerraConexao()
    {
        if(socket != null && !socket.isConnected())
        {
            try
            {
                socket.close();
            }
            catch (IOException ex)
            {
                Logger.getLogger(TcpClient.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    private void invokeOnReceive(byte[] dados)
    {
        for(IReciveInternal i:listRecive)
            i.onReceive(dados, port);
    }
    
    public void addListen(IReciveInternal reciver)
    {
        listRecive.add(reciver);
    }
    
    public int getPort()
    {
        return port;
    }
}
