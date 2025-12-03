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
    struct sockaddr_in servaddr;

    // Membuat socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    // Bersihkan struktur
    memset(&servaddr, 0, sizeof(servaddr));

    // Konfigurasi alamat server
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(servaddr);

    printf("Terhubung ke server UDP.\n");
    printf("Ketik 'exit' untuk keluar.\n");

    while (1) {
        printf("Masukkan pesan: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Hilangkan newline dari fgets
        buffer[strcspn(buffer, "\n")] = 0;

        // Kirim data ke server
        sendto(
            sockfd,
            buffer,
            strlen(buffer),
            MSG_CONFIRM,
            (const struct sockaddr *)&servaddr,
            len
        );

        if (strcmp(buffer, "exit") == 0) {
            printf("Keluar...\n");
            break;
        }

        // Terima data dari server
        int n = recvfrom(
            sockfd,
            buffer,
            BUFFER_SIZE,
            MSG_WAITALL,
            (struct sockaddr *)&servaddr,
            &len
        );

        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
