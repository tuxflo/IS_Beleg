#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

int main (void)
{
  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */

  /* A 256 bit key */
  unsigned char key[32];


  // fopen corrupted file
  FILE *corrupted;

  corrupted = fopen("./s68407-src-cipher.bin", "r");
  if(corrupted == NULL)
  {
    printf("unable to open file\n");
    exit(-1);
  }

  //copy key in array using fread
  FILE * key = fopen("s68407-corrupt-src-key.bin", "r");
  long length;
  unsigned char *buffer;
  if(key == NULL)
  {
    printf("unable to open key file\n");
    exit(-1);
  }
  fseek (key, 0, SEEK_END);
  length = ftell (key);
  fseek (key, 0, SEEK_SET);
  buffer = malloc (length);
  if (!buffer)
  {
    printf("error using malloc\n");
    exit(-2);
  }
  fread (buffer, 1, length, key);
  fclose (key);
  printf("key: %s\n", key);

  EVP_CIPHER_CTX ctx;
  
  //rückgabewert
  const EVP_CIPHER *chifer;
  //zum Vergleichen der Key sizes
  //
  EVP_CIPHER_key_length == 32;
  //bei arbeit & Operator verwenden
  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */
  int byte = 0;
  for(byte = 0; byte < 256; byte++)
  {
    key[30] = byte;

    //Decrypt init
    //Datei in Puffer laden
    //Decrypt update (dazu wird ausgangspuffer benötigt)
    //erste 4 Byte in pdf sind "%PDF"
    //Vergleich mit memcmp durchführen falls nicht pdf, continue
    //Ausgabepuffer in Datei schreiben
    //EVP_Decrypt_final
    //Ausgabepuffer erneut in Datei schreiben
    //EVP Cipher Cleanup
    //EVP_MD context
    //MD value als char array länge 28 Byte 
    unsigned char value[28];
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
  return 0;
}
