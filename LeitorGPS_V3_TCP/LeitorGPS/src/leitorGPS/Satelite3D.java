/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import com.interactivemesh.jfx.importer.obj.ObjModelImporter;
import com.sun.javafx.geom.Vec3d;
import java.net.URL;
import javafx.scene.Group;
import javafx.scene.image.Image;
import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Box;
import javafx.scene.shape.Shape3D;
import javafx.scene.shape.Sphere;
import javafx.scene.transform.Rotate;
import util.Util;

/**
 * Classe usada para representar um satélite 3D.
 * Isso inclui o satélite em si, a linha amarela e os objetos
 * transformadores de rotação do satélite.
 * 
 * @author Vitor
 */
public class Satelite3D extends Group
{   
    private Shape3D satObj;
    private Box eixoObj;
    private Rotate[] rotates = new Rotate[3];
    
    static PhongMaterial lineMat = new PhongMaterial(Color.YELLOW);
    PhongMaterial mat;
    
    /**
     * Retorna o objeto satélite propriamente dito.
     * @return A malha 3D do satélite.
     */
    public Shape3D getSatObj()
    {
        return satObj;
    }
    
    /**
     * Cria um satélite e adiciona ele ao grupo.
     * @param gpsGroup O grupo que receberá o satélite.
     */
    public Satelite3D(Group gpsGroup)
    {
        ObjModelImporter objImport = new ObjModelImporter();
        
        try
        {
            URL urlObj = this.getClass().getResource("/resources/satelite.obj");
            objImport.read(urlObj);
            satObj = objImport.getImport()[0];
            
            //satObj = new Box(2,2,2);
            
            Image im = new Image(getClass().getResourceAsStream("/resources/satMap.jpg"),512,512,true,true);
            mat = new PhongMaterial(Color.WHITE,im,im,null,null);
        }
        catch(Exception erro)
        {
            satObj = new Sphere(1, 5);  
        }
        finally
        {
            objImport.close();
        }
        
        //satObj.setScaleX(10);
        //satObj.setScaleY(10);
        //satObj.setScaleZ(10);
        
        eixoObj = new Box(0.1,1,0.1);
        eixoObj.setTranslateY(0.5f);
        //eixoObj = new Box(0.1,InfoSatelite.RAIO_SAT3D,0.1);
        //eixoObj.setTranslateY(InfoSatelite.RAIO_SAT3D / 2);
        satObj.setTranslateY(InfoSatelite.RAIO_SAT3D);
        
        satObj.setMaterial(mat);
        eixoObj.setMaterial(lineMat);
        
        try
        {
            Image im = new Image(getClass().getResourceAsStream("/resources/linha.jpg"));
            lineMat.setDiffuseMap(im);
            lineMat.setSelfIlluminationMap(im);
        }
        catch(Exception erro)
        {
            
        }

        rotates[0] = new Rotate(0,Rotate.X_AXIS);
        rotates[1] = new Rotate(0,Rotate.Y_AXIS);
        rotates[2] = new Rotate(0,Rotate.Z_AXIS);
        
        getTransforms().addAll(rotates[0],rotates[1],rotates[2]);
        //setAzimute(180);
        //setElevacao(45);
        
        getChildren().addAll(satObj,eixoObj);
        addSat(gpsGroup);
    }
    
    /**
     * Define o azimute do satélite.
     * @param angle O ângulo de 0 a 360.
     */
    public void setAzimute(float angle)
    {
        rotates[0].setAngle(-1 * angle);
    }
    
    /**
     * Define a elevação do satélite.
     * @param angle O ângulo de 0 a 90.
     */
    public void setElevacao(float angle)
    {
        rotates[2].setAngle(-angle);
    }
    
    /**
     * Define se o satélite é usado no cálculo de posição ou não.
     * Se for usado, a linha amarela é desenhada.
     * @param usado True se for usado.
     */
    public void setUsadoNoFix(boolean usado)
    {
        eixoObj.setVisible(usado);
    }
    
    /**
     * Os satélites são apenas criados, conforme necessário.
     * Quando não são mais desejados, são apenas escondidos para
     * reduzir overhead.
     * @param visivel True se for visível.
     */
    public void setSatVisivel(boolean visivel)
    {
        this.setVisible(visivel);
    }
    
    /**
     * Adiciona satélite no grupo em questão e o
     * posiciona adequadamente.
     * @param gpsGroup O grupo que irá receber o satélite.
     */ 
    private void addSat(Group gpsGroup)
    {
        gpsGroup.getChildren().add(this);
        //setTranslateX(gpsGroup.get);
        setTranslateY(gpsGroup.getTranslateY());
        setTranslateZ(gpsGroup.getTranslateZ());
    }
    
    /**
     * Devolve a posição do satélite.
     * @return Um vetor 3D com a posição X, Y e Z do satélite.
     */
    public Vec3d getPositSat()
    {
        Vec3d pos = new Vec3d();
        pos.x = satObj.getLocalToSceneTransform().getTx();
        pos.y = satObj.getLocalToSceneTransform().getTy();
        pos.z = satObj.getLocalToSceneTransform().getTz();
        return pos;
    }
    
    /**
     * Usado para posicionar o satélite na órbita correta.
     * @param dist A distância do deslocamento.
     * @param positGps A posição marcador do GPS.
     */
    public void deslocarPositSat(double dist, Vec3d positGps)
    {
        satObj.setTranslateY(satObj.getTranslateY() + dist);
        dist = Util.distancia(positGps,getPositSat());;
        eixoObj.setScaleY(dist);
        eixoObj.setTranslateY(dist / 2);
    }
}
