import java.io.*;
import java.net.*;

public class Main {
    public static void main(String[] args) throws IOException {
        int port = 5000; // puerto configurable
        try (ServerSocket server = new ServerSocket(port)) { // crea el servidor y queda a la escucha
            System.out.println("Servidor escuchando en puerto " + port);
            try (Socket socket = server.accept(); // acepta una conexión
                 BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                 PrintWriter out = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true)) {

                String recibido = in.readLine(); // lee una línea del cliente
                System.out.println("Cliente dijo: " + recibido);
                String respuesta = "Hola, que tal"; // responde con otro mensaje
                out.println(respuesta); // envía con terminación de línea
                System.out.println("Respondido: " + respuesta);
            }
        }
    }
}