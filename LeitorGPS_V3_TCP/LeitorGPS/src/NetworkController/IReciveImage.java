/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package NetworkController;

import java.awt.image.BufferedImage;

/**
 *
 * @author lillo
 */
public interface IReciveImage
{
    //void onReciveImage(BufferedImage image); 
    void onReciveImage(String image, int port); 
}
