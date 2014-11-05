/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

import javafx.scene.Group;
import javafx.scene.transform.Rotate;
import javafx.scene.transform.Scale;
import javafx.scene.transform.Translate;

/**
 * Classe para facilitar a manipulação de certos
 * objetos no espaço 3D.
 * 
 * @author Oracle
 */
public class Xform extends Group {

    public enum RotateOrder {
        XYZ, XZY, YXZ, YZX, ZXY, ZYX
    }

    public Translate t  = new Translate(); 
    public Translate p  = new Translate(); 
    public Translate ip = new Translate(); 
    public Rotate rx = new Rotate();
    { rx.setAxis(Rotate.X_AXIS); }
    public Rotate ry = new Rotate();
    { ry.setAxis(Rotate.Y_AXIS); }
    public Rotate rz = new Rotate();
    { rz.setAxis(Rotate.Z_AXIS); }
    public Scale s = new Scale();

    public Xform() { 
        super(); 
        getTransforms().addAll(t, rz, ry, rx, s); 
    }

    public Xform(RotateOrder rotateOrder) { 
        super(); 
        switch (rotateOrder) {
        case XYZ:
            getTransforms().addAll(t, p, rz, ry, rx, s, ip); 
            break;
        case XZY:
            getTransforms().addAll(t, p, ry, rz, rx, s, ip); 
            break;
        case YXZ:
            getTransforms().addAll(t, p, rz, rx, ry, s, ip); 
            break;
        case YZX:
            getTransforms().addAll(t, p, rx, rz, ry, s, ip);  // For Camera
            break;
        case ZXY:
            getTransforms().addAll(t, p, ry, rx, rz, s, ip); 
            break;
        case ZYX:
            getTransforms().addAll(t, p, rx, ry, rz, s, ip); 
            break;
        }
    }

    /**
     * Posiciona um objeto na posição desejada.
     * @param x A coordenada X
     * @param y A coordenada Y
     * @param z A coordenada Z 
     */
    public void setTranslate(double x, double y, double z) {
        t.setX(x);
        t.setY(y);
        t.setZ(z);
    }

    /**
     * Posiciona um objeto na posição desejada.
     * @param x A coordenada X
     * @param y A coordenada Y
     */
    public void setTranslate(double x, double y) {
        t.setX(x);
        t.setY(y);
    }

    /**
     * Define a posição X do objeto.
     * @param x A nova posição no eixo X.
     */
    public void setTx(double x) { t.setX(x); }
    
    /**
     * Define a posição Y do objeto.
     * @param y A nova posição no eixo y.
     */
    public void setTy(double y) { t.setY(y); }
    
    /**
     * Define a posição Z do objeto.
     * @param z A nova posição no eixo Z.
     */
    public void setTz(double z) { t.setZ(z); }

    /**
     * Define a rotação do objeto.
     * @param x A rotação em ângulos no eixo X.
     * @param y A rotação em ângulos no eixo Y.
     * @param z A rotação em ângulos no eixo Z. 
     */
    public void setRotate(double x, double y, double z) {
        rx.setAngle(x);
        ry.setAngle(y);
        rz.setAngle(z);
    }

    /**
     * Define a rotação do objeto no eixo X.
     * @param x A rotação em ângulos no eixo X.
     */
    public void setRotateX(double x) { rx.setAngle(x); }
    
    /**
     * Define a rotação do objeto no eixo Y.
     * @param y A rotação em ângulos no eixo Y.
     */
    public void setRotateY(double y) { ry.setAngle(y); }
    
    /**
     * Define a rotação do objeto no eixo Z.
     * @param z A rotação em ângulos no eixo Z.
     */
    public void setRotateZ(double z) { rz.setAngle(z); }
    
    /**
     * Define a rotação do objeto no eixo X.
     * @param x A rotação em ângulos no eixo X.
     */
    public void setRx(double x) { rx.setAngle(x); }
    
    /**
     * Define a rotação do objeto no eixo Y.
     * @param y A rotação em ângulos no eixo Y.
     */
    public void setRy(double y) { ry.setAngle(y); }
    
    /**
     * Define a rotação do objeto no eixo Z.
     * @param z A rotação em ângulos no eixo Z.
     */
    public void setRz(double z) { rz.setAngle(z); }

    /**
     * Define o coeficiente de escala em todos os eixos.
     * @param scaleFactor O coeficiente de escala.
     */
    public void setScale(double scaleFactor) {
        s.setX(scaleFactor);
        s.setY(scaleFactor);
        s.setZ(scaleFactor);
    }

    /**
     * Define o coeficiente de escala em todos os eixos.
     * @param x O coeficiente para o eixo X.
     * @param y O coeficiente para o eixo Y.
     * @param z O coeficiente para o eixo Z.
     */
    public void setScale(double x, double y, double z) {
        s.setX(x);
        s.setY(y);
        s.setZ(z);
    }

    /**
     * Define a escala X do objeto.
     * @param x A escala do eixo X.
     */
    public void setSx(double x) { s.setX(x); }
    
    /**
     * Define a escala Y do objeto.
     * @param y A escala Y do objeto.
     */
    public void setSy(double y) { s.setY(y); }
    
    /**
     * Define a escala Z do objeto.
     * @param z A escala Z do objeto.
     */
    public void setSz(double z) { s.setZ(z); }

    /**
     * Transfere o pivô de um objeto para outro ponto.
     * @param x A nova coordenada X do pivô.
     * @param y A nova coordenada Y do pivô.
     * @param z A nova coordenada Z do pivô.
     */
    public void setPivot(double x, double y, double z) {
        p.setX(x);
        p.setY(y);
        p.setZ(z);
        ip.setX(-x);
        ip.setY(-y);
        ip.setZ(-z);
    }

    /**
     * Reseta todos objetos transform do objeto.
     */
    public void reset() {
        t.setX(0.0);
        t.setY(0.0);
        t.setZ(0.0);
        rx.setAngle(0.0);
        ry.setAngle(0.0);
        rz.setAngle(0.0);
        s.setX(1.0);
        s.setY(1.0);
        s.setZ(1.0);
        p.setX(0.0);
        p.setY(0.0);
        p.setZ(0.0);
        ip.setX(0.0);
        ip.setY(0.0);
        ip.setZ(0.0);
    }
}