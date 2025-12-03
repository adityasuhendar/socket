#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Membuat socket TCP
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Gagal membuat socket");
        return -1;
    }

    // Konfigurasi alamat server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Konversi alamat IPv4 dari text ke binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Alamat tidak valid");
        return -1;
    }

    // Koneksi ke server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Koneksi gagal");
        return -1;
    }

    printf("Terhubung ke server TCP.\n");
    printf("Ketik 'exit' untuk keluar.\n");

    while (1) {
        printf("Masukkan pesan: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Hapus newline dari fgets
        buffer[strcspn(buffer, "\n")] = 0;

        // Kirim data ke server
        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Keluar...\n");
            break;
        }

        // Terima balasan dari server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);

        if (valread > 0)
            printf("Server: %s\n", buffer);
        else
            printf("Server terputus.\n");
    }

    close(sock);
    return 0;
}
