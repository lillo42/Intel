/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

import com.sun.javafx.geom.Vec3d;
import javax.swing.JOptionPane;

/**
 * Classe com métodos auxiliares.
 * 
 * @author Vitor
 */
public class Util {
    
    /**
     * Indica o tipo de mensagem a ser exibida, e define um título e um ícone para o Dialog.
     */
    public enum EnumTipoMensagem {Informacao, Erro};
    
    /**
     * Indica o estado atual do form de dados, se está conectado ou não.
     */
    public enum EnumEstadoForm {Ocioso, Conectado};
    
    /**
     * Indica o tipo de conexão que será criada.
     */
    public enum EnumTipoConexao {Serial, Arquivo, TCP};
    
    /**
     * Exibe um Message Dialog na tela com ícone e títulos definidos de acordo
     * com o enumerador tipo.
     * @param tipo O enumerador que define o tipo da mensagem.
     * @param msg O conteúdo da mensagem.
     */
    public static void ExibirMensagem(EnumTipoMensagem tipo, String msg)
    {
        if(tipo == EnumTipoMensagem.Erro)
            JOptionPane.showMessageDialog(null, msg, "Erro", JOptionPane.ERROR_MESSAGE);
        else if(tipo == EnumTipoMensagem.Informacao)
            JOptionPane.showMessageDialog(null, msg, "Informação", JOptionPane.INFORMATION_MESSAGE);
    }
    
    /**
     * Converte nós para KMh.
     * @param knots Velocidade em nós.
     * @return A velocidade em KMh.
     */
    public static double knotsToKMh(double knots)
    {
        return knots * 1.852;
    }
    
    /**
     * Converte uma velocidade em KMh para nós.
     * @param kmh Velocidade em KMh.
     * @return A velocidade em nós.
     */
    public static double kmhToKnots(double kmh)
    {
        return kmh / 1.852;
    }
    
    /**
     * Mapeia um valor linearmente de uma escala para outra.
     * @param x O valor que será mapeado
     * @param inMin O menor valor possível de X
     * @param inMax O maior valor possível de X
     * @param outMin O valor retornado para o menor valor de X
     * @param outMax O valor retornado para o maior valor de X
     * @return O valor mapeado para a escala de saída definida pelos parâmetros.
     */
    public static float map(float x, float inMin, float inMax, float outMin, float outMax)
    {
            return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }
    
    /**
     * Converte um valor de latitude NMEA para um valor de latitude em graus.
     * @param nmeaLatitude O valor de latitude.
     * @param hemisferio O hemisfério da latitude.
     * @return A latitude em graus de 90 a -90.
     */
    public static double getLatitudeDegrees(String nmeaLatitude, char hemisferio)
    {
        double graus = Double.parseDouble(nmeaLatitude.substring(0,2));
        double minutos = Double.parseDouble(nmeaLatitude.substring(2,nmeaLatitude.length())) / 60;
        
        if(hemisferio == 'S')
            return -1 * graus + minutos;
        else
            return graus + minutos;
    }
    
    /**
     * Converte um valor de longitude NMEA para um valor de longitude em graus.
     * @param nmeaLongitude  O valor de longitude.
     * @param hemisferio O hemisfério da latitude.
     * @return A longitude em graus, de 180 a -180.
     */
    public static double getLongitudeDegrees(String nmeaLongitude, char hemisferio)
    {
        double graus = Double.parseDouble(nmeaLongitude.substring(0,3));
        double minutos = Double.parseDouble(nmeaLongitude.substring(3,nmeaLongitude.length())) / 60;
        
        if(hemisferio == 'W')
            return -1 * graus + minutos;
        else
            return graus + minutos;
    }
    
    /**
     * Retorna a distância entre dois pontos no espaço 3D.
     * @param x1 Coordenada X1
     * @param y1 Coordenada Y1
     * @param z1 Coordenada Z1
     * @param x2 Coordenada X2
     * @param y2 Coordenada Y2
     * @param z2 Coordenada Z2
     * @return A distância entre os dois pointos.
     */
    public static double distancia(double x1, double y1, double z1,
                                    double x2, double y2, double z2)
    {
        double x = x2 - x1;
        double y = y2 - y1;
        double z = z2 - z1;
        
        return Math.sqrt(x*x +y*y + z*z);
    }
    
    /**
     * Retorna a distância entre dois pontos no espaço 3D.
     * @param v1 O primeiro ponto
     * @param v2 O segundo ponto
     * @return A distância entre ambos.
     */
    public static double distancia(Vec3d v1, Vec3d v2)
    {
        return distancia(v1.x,v1.y,v1.z,v2.x,v2.y,v2.z);
        
    }
}
