import com.fazecast.jSerialComm.*;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        ArduinoConnection connection = new ArduinoConnection();
        connection.connect();
        MainView mainView = new MainView();
        connection.getData();
    }
}
