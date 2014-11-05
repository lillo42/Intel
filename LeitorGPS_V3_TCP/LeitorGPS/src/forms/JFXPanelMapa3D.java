/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package forms;

import com.interactivemesh.jfx.importer.obj.ObjModelImporter;
import com.sun.javafx.geom.Vec3d;
import java.net.URL;
import java.util.ArrayList;
import javafx.application.Platform;
import javafx.embed.swing.JFXPanel;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.PerspectiveCamera;
import javafx.scene.Scene;
import javafx.scene.SceneAntialiasing;
import javafx.scene.image.Image;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Box;
import javafx.scene.shape.MeshView;
import javafx.scene.shape.Sphere;
import javafx.scene.transform.Rotate;
import leitorGPS.InfoSatelite;
import leitorGPS.Satelite3D;
import util.Util;
import util.Xform;
/**
 * Classe usada para renderizar um painel com visualização 3D dos dados NMEA
 * obtidos relativos à localização e satélites.
 * 
 * @author Vitor
 */

public class JFXPanelMapa3D extends JFXPanel{
    
    private Group root;
    private PerspectiveCamera camera;
    private Scene scene;
    private Group axisGroup;
    
    final Xform cameraXform = new Xform();
    final Xform cameraXform2 = new Xform();
    final Xform cameraXform3 = new Xform();
    
    private Group grpPlaneta;
    private MeshView objTerra;
    private ArrayList<Satelite3D> sats3d = new ArrayList<>();
    
    private Group grpGps;
    private Group grpSatelites;
    
    private Rotate[] grpGpsRotates = new Rotate[3];
    private MeshView gpsObj;
    
    double mousePosX;
    double mousePosY;
    double mouseOldX;
    double mouseOldY;
    double mouseDeltaX;
    double mouseDeltaY;
    
    /**
     * Quando se integra um painel JavaFX em uma aplicação Swing
     * é preciso executar o painel em uma Thread específica.
     * O método init cria essa Thread e inicializa o painel 3D.
     */
    public void init()
    {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                configScene();
            }
        });
        
    }
    
    /**
     * Método usado para debugar a posição dos satélites.
     * Desenha uma esfera branca ao redor da Terra indicando a altitude
     * aproximada na qual os satélites devem ser posicionados.
     */
    private void criarOrbitaSatelite()
    {
        Sphere p = new Sphere(InfoSatelite.RAIO_SAT3D);
        PhongMaterial m = new PhongMaterial(new Color(1,1,1,0.3));
        p.setMaterial(m);
        p.setOpacity(0.2);
        root.getChildren().add(p);
    }
    
    /**
     * Configura informações básicas sobre a cena 3D
     * e carrega modelos 3D e suas texturas na cena.
     */
    private void configScene()
    {
        root = new Group();
        grpPlaneta = new Group();
        camera = new PerspectiveCamera(true);
        
        scene = new Scene(root, 1024, 768, true,SceneAntialiasing.BALANCED);
        scene.setFill(javafx.scene.paint.Color.BLACK);
        
        root.getChildren().add(cameraXform);
        cameraXform.getChildren().add(cameraXform2);
        cameraXform2.getChildren().add(cameraXform3);
        cameraXform3.getChildren().add(camera);
        cameraXform3.setRotateZ(180.0);

        camera.setNearClip(0.1);
        camera.setFarClip(100000.0);
        camera.setTranslateZ(-450);
        cameraXform.ry.setAngle(320.0);
        cameraXform.rx.setAngle(40);
        scene.setCamera(camera);
        
        initGPS();
        carregarTerra();
        carregarEspaco();
        
        grpPlaneta.setTranslateX(0);
        grpPlaneta.setTranslateY(0);
        grpPlaneta.getChildren().add(objTerra);
        grpPlaneta.getChildren().add(grpGps);
        
        root.getTransforms().add(new Rotate(180,Rotate.X_AXIS));
        root.getChildren().add(grpPlaneta);
        
        for(int i = 0; i < 10; i++)
        {
            Satelite3D sat = new Satelite3D(grpSatelites);
            sat.setSatVisivel(false);
            sats3d.add(sat);
        }
        
        //buildAxes();
        
        camera.setTranslateZ(-80);
        
        handleMouse(scene);
        setScene(scene);
        //criarOrbitaSatelite();
    }
    
    /**
     * Carrega o modelo e textura de estrelas do espaço.
     */
    private void carregarEspaco()
    {
        ObjModelImporter objImport = new ObjModelImporter();
        
        try
        {
            URL urlObj = this.getClass().getResource("/resources/espaco.obj");
            objImport.read(urlObj);
            
            MeshView espaco = objImport.getImport()[0];
            espaco.setScaleX(200);
            espaco.setScaleY(200);
            espaco.setScaleZ(200);

            Image im = new Image(getClass().getResourceAsStream("/resources/stars.jpg"));
            PhongMaterial mat = new PhongMaterial(Color.WHITE,im,im,null,null);
            
            espaco.setMaterial(mat);
            grpPlaneta.getChildren().add(espaco);
        }
        catch(Exception erro)
        {
            util.Util.ExibirMensagem(Util.EnumTipoMensagem.Erro, "Erro ao importar modelo da Terra.");
        }
        finally
        {
            objImport.close();
        }
    }
    
    /**
     * Carrega o modelo da Terra e a textura de mapa-múndi.
     */
    private void carregarTerra()
    {
        ObjModelImporter objImport = new ObjModelImporter();
        
        try
        {
            URL urlObj = this.getClass().getResource("/resources/terra.obj");
            objImport.read(urlObj);

            objTerra = objImport.getImport()[0];
            objTerra.setScaleX(InfoSatelite.RAIO_TERRA_3D);
            objTerra.setScaleY(InfoSatelite.RAIO_TERRA_3D);
            objTerra.setScaleZ(InfoSatelite.RAIO_TERRA_3D);

            Image im = new Image(getClass().getResourceAsStream("/resources/terra.jpg"));
            PhongMaterial mat = new PhongMaterial(Color.WHITE,im,null,null,null);
            
            objTerra.setMaterial(mat);
        }
        catch(Exception erro)
        {
            util.Util.ExibirMensagem(Util.EnumTipoMensagem.Erro, "Erro ao importar modelo da Terra.");
        }
        finally
        {
            objImport.close();
        }
    }
    
    /**
     * Carrega e posiciona o marcador de GPS no planeta Terra.
     */
    private void initGPS()
    {
        grpGps = new Group();
        grpSatelites = new Group();
        
        ObjModelImporter objImport = new ObjModelImporter();
        
        try
        {
            URL urlObj = this.getClass().getResource("/resources/marcador.obj");
            objImport.read(urlObj);

            gpsObj = objImport.getImport()[0];
        }
        catch(Exception erro)
        {
            util.Util.ExibirMensagem(Util.EnumTipoMensagem.Erro, "Erro ao importar marcador de posição.");
        }
        finally
        {
            objImport.close();
        }
        
        gpsObj.setTranslateX(InfoSatelite.RAIO_TERRA_3D);
        grpSatelites.setTranslateX(InfoSatelite.RAIO_TERRA_3D);
        //grpSatelites.getTransforms().add(new Rotate(-90,Rotate.X_AXIS));
        
        grpGps.getChildren().add(gpsObj);
        grpGps.getChildren().add(grpSatelites);
        
        grpGpsRotates[0] = new Rotate(0, Rotate.X_AXIS);
        grpGpsRotates[1] = new Rotate(0, Rotate.Y_AXIS);
        grpGpsRotates[2] = new Rotate(0, Rotate.Z_AXIS);
        
        grpGps.getTransforms().add(grpGpsRotates[0]);
        grpGps.getTransforms().add(grpGpsRotates[1]);
        grpGps.getTransforms().add(grpGpsRotates[2]);
        
        //-43.3744881,172.4662705
        //Nova Zelândia
        
        //grpGpsRotates[0].setAngle(45);
        grpGpsRotates[1].setAngle(-0.010635);
        grpGpsRotates[2].setAngle(51.478885);
        //grpGps.setRotate(0, 90, 0);
        //grpGps.setRotateY(89.99);
        //grpGps.setRotateZ(90.11);
        
        PhongMaterial mat = new PhongMaterial(Color.RED);
        gpsObj.setMaterial(mat);
        
        //sats3d.add(new Satelite3D(grpSatelites));
    }
    
    /**
     * Método usado para indicar que é necessário atualizar as posições
     * dos satélites.
     * Esse método consulta a classe InfoSatelite para obter as informações necessárias sobre os mesmos.
     * @param mostrarFix Indica se as linhas amarelas serão exibidas
     */
    public void updateSats(final boolean mostrarFix)
    {
        Platform.runLater(new Runnable() {
            @Override
            public void run() 
            {
                int satCount = InfoSatelite.getQtdTotalSats();
                int listCount = sats3d.size();
                int diferenca = satCount - listCount;

                //instanciar satelites
                if(diferenca > 0)
                {
                    for(int x = 0; x < diferenca; x++)
                    {
                        sats3d.add(new Satelite3D(grpSatelites));
                    }
                }
                else if(diferenca < 0)
                {
                    diferenca *= -1;
                    //esconder satelites
                    for(int x = satCount; x < listCount; x++)
                    {
                        sats3d.get(x).setSatVisivel(false);
                    }
                }

                Vec3d vOrigin = new Vec3d(0,0,0);
                
                for(int x = 0; x < satCount; x++)
                {
                    InfoSatelite info = InfoSatelite.getSatAt(x);
                    Satelite3D sat = sats3d.get(x);
                    
                    float elevacao;
                    float azimute;
                    
                    try 
                    {
                        elevacao = (float)(Float.parseFloat(info.getElevacao()));
                        azimute = (float)(Float.parseFloat(info.getAzimute()));
                    }
                    catch(Exception erro)
                    { 
                        sat.setSatVisivel(false);
                        continue;
                    }
                    
                    sat.setSatVisivel(true);
                    sat.setAzimute(azimute);
                    sat.setElevacao(elevacao);
                    sat.setUsadoNoFix(mostrarFix && info.isUsadoNoFix());
                    
                    //usado para posicionar os satelites na sua orbita
                    double dist = Util.distancia(sat.getPositSat(),vOrigin);
                    double deslocamento = InfoSatelite.RAIO_SAT3D - dist;
                    
                    sat.deslocarPositSat(deslocamento,getPositGps());
                }
            }
        });
    }
    
    /**
     * Retorna a posição do marcador GPS.
     * @return Um vetor3D com a posição X,Y e Z do marcador.
     */
    public Vec3d getPositGps()
    {
        Vec3d pos = new Vec3d();
        pos.x = gpsObj.getLocalToSceneTransform().getTx();
        pos.y = gpsObj.getLocalToSceneTransform().getTy();
        pos.z = gpsObj.getLocalToSceneTransform().getTz();
        return pos;
    }
    
    /**
     * Posiciona o marcador na longitude desejada.
     * @param val A longitude desejada.
     */
    public void setLongitude(final double val)
    {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                grpGpsRotates[1].setAngle(val);
            }
        });
    }
    
    /**
     * Posiciona o marcador na latitude desejada
     * @param val A latitude desejada
     */
    public void setLatitude(final double val)
    {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                grpGpsRotates[2].setAngle(val);
            }
        });
    }
    
    /**
     * Método usado para debug.
     * Apenas renderiza três barras indicam os três eixos 3D.
     */
    private void buildAxes() {
        final PhongMaterial redMaterial = new PhongMaterial();
        redMaterial.setDiffuseColor(Color.DARKRED);
        redMaterial.setSpecularColor(Color.RED);
 
        final PhongMaterial greenMaterial = new PhongMaterial();
        greenMaterial.setDiffuseColor(Color.DARKGREEN);
        greenMaterial.setSpecularColor(Color.GREEN);
 
        final PhongMaterial blueMaterial = new PhongMaterial();
        blueMaterial.setDiffuseColor(Color.DARKBLUE);
        blueMaterial.setSpecularColor(Color.BLUE);
        
        final Box xAxis = new Box(240.0, 1, 1);
        final Box yAxis = new Box(1, 240.0, 1);
        final Box zAxis = new Box(1, 1, 240.0);
        
        xAxis.setMaterial(redMaterial);
        yAxis.setMaterial(greenMaterial);
        zAxis.setMaterial(blueMaterial);
        
        axisGroup = new Group();
        axisGroup.getChildren().addAll(xAxis, yAxis, zAxis);
        root.getChildren().addAll(axisGroup);
    }
    
    /**
     * Anexa métodos tratadores de eventos do mouse à cena.
     * @param scene A cena que irá disparar os eventos.
     */
    private void handleMouse(Scene scene) {
        scene.setOnMousePressed(new EventHandler<MouseEvent>() {
            @Override public void handle(MouseEvent me) {
                mousePosX = me.getSceneX();
                mousePosY = me.getSceneY();
                mouseOldX = me.getSceneX();
                mouseOldY = me.getSceneY();
            }
        });
        scene.setOnMouseDragged(new EventHandler<MouseEvent>() {
            @Override public void handle(MouseEvent me) {
                mouseOldX = mousePosX;
                mouseOldY = mousePosY;
                mousePosX = me.getSceneX();
                mousePosY = me.getSceneY();
                mouseDeltaX = (mousePosX - mouseOldX); 
                mouseDeltaY = (mousePosY - mouseOldY); 
                
                double modifier = 1.0;
                double modifierFactor = 0.1;
                
                if (me.isControlDown()) {
                    modifier = 0.1;
                } 
                if (me.isShiftDown()) {
                    modifier = 10.0;
                }     
                if (me.isPrimaryButtonDown()) {
                    cameraXform.ry.setAngle(cameraXform.ry.getAngle() - mouseDeltaX*modifierFactor*modifier*2.0);  // +
                    cameraXform.rx.setAngle(cameraXform.rx.getAngle() + mouseDeltaY*modifierFactor*modifier*2.0);  // -
                }
                else if (me.isSecondaryButtonDown()) {
                    double z = camera.getTranslateZ();
                    double newZ = z + mouseDeltaX*modifierFactor*modifier;
                    camera.setTranslateZ(newZ);
                }
                else if (me.isMiddleButtonDown()) {
                    cameraXform2.t.setX(cameraXform2.t.getX() + mouseDeltaX*modifierFactor*modifier*0.3);  // -
                    cameraXform2.t.setY(cameraXform2.t.getY() + mouseDeltaY*modifierFactor*modifier*0.3);  // -
                }
            }
        });
    }
}
