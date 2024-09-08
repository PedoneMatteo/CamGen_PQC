#include <stdio.h>
#include <stdlib.h>

#define MAX_HEX_LENGTH 4000 // Definiamo la lunghezza massima di ogni numero esadecimale
void printPubKey();
int main()
{
    FILE *file;
    int hex_number[MAX_HEX_LENGTH];
    int count = 0;

    // Apriamo il file in modalità lettura
    file = fopen("notes1.txt", "r");

    // Controllo se il file è stato aperto correttamente
    if (file == NULL)
    {
        printf("Errore nell'aprire il file!\n");
        return 1;
    }
    unsigned int num, j = 0;
    // Legge i numeri esadecimali fino alla fine del file
    while (fscanf(file, "%x", &num) != EOF)
    {
        // printf("%02X ", num);
        hex_number[j++] = num;
        count++; // Incrementa il contatore ogni volta che trova un numero
    }

    // Chiudi il file
    fclose(file);

    // Stampa il numero totale di numeri esadecimali trovati
    printf("\nNumero di numeri esadecimali trovati: %d\n", count);
    printf(" inizio verificationKey = %x\n", hex_number[211]);
    printf(" ultimo numero verifKey = %x\n", hex_number[211 + 1316]);
    int l = 3951 - 2420;
    printf("\n%x %x %x %x\n", hex_number[l-1], hex_number[l], hex_number[l + 1], hex_number[l + 2]);
    //80 85 82 05 20
    //85 82 09 74 b9 
    //printPubKey();
}

void printPubKey()
{
    char *pubKey = malloc(1312);

    FILE *f = fopen("./POOL_CAM_PQC/CERT_IUT_A_RCA_Dilithium.vkey_pub", "rb");

    if (f == NULL)
    {
        free(pubKey);
        return;
    }

    // read content of file
    size_t bytesRead = fread(pubKey, 1, 1312, f);

    if (bytesRead != 1312)
    {
        free(pubKey);
        pubKey = NULL;
        printf("    ERROR\n\n");
    }

    fclose(f);

    for (int i = 0; i < 1312; i++)
    {
        if (i % 20 == 0)
            printf("\n");
        printf("%02X ", (unsigned char)pubKey[i]);
    }
    printf("\n");
}