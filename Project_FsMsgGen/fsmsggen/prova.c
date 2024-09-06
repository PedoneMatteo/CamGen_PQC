#include <stdio.h>
#include <stdlib.h>

#define MAX_HEX_LENGTH 4000 // Definiamo la lunghezza massima di ogni numero esadecimale

int main()
{
    FILE *file;
    int hex_number[MAX_HEX_LENGTH];
    int count = 0;
 /*
    // Apriamo il file in modalità lettura
    file = fopen("notes1.txt", "r");
   
        // Controllo se il file è stato aperto correttamente
        if (file == NULL) {
            printf("Errore nell'aprire il file!\n");
            return 1;
        }
        unsigned int num, j=0;
        // Legge i numeri esadecimali fino alla fine del file
        while (fscanf(file, "%x", &num) != EOF) {
            //printf("%02X ", num);
            hex_number[j++]=num;
            count++;  // Incrementa il contatore ogni volta che trova un numero
        }


        // Chiudi il file
        fclose(file);

        // Stampa il numero totale di numeri esadecimali trovati
        printf("\nNumero di numeri esadecimali trovati: %d\n", count);
        printf(" inizio verificationKey = %x\n", hex_number[213]);
        printf(" ultimo numero = %x\n", hex_number[213+1311]);
        int l = 3952-2420;
        printf("\n%x %x %x %x\n", hex_number[l-1], hex_number[l], hex_number[l+1], hex_number[l+2]);
       */

 // Apriamo il file in modalità lettura
    file = fopen("./POOL_CAM_PQC/CERT_IUT_A_RCA_Dilithium.vkey_pub", "r");
    unsigned int num, j = 0;
    // Legge i numeri esadecimali fino alla fine del file
    while (fscanf(file, "%x", &num) != EOF)
    {
        // printf("%02X ", num);
        hex_number[j++] = num;
        count++; // Incrementa il contatore ogni volta che trova un numero
    }
    fclose(file);
    printf("primo e utlimo numero = %x - %x\n", hex_number[0], hex_number[j-1]);
}
