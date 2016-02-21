#include "CRSignal.h"

void CRSignal::listen() {
    this->listenFromKeyboard();
}

void CRSignal::listenFromKeyboard() {
    while (true) {
        char c = _getch();
        this->automaton->transit(c);
    }
}

void CRSignal::listenFromRaspberryPi() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        LOG("WSAStartup failed");
        exit(-1);
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        LOG("Invalid Socket. Error code : %d", WSAGetLastError());
        exit(-1);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(serverHost.c_str());
    address.sin_port = htons(serverPort);

    auto connection = [&sock](sockaddr_in &address) {
        LOG("Connecting");
        while (connect(sock, (sockaddr*)&address, sizeof(address)) != 0);
        LOG("Connection estabilished");
    };

    connection(address);

    int res;
    char buf[2] = "\0";
    while (true) {
        res = recv(sock, buf, 2, 0);
        if (res > 0) {
            if (strlen(buf) > 0) {
                this->automaton->transit(buf[0]);
            }
        }
        else if (res == 0) {
            LOG("Connection closed. reconnect");
            connection(address);
            break;
        }
        else {
            int err = WSAGetLastError();
            LOG("Receive failed: Error code : %d", err);
            switch (err) {
            case WSAENOTCONN:
                connection(address);
                break;
            default:
                exit(-1);
            }
            break;
        }
    }
}