// ClienteWin.c (Windows)
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { fprintf(stderr, "WSAStartup failed\n"); return 1; }

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) { fprintf(stderr, "socket failed\n"); WSACleanup(); return 1; }

    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &srv.sin_addr); // requiere ws2tcpip.h y _WIN32_WINNT adecuado
    if (connect(s, (struct sockaddr*)&srv, sizeof(srv)) == SOCKET_ERROR) { fprintf(stderr, "connect failed\n"); closesocket(s); WSACleanup(); return 1; }

    const char *msg = "Hola\n";
    send(s, msg, (int)strlen(msg), 0);

    char buf[256];
    int r = recv(s, buf, sizeof(buf)-1, 0);
    if (r > 0) { buf[r] = '\0'; printf("Servidor respondio: %s", buf); }

    closesocket(s);
    WSACleanup();
    system("pause");
    return 0;
}
