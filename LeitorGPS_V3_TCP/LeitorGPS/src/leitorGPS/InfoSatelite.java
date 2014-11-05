/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package leitorGPS;

import gpSentences.GPGSA;
import gpSentences.GPGSV;
import java.util.ArrayList;
import util.Util;
/**
 * Classe quue mantém uma lista de satélites conhecidos pela aplicação.
 * A classe define algumas constantes usadas nos cálculos de posicionamento 3D dos satélites.
 * A classe também é usada como um struct para informações de um certo satélite.
 * 
 * @author Vitor
 */
public class InfoSatelite {
    
    public static final int RAIO_TERRA_KM = 6371;
    public static final int RAIO_ORBITA_SAT = 20200 + RAIO_TERRA_KM;
    public static final int RAIO_TERRA_3D = 10;
    public static final float RAIO_SAT3D = Util.map(RAIO_ORBITA_SAT, 0, RAIO_TERRA_KM, 0, 10);
    
    private static ArrayList<InfoSatelite> sats = new ArrayList<>();
    private static ArrayList<InfoSatelite> bufferSats = new ArrayList<>();
    private static ArrayList<Integer> satFixInfo = new ArrayList<>();
    private static int qtdTotalSats = 0;
    private static ArrayList<ISatInfoListener> observers = new ArrayList<>();
    
    private String prn;
    private String elevacao;
    private String azimute;
    private String snr;
    private boolean usadoNoFix;

    /**
     * Diz se o satélite é considerado no cálculo de posição no GPS.
     * @return True se for considerado.
     */ 
    public boolean isUsadoNoFix() {
        return usadoNoFix;
    }

    /**
     * Método usado para definir se o satélite é usado no cálculo de posição.
     * @param usadoNoFix True se for usado.
     */ 
    public void setUsadoNoFix(boolean usadoNoFix) {
        this.usadoNoFix = usadoNoFix;
    }
    
    /**
     * Retorna o ID do satélite, chamado de PRN.
     * @return O ID do satélite.
     */
    public String getPrn() {
        return prn;
    }

    /**
     * Define o PRN do satélite.
     * @param prn O PRN do satélite.
     */
    public void setPrn(String prn) {
        this.prn = prn;
    }

    /**
     * Retorna a elevação do satélite, que varia de 0 a 90 graus.
     * @return 90 se o satélite estiver diretamente acima do dispositivo. O número
     * vai diminuindo a medida que o satélite está fica a frente do dispositivo, até chegar em 0.
     */
    public String getElevacao() {
        return elevacao;
    }

    /**
     * Define a elevação do satélite.
     * @param elevacao A elevação do satélite.
     */
    public void setElevacao(String elevacao) {
        this.elevacao = elevacao;
    }

    /**
     * Retorna o azimute do satélite. Azimute é a medida em graus do ângulo formado
     * entre a direção Norte do planeta e a posição do satélite.
     * @return Um valor de 0 a 360.
     */
    public String getAzimute() {
        return azimute;
    }

    /**
     * Define o azimute do satélite.
     * @param azimute O valor do azimute.
     */
    public void setAzimute(String azimute) {
        this.azimute = azimute;
    }

    /**
     * Define a intensidade do sinal do satélite.
     * @return Um valor de 0 a 99. Um sinal razoável tem um valor minimo de 20.
     */
    public String getSnr() {
        return snr;
    }

    /**
     * Define a intensidade de sinal do satélite.
     * @param snr A intensidade de sinal do satélite.
     */
    public void setSnr(String snr) {
        this.snr = snr;
    }
    
    /**
     * Atualiza a lista de satelites usados na aplicação.
     * Esse método cria um buffer quando chega a primeira mensagem do ciclo.
     * Os satélites são adicionados no buffer até que chegue a última mensagem,
     * quando o buffer substitui a lista atual de satélites.
     * @param satsInView Os satélites que serão analisados para atualizar a lista.
     */
    public static void addSats(GPGSV satsInView)
    {
        for(InfoSatelite info : satsInView.getSats())
        {
            for(int i = 0; i < satFixInfo.size(); i++)
            {
                if(Integer.parseInt(info.getPrn()) == satFixInfo.get(i))
                    info.setUsadoNoFix(true);
            }
        }
        
        if(satsInView.getIdMessage() == 1) // primeira mensagem do ciclo
        {
            bufferSats = new ArrayList<>();
            bufferSats.addAll(satsInView.getSats());
        }
        else if(satsInView.getIdMessage() == satsInView.getQtdMessages()) //ultima mensagem do ciclo
        {
            bufferSats.addAll(satsInView.getSats());
            qtdTotalSats = satsInView.getTotalSatsView();
            sats = new ArrayList<>(bufferSats);
            
            for(ISatInfoListener sat : observers)
            {
                sat.updateSatInfo();
            }
        }
        else
        {
            bufferSats.addAll(satsInView.getSats());
        }
    }
    
    /**
     * Atualiza a lista de satélites usados no cálculo
     * de localização.
     * @param fixInfo A lista com os satélites usados.
     */
    public static void addSatFixInfo(GPGSA fixInfo)
    {
        satFixInfo = fixInfo.getSatFix();
    }
    
    /**
     * Retorna a quantidade de satélites usados no cálculo
     * de localização.
     * @return A quantidade de satélites usados.
     */
    public static int getQtdSatsFix()
    {
        return satFixInfo.size();
    }

    /**
     * Retorna a lista com todos os satélites conhecidos pela 
     * aplicação.
     * @return A lista com os satélites.
     */
    public static ArrayList<InfoSatelite> getSats() {
        return sats;
    }

    /**
     * Retorna um satélite específico da lista.
     * @param index O indíce do satélite.
     * @return O satélite desejado.
     */
    public static InfoSatelite getSatAt(int index)
    {
        return sats.get(index);
    }
    
    /**
     * Retorna a quantidade total de satélites conhecidos
     * pela aplicação.
     * Nem todos os satélites podem ser usados no cálculo de posição.
     * @return A quantidade de satélites usados no cálculo.
     */
    public static int getQtdTotalSats() {
        return qtdTotalSats;
    }
    
    /**
     * Adiciona um objeto a lista de ouvintes do evento
     * de atualização da lista de satélites.
     * @param obj Um objeto que implemente a interface ISatInfoListener
     */
    public static void addListener(ISatInfoListener obj)
    {
        observers.add(obj);
    }
}
