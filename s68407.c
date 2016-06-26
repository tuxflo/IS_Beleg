#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <openssl/evp.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (void)
{

  // fopen corrupted file
  unsigned char *ciphertextbuffer;
  unsigned char *plaintextbuffer;
  unsigned char *keytextbuffer;
  FILE *keytextfile;
  FILE *cipherfile;
  FILE *hashfile;
  long keylen;
  long cipherlen;
  //load the key file (classic fopen)
  keytextfile = fopen("./s68407-corrupt-src-key.bin", "r");
  if(keytextfile == NULL)
  {
    printf("error opening key file\n");
    exit(EXIT_FAILURE);
  }
  fseek (keytextfile, 0, SEEK_END);
  keylen = ftell (keytextfile);
  fseek (keytextfile, 0, SEEK_SET);
  
  keytextbuffer = malloc (keylen);
  if (!keytextbuffer)
  {
    printf("error using malloc\n");
    exit(EXIT_FAILURE);
  }
  fread (keytextbuffer, 1, keylen, keytextfile);
  fclose (keytextfile);
  printf("Key: %s\n", keytextbuffer);

  //loading cipher file
  cipherfile = fopen("./s68407-src-cipher.bin", "r");
  if(cipherfile == NULL)
  {
    printf("error opening cipher file\n");
    exit(EXIT_FAILURE);
  }
  fseek (cipherfile, 0, SEEK_END);
  cipherlen = ftell (cipherfile);
  fseek (cipherfile, 0, SEEK_SET);
  
  ciphertextbuffer = malloc (cipherlen);
  if (!ciphertextbuffer)
  {
    printf("error using malloc\n");
    exit(EXIT_FAILURE);
  }
  fread (ciphertextbuffer, 1, cipherlen, cipherfile);
  fclose (cipherfile);
  printf("Ciphertext: %s\n", ciphertextbuffer);

  plaintextbuffer = malloc (cipherlen);
  if (!plaintextbuffer)
  {
    printf("error using malloc\n");
    exit(EXIT_FAILURE);
  }
  //open_file_memory_mapped_read("./s68407-src-cipher.bin", &ciphertextbuffer, &cipher_text_meta);

  //open_file_memory_mapped_write("./output.pdf", &plaintextbuffer, &plain_text_meta, plain_text_meta.file_info.st_size);
  FILE *outputfile;
  //load the key file (classic fopen)
  outputfile = fopen("./output.pdf", "r");
  if(outputfile == NULL)
  {
    printf("error opening outputfile file\n");
    exit(EXIT_FAILURE);
  }
  printf("Opening output file done\n");

  //chifer = EVP_CIPHER_key_length(;
  
  //zum Vergleichen der Key sizes
  //
  //EVP_CIPHER_key_length == 32;
  //bei arbeit & Operator verwenden
  int byte = 0;
  //unsigned char key[256];
  int plaintextlen = 0;
  for(byte = 0; byte < 256; byte++)
  {
    keytextbuffer[30] = byte;

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
      printf("error initialising cipher context\n");
      exit(EXIT_FAILURE);
    }
  
    //Decrypt init
    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, keytextbuffer, NULL))
    {
      printf("error decrypt init\n");
      exit(-4);
    }
    //Decrypt update (dazu wird ausgangspuffer benötigt)
    if(!EVP_DecryptUpdate(ctx, plaintextbuffer, &plaintextlen, ciphertextbuffer, cipherlen))
    {
      printf("error decrypt update\n");
      exit(-4);
    }


    //erste 4 Byte in pdf sind "%PDF"
    //Vergleich mit memcmp durchführen falls nicht pdf, continue
    unsigned char pdf_start[] = {"%PDF"};
    int finallen;
    if(!memcmp(pdf_start, plaintextbuffer, sizeof(pdf_start) - 1))
    {
        printf("Correct pdf using byte: %d\n", keytextbuffer[30]);
        printf("plaintextlen %d\n", plaintextlen);
        fwrite(plaintextbuffer, plaintextlen, 1, outputfile);
        if(!EVP_DecryptFinal_ex(ctx, plaintextbuffer + plaintextlen, &finallen))
        {
          printf("error decrypt final\n");
        }
        printf("finallen %d\n", finallen);
        printf("plaintextlen %d\n", plaintextlen);
        int completelen = plaintextlen + finallen;
        printf("completelen %d\n", completelen);
        

        fseek(outputfile, 0L, SEEK_SET);
        fwrite(plaintextbuffer, completelen, 1, outputfile);
        fclose(outputfile);

        free(plaintextbuffer);
        EVP_CIPHER_CTX_free(ctx);
        printf("decrypting done\n");
        break;
    }
    //Ausgabepuffer in Datei schreiben
    //EVP_Decrypt_final
    //Ausgabepuffer erneut in Datei schreiben
    //EVP Cipher Cleanup
    //EVP_MD context
    //MD value als char array länge 28 Byte 
    //unsigned char value[28];
    //Datei laden
    //MD_CTX init
    //EVP_DigestInit
    //EVP_DigestUpdate
    //EVP_DigestFinal


    //4. dest_key einlesen zum Schlüssel holen
    //output file
    //context anlegen
    //initialisieren
    //CipherUpdate zu verschlüsseln
    //CipherFinal
  }
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    if(!mdctx)
    {
      printf("error creating MD context\n");
      exit(EXIT_FAILURE);
    }
    unsigned char value[28];
    unsigned int hash_len;
    outputfile = fopen("./output.pdf", "r");
    if(outputfile == NULL)
    {
      printf("error opening output file\n");
      exit(EXIT_FAILURE);
    }
    fseek (outputfile, 0, SEEK_END);
    plaintextlen = ftell (outputfile);
    fseek (outputfile, 0, SEEK_SET);

    plaintextbuffer = malloc (plaintextlen);
    if (!plaintextbuffer)
    {
      printf("error using malloc\n");
      exit(EXIT_FAILURE);
    }
    fread (plaintextbuffer, 1, plaintextlen, outputfile);
    fclose (outputfile);
    printf("reading output file done\n");
    if(!EVP_DigestInit_ex(mdctx,  EVP_sha224(), NULL))
    {
      printf("error digest init\n");
      exit(EXIT_FAILURE);
    }
    if(!EVP_DigestUpdate(mdctx, plaintextbuffer, plaintextlen))
    {
      printf("error digest update\n");
      exit(EXIT_FAILURE);
    }
    if(!EVP_DigestFinal_ex(mdctx, value, &hash_len))
    {
      printf("error digest final\n");
      exit(EXIT_FAILURE);
    }
    unsigned int i=0;
    printf("hash: ");
    for(i=0; i<hash_len; i++)
    {

      printf(" %02X", value[i]);
    }
    printf("\n");
    hashfile = fopen("./hash", "wb");
    if(hashfile == NULL)
    {
      printf("error opening hashfile\n");
      exit(EXIT_FAILURE);
    }
    if(1 != fwrite(value, hash_len, 1, hashfile))
    {
      printf("error writing hashfile\n");
      exit(EXIT_FAILURE);
    }
    fclose(hashfile);

    //encryption
    EVP_CIPHER_CTX *enc_ctx =  EVP_CIPHER_CTX_new();
    keytextfile = fopen("./s68407-dest-key.bin", "r");
    unsigned char *hashbuffer;
    if(keytextfile == NULL)
    {
      printf("error opening key file\n");
      exit(EXIT_FAILURE);
    }
    fseek (keytextfile, 0, SEEK_END);
    keylen = ftell (keytextfile);
    fseek (keytextfile, 0, SEEK_SET);

    hashbuffer = malloc (keylen);
    if (!hashbuffer)
    {
      printf("error using malloc\n");
      exit(EXIT_FAILURE);
    }
    fread (hashbuffer, 1, keylen, keytextfile);
    printf("hash enc key: %s\n", hashbuffer);
    fclose (keytextfile);

    unsigned char *encbuffer;
    encbuffer = malloc (hash_len);
    if (!encbuffer)
    {
      printf("error using malloc\n");
      exit(EXIT_FAILURE);
    }

    if(!enc_ctx)
    {
      printf("error creating cipher context\n");
      exit(EXIT_FAILURE);
    }
    if(!EVP_EncryptInit_ex(enc_ctx, EVP_des_ede(), NULL, hashbuffer, NULL))
    {
      printf("error encrypt init\n");
      exit(EXIT_FAILURE);
    }
    printf("init done\n");
    int final = 0;
    int enclen = 0;
    if(!EVP_EncryptUpdate(enc_ctx, encbuffer, &enclen, value, hash_len))
    {
      printf("error encrypt update\n");
      exit(EXIT_FAILURE);
    }
    printf("update done\n");
    if(!EVP_EncryptFinal_ex(enc_ctx, encbuffer + enclen, &final))
    {
      printf("error encrypt final\n");
      exit(EXIT_FAILURE);
    }
    int final_hash_len = encbuffer + final;
    printf("final done\n");
    printf("encrypted hash: %s\n", encbuffer);

    free(plaintextbuffer);
  return 0;
}
