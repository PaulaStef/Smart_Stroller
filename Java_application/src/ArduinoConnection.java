import java.sql.PreparedStatement;
import java.util.Scanner;
import com.fazecast.jSerialComm.*;

public class ArduinoConnection {
    private SerialPort ports[];
    private SerialPort port;
    public DbConnection dbConnection;

    public void connect() {
        //first connect the Database
        this.dbConnection = new DbConnection();
        dbConnection.connect();
        //Get the wanted port;
        this.ports = SerialPort.getCommPorts();

        System.out.println("Select a port:");
        int i = 1;
        for (SerialPort portI : ports) {
            System.out.println(i + " " + portI.getSystemPortName());
            i++;
        }
        Scanner s = new Scanner(System.in);
        int chosenPort = s.nextInt();
        this.port = ports[chosenPort - 1];
        //open the right port
        if (this.port.openPort()) {
            System.out.println("Port opened.");
        } else {
            System.out.println("Unable to open the port");
        }
        //wait for data
        this.port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0);

    }

    public void getData(){
        Scanner data = new Scanner(port.getInputStream());
        while(data.hasNextLine()){
            String line = data.nextLine();
            //System.out.println(line);
            String[] parts = line.split("x",2);
            System.out.println("Length is: " + parts.length);
            String productName = parts[0];
            //productName.replace(" ", "");
            productName = productName.substring(0,productName.length()-1);
            System.out.println(productName + "dsji");
            String quantity = parts[1];
            dbConnection.updateQuantity(quantity,productName);
        }
    }

}
