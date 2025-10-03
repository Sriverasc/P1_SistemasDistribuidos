import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws Exception {
        String host = "127.0.0.1";
        int port = 5001;
        try (Socket s = new Socket(host, port);
             DataOutputStream out = new DataOutputStream(s.getOutputStream());
             DataInputStream in = new DataInputStream(s.getInputStream());
             Scanner sc = new Scanner(System.in)) {

            while (true) {
                System.out.print("Ingrese entero (0 para salir): ");
                int n = Integer.parseInt(sc.nextLine().trim());
                out.writeInt(n); // big-endian por especificación de DataOutputStream
                out.flush();     // envía inmediatamente
                if (n == 0) {
                    System.out.println("Terminando...");
                    break;
                }
                int resp = in.readInt(); // lee respuesta del servidor
                System.out.println("Servidor respondió: " + resp);
            }
        }
    }
}