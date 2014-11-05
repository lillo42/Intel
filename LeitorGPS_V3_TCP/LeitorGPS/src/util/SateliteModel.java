/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

import java.util.ArrayList;
import javax.swing.table.AbstractTableModel;
import leitorGPS.InfoSatelite;

/**
 * Modelo de tabela usado no form de dados para listar
 * informações sobre os satélites.
 * 
 * @author Vitor
 */
public class SateliteModel extends AbstractTableModel{

    private ArrayList<InfoSatelite> linhas;
    
    private String[] colunas = new String[] {"PRN", "Azimute", "Elevação", "SNR", "Usado"};
    
    public SateliteModel()
    {
        linhas = new ArrayList<>();
    }
    
    public SateliteModel(ArrayList<InfoSatelite> sats)
    {
        linhas = new ArrayList<>(sats);
    }
    
    /**
     * Retorna o nome da coluna para o índice desejado.
     * @param col O índice da coluna.
     * @return A string com o nome.
     */
    @Override
    public String getColumnName(int col)
    {
        return colunas[col];
    }
    
    /**
     * Retorna o número de linhas.
     * @return O número de linhas.
     */
    @Override
    public int getRowCount() {
        return linhas.size();
    }

    /**
     * Retorna o número de colunas.
     * @return O número de colunas.
     */
    @Override
    public int getColumnCount() {
        return colunas.length;
    }

    /**
     * Método usado pelo Java para preencher uma célula da tabela.
     * @param rowIndex A linha da célula.
     * @param columnIndex A coluna da célula.
     * @return O valor da célula.
     */
    @Override
    public Object getValueAt(int rowIndex, int columnIndex) 
    {
        InfoSatelite info = linhas.get(rowIndex);
        
        switch(columnIndex)
        {
            case 0:
                return info.getPrn();
            case 1:
                return info.getAzimute();
            case 2:
                return info.getElevacao();
            case 3:
                return info.getSnr();
            case 4:
                return info.isUsadoNoFix()? "Sim" : "Não";
            default:
                throw new IndexOutOfBoundsException("Índice da coluna fora dos limites");
        }
        
    }
    
    /**
     * Método usado pelo Java para definir se usuário pode alterar célula.
     * @param rowIndex A linha dá célula
     * @param columnIndex A coluna da célula
     * @return False sempre. O usuário nunca pode editar nenhuma célula dessa tabela.
     */
    @Override
    public boolean isCellEditable(int rowIndex, int columnIndex)
    {
        return false;
    }
    
    /**
     * Atualiza a tabela com novos dados de satélite
     * e dispara o evento de alteração de dados para que o Java
     * redesenhe a tabela.
     * @param sats A nova lista de satélites.
     */
    public void setDados(ArrayList<InfoSatelite> sats)
    {
        linhas = new ArrayList<>(sats);
        
        fireTableDataChanged();
    }
}
