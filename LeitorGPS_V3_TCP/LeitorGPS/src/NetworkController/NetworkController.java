/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package NetworkController;

import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 *
 * @author lillo
 */
public class NetworkController implements IReciveInternal
{

    private NetworkController()
    {
        listTcpClient = new ArrayList<>();
        dictTcpClient = new Hashtable();
    }
    private static NetworkController instance;
    public static NetworkController getInstace()    
    {
        if(instance == null)
            instance = new NetworkController();
        return instance;
    }
    
    private final ArrayList<TcpClient> listTcpClient;
    private final Dictionary dictTcpClient;
    
    @Override
    public void onReceive(byte[] dados, int port)
    {
        //String image = convertByteToBufferImage(dados);
        //IReciveImage recive = (IReciveImage) dictTcpClient.get(port);
        //recive.onReciveImage(image,port);
    }
    
    private BufferedImage convertByteToBufferImage(byte[] dados)
    {
       ByteArrayInputStream bais = new ByteArrayInputStream(dados);
        try
        {
            return ImageIO.read(bais);
        } 
        catch (Exception e) 
        {
            return null;
        }
    }
    
    public boolean createTcpClient(String ip,int port,IReciveImage recive )
    {
        boolean retorno = true;
        try
        {
            TcpClient t = new TcpClient(ip,port);
            t.addListen(this);
            listTcpClient.add(t);
            dictTcpClient.put(port, recive);
            Thread thread = new Thread(t);
            thread.start();
        }
        catch (IOException ex)
        {
            Logger.getLogger(NetworkController.class.getName()).log(Level.SEVERE, null, ex);
            retorno = false;
        }
        return retorno;
    }
}
