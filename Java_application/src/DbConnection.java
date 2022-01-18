import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class DbConnection {
    private String jdbcURL = "jdbc:postgresql://localhost:5432/shopdb";
    private String username = "postgres";
    private String password = "root1234";
    private static DbConnection singleInstance = new DbConnection();

    public Connection connect() {
        Connection connection = null;
        {
            try {
                connection = DriverManager.getConnection(jdbcURL, username, password);
                System.out.println("Connected to PostgreSQL server.");
            } catch (SQLException throwables) {
                System.out.println("Error in connecting to PostgreSQL server.");
                throwables.printStackTrace();
            }
        }
        return connection;
    }

    public void updateQuantity(String quantity, String name) {
        String query = "UPDATE products\n" +
                "SET quantity = quantity - " + quantity +"\n" +
                "WHERE product_name = '" + name + "'";

        try {
            PreparedStatement statement = getConnection().prepareStatement(query);
            int rows = statement.executeUpdate();
            if (rows > 0) {
                System.out.println("Product quantity modified!");
            }else{
                System.out.println("UPDATE FAILED");
                System.out.println("Query statement : " + query);
            }

        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
    }


    public static Connection getConnection() {
        return singleInstance.connect();
    }
}

