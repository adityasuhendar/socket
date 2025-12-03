#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Membuat socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    // Bersihkan struktur address
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Konfigurasi alamat server
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket ke port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Gagal bind");
        exit(EXIT_FAILURE);
    }

    printf("Server UDP berjalan di port %d...\n", PORT);

    socklen_t len = sizeof(cliaddr);

    while (1) {
        int n = recvfrom(
            sockfd,
            buffer,
            BUFFER_SIZE,
            MSG_WAITALL,
            (struct sockaddr *)&cliaddr,
            &len
        );

        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        // Perintah keluar
        if (strcmp(buffer, "exit") == 0) {
            printf("Client keluar.\n");
            break;
        }

        // Kirim kembali (echo)
        sendto(
            sockfd,
            buffer,
            strlen(buffer),
            MSG_CONFIRM,
            (const struct sockaddr *)&cliaddr,
            len
        );

        printf("Echo dikirim kembali.\n");
    }

    close(sockfd);
    return 0;
}
