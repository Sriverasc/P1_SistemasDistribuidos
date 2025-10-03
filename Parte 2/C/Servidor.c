// server_win.c
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

#pragma comment(lib, "Ws2_32.lib") // ignorado por MinGW, pero útil en MSVC

static int recv_all(SOCKET s, void *buf, int len) {
    char *p = (char*)buf; int off = 0;
    while (off < len) {
        int r = recv(s, p + off, len - off, 0);
        if (r == 0) return 0;        // peer cerró
        if (r < 0) return -1;        // error
        off += r;
    }
    return 1;
}
static int send_all(SOCKET s, const void *buf, int len) {
    const char *p = (const char*)buf; int off = 0;
    while (off < len) {
        int n = send(s, p + off, len - off, 0);
        if (n <= 0) return -1;
        off += n;
    }
    return 0;
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { printf("WSAStartup failed\n"); return 1; }

    SOCKET srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srv == INVALID_SOCKET) { printf("socket failed: %d\n", WSAGetLastError()); WSACleanup(); return 1; }

    BOOL yes = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5001);

    if (bind(srv, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError()); closesocket(srv); WSACleanup(); return 1;
    }
    if (listen(srv, 1) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError()); closesocket(srv); WSACleanup(); return 1;
    }
    printf("Servidor Win escuchando en 0.0.0.0:5001\n");

    struct sockaddr_in cli; int clilen = sizeof(cli);
    SOCKET c = accept(srv, (struct sockaddr*)&cli, &clilen);
    if (c == INVALID_SOCKET) { printf("accept failed: %d\n", WSAGetLastError()); closesocket(srv); WSACleanup(); return 1; }

    for (;;) {
        uint32_t netnum;
        int st = recv_all(c, &netnum, sizeof(netnum));
        if (st == 0) { printf("Cliente cerró\n"); break; }
        if (st < 0) { printf("recv error: %d\n", WSAGetLastError()); break; }

        uint32_t num = ntohl(netnum);
        printf("Recibido: %u\n", num);

        if (num == 0) { printf("Cerrando por 0\n"); break; }

        uint32_t resp = htonl(num + 1);
        if (send_all(c, &resp, sizeof(resp)) < 0) { printf("send error: %d\n", WSAGetLastError()); break; }
    }

    closesocket(c);
    closesocket(srv);
    WSACleanup();
    return 0;
}
