import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyDescriptor;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Permission implements ActionListener {
    MainView page ;

    public Permission(MainView page) {
        this.page = page;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == page.getRefresh()){
            ProductTable j = new ProductTable();
            List<Product> result = findAll();
            System.out.println(result.size());
            page.getFrame().setVisible(false);
            page.getFrame().dispose();
            page.initialize();
        }
    }

    static public List<Product> findAll() {
        List<Product> list = new ArrayList<>();
        StringBuilder findAll = new StringBuilder();
        findAll.append("SELECT * FROM products");
        ResultSet result = null;
        try(
            Connection connection = DbConnection.getConnection();
            PreparedStatement preparedStatement = connection.prepareStatement(String.valueOf(findAll))){
            result = preparedStatement.executeQuery();
            while (result.next()){
                Product product = new Product();
                product.setId_product(result.getInt(1));
                product.setProduct_name(result.getString(2));
                product.setQuantity(result.getInt(3));
                product.setPrice(result.getInt(4));
                list.add(product);
            }
            return list;
        }catch ( SQLException e){
            e.printStackTrace();
        } finally {
            if(result != null){
                System.out.println("Result is not null");
            }
        }
        return Collections.emptyList();
    }

}
