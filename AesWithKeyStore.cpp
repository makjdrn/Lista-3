#include <openssl/evp.h>
#include <openssl/bio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
using namespace std;

unsigned char *key;
unsigned char *iv;

void Decrypt_Encrypt(int encrypt, FILE *ifp, FILE *ofp, unsigned char *key, unsigned char *iv) {

	const unsigned BUFSIZE=4096;
	unsigned char *read_buf = (unsigned char *)malloc(BUFSIZE);
	unsigned char *cipher_buf;
	unsigned blocksize;
	int out_len;
	EVP_CIPHER_CTX ctx;
	
	EVP_CipherInit(&ctx, EVP_aes_256_cbc(), key, iv, encrypt);
		
	blocksize = EVP_CIPHER_CTX_block_size(&ctx);
	cipher_buf = (unsigned char *)malloc(BUFSIZE + blocksize);

	while(1) {
		int numRead = fread(read_buf, sizeof(unsigned char), BUFSIZE, ifp);
		EVP_CipherUpdate(&ctx, cipher_buf, &out_len, read_buf, numRead);
		fwrite(cipher_buf, sizeof(unsigned char), out_len, ofp);
		if (numRead < BUFSIZE) {
			break;
		}
	}
	EVP_CipherFinal(&ctx, cipher_buf, &out_len);
	fwrite(cipher_buf, sizeof(unsigned char *), out_len, ofp);

	free(cipher_buf);
	free(read_buf);
}
int main(int argc, char **argv) {

	int il_el = 2689452;
	const unsigned BUFSIZE = 4096;
	char *path = argv[1];
	FILE *p;

	char buf[il_el];
	unsigned char *uc;
	p = fopen(path, "r");
	fread(buf, 1, il_el, p);
	fclose(p);
	string ss = "";
	ss = buf;
	ss[ss.length()] = '\0';
	uc = (unsigned char *)ss.c_str();

	key = (unsigned char *)uc;
	iv = (unsigned char *)argv[2];
	char *filename = argv[3];

	string s = "";
	char buffer[il_el];
	FILE *fp, *enc, *dec;
	unsigned char *file;
	unsigned char *ef, *def;
	int efl, defl;
	unsigned char *readf = (unsigned char *)malloc(BUFSIZE);
	string ED = argv[4];
	if(ED.at(0) == 'E') {
		fp = fopen(filename, "rb");
		enc = fopen("enc", "wb");
		Decrypt_Encrypt(1, fp, enc, key, iv);
		fclose(fp);
		fclose(enc);
 
		char allbuffer[il_el];
		unsigned char *tofn;
		string sb = "";

		enc = fopen("enc", "rb");
		fread(allbuffer, 1, il_el, enc);
		fclose(enc);

		sb = allbuffer;
		sb[sb.length()] = '\0';
		tofn = (unsigned char *)sb.c_str();

		fp = fopen(filename, "wb");
		fwrite(tofn, sizeof(unsigned char *), il_el, fp);
		fclose(fp);

	}
	else if(ED.at(0) == 'D') {
		fp = fopen(filename, "wb");
		enc = fopen("enc", "rb");
		Decrypt_Encrypt(0, enc, fp, key, iv);

		fclose(enc);
		fclose(fp);
	}
	return 0;
}
