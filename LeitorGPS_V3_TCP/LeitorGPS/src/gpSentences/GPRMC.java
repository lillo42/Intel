/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package gpSentences;

import excecoes.SentencaInvalidaException;
import util.Date;
import util.Time;

/**
 * Classe usada para representar uma sentença GPRMC.
 * 
 * @author Vitor
 */
public class GPRMC extends GPSentence{
    
    private Time utcTime;                   //hora UTC 
    private char posicaoValida;             //A se for valido, senao V
    private String latitude, longitude;     //cooredenadas
    private char latiChar, longChar;        //podem ser N ou S e E ou W
    private String speed;                   //velocidade em nós
    private String direcao;                 //angulo de 0 a 360 graus    
    private Date data;
    private String variacaoMagnetica;
    private String direcaoVariacao;           //pode ser W ou E
    
    public GPRMC(String sentenca) throws SentencaInvalidaException
    {
        String[] dados = sentenca.split(",", -2);
        
        try
        {
            utcTime = new Time(dados[0]);
            posicaoValida = dados[1].charAt(0);
            latitude = dados[2];
            latiChar = dados[3].charAt(0);
            longitude = dados[4];
            longChar = dados[5].charAt(0);
            speed = dados[6];
            direcao = dados[7];
            data = new Date(dados[8]);
            variacaoMagnetica = dados[9];
            direcaoVariacao = dados[10];
        }
        catch(Exception e)
        {
            throw new SentencaInvalidaException();
        }
    }

    /**
     * Retorna o horário UTC usado na localização.
     * @return O horário UTC usado na localização.
     */
    public Time getUtcTime() {
        return utcTime;
    }

    /**
     * Indica se a posição na string é válida ou não.
     * @return A se for válido, senão V
     */
    public char getPosicaoValida() {
        return posicaoValida;
    }

    /**
     * Retorna a latitude da posição.
     * @return A latitude da posição.
     */
    public String getLatitude() {
        return latitude;
    }

    /**
     * Retorna a longitude da posição.
     * @return A longitude da posição.
     */
    public String getLongitude() {
        return longitude;
    }

    /**
     * Indica se é latitude norte ou sul.
     * @return N para norte ou S para sul.
     */
    public char getLatiChar() {
        return latiChar;
    }

    /**
     * Indica se a longitude é leste ou oeste.
     * @return E para leste ou W para oeste.
     */
    public char getLongChar() {
        return longChar;
    }

    /**
     * Retorna a velocidade atual.
     * @return A velocidade atual.
     */
    public String getSpeed() {
        return speed;
    }

    /**
     * A direção atual em graus.
     * @return A direção atual.
     */
    public String getDirecao() {
        return direcao;
    }

    /**
     * Retorna a data atual.
     * @return A data da posição.
     */
    public Date getData() {
        return data;
    }

    /**
     * Retorna um coeficiente de variação magnética.
     * @return O coeficiente de variação magnética.
     */
    public String getVariacaoMagnetica() {
        return variacaoMagnetica;
    }

    /**
     * Retorna um coeficiente de variação de direção.
     * @return O coeficiente de variação de direção.
     */
    public String getDirecaoVariacao() {
        return direcaoVariacao;
    }
    
    /**
     * Método auxiliar para retornar o estado da posição.
     * @return Uma string descritiva sobre a posição.
     */
    public String getDescricaoPosicao()
    {
        if(posicaoValida == 'A')
            return "A = Válida";
        else if(posicaoValida == 'V')
            return "V = Inválida";
        else
          return "";
    }
    
}
