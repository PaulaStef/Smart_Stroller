import javax.swing.*;
import java.awt.*;
import java.util.List;

public class MainView {

    private JFrame frame;
    private JButton refresh;
    private JTable table;
    Permission permission = new Permission(this);

    public MainView() {
        initialize();
    }

    public void initialize(){
        List<Product> result = Permission.findAll();
        //System.out.println(result.size());
        ProductTable j = new ProductTable();
        this.setTable(j.JTable(result));
        frame = new JFrame();
        frame.setSize(1000, 700);
        frame.setBounds(0, 0, 1000, 700);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        table.setBackground(SystemColor.pink);
        frame.add(this.getTable());
        frame.setVisible(true);
        refresh = new JButton("Refresh");
        refresh.setBounds(300,380,100,50);
        refresh.setFont(new Font("Ariel",Font.PLAIN,17));
        refresh.setBackground(Color.white);
        refresh.addActionListener(permission);
        frame.add(refresh);
        frame.add(table);
    }

    public JButton getRefresh() {
        return refresh;
    }

    public JTable getTable() {
        return table;
    }

    public void setTable(JTable table) {
        this.table = table;
    }

    public JFrame getFrame() {
        return frame;
    }
}
