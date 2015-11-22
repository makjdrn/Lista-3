#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <iostream>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <typeinfo>
#include <openssl/md5.h>
using namespace std;

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
char *md5(char *pin) {
	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5((unsigned char *)pin, strlen(pin), (unsigned char*)&digest);

	char *mdString = (char*)malloc(33);
	for(int i = 0; i < 16; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
	return mdString;
}
int main(int argc, char **argv) {

	FILE *file, *enc, *pin;
	int il_el = 20000;
	int random;
	unsigned char *key = (unsigned char *)"01234567890123456789012";
	unsigned char *ivv = (unsigned char *)"0123456";
	const char *c = "\"";
	if((file = fopen("PK", "r")) == NULL) {
		file = fopen("PK", "w+");
		const char *keypath = "\"/home/marcin/Dokumenty/Kryptografia 2015/klucze\"";
		fprintf(file, "%s", keypath);
		const char *iv = "012345667890123456";
		fprintf(file, "\n%s", iv);
		char *password = getpass("Enter password: ");
		fprintf(file, "\n%s", password);
		cout << "keypath:" << keypath << "\niv: " << iv << "\npassword: " << password << endl;
		fclose(file);
		file = fopen("PK", "rb");
		enc = fopen("enc", "wb");
		Decrypt_Encrypt(1, file, enc, key, ivv);
		fclose(file);
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

		file = fopen("PK", "wb");
		fwrite(tofn, sizeof(unsigned char *), il_el, file);
		fclose(file);
		char pinn[8];
		cout << "Podaj pin:" << endl;
		cin >> pinn;
		char *s = md5(pinn);
		pin = fopen("PIN", "wb");
		fprintf(pin, "%s", s);
		fclose(pin);
	}
	else {
char buffer[100];
		char buf[il_el];
		char path2[50];
		string path22;
		char dkk[40];
		char *path = argv[1];
		cout << path << endl;
		FILE *fkey, *decfile, *enc2;

		char *passpin = getpass("Enter PIN: ");
		char c[8];
		strcpy(c, passpin);
		char *ss = md5(c);
		char *s = (char *)malloc(33);
		pin = fopen("PIN", "rb");
		fscanf(pin, "%s", s);
		fclose(pin);


		if(strcmp(s, ss) == 0) {
			cout << "Jestem" << endl;
			file = fopen("PK", "rb");
			enc = fopen("enc", "w");
			Decrypt_Encrypt(0, file, enc, key, ivv);
			fclose(enc);
			fclose(file);
			enc = fopen("enc", "rb");
			fgets(path2, 100, enc);
			fgets(dkk, 40, enc);				
			fclose(enc);
			path2[strlen(path2)-1] = '\0';
			dkk[strlen(dkk)-1] = '\0';
			const char *cc = path2;
			cout << typeid(path2).name() << "\n" << typeid(cc).name() << "\n" << typeid("\"/home/marcin/Dokumenty/Kryptografia 2015/klucze\"").name() << endl;
			fkey = fopen("/home/marcin/Dokumenty/Kryptografia 2015/klucze", "rb");
			fread(buf, 1, 100, fkey);
			cout << buf << endl;
			fclose(fkey);
			cout << "fkey: " << buf << endl;
	
			string sss = "";
			sss = buf;
			sss[sss.length()] = '\0';
			unsigned char *kkey;
			kkey = (unsigned char *)sss.c_str();
			unsigned char *idk = (unsigned char *)dkk;
			cout << idk << " " << kkey << " " << path << endl;
			decfile = fopen(path, "rb");
			enc2 = fopen("enc2", "wb");
			Decrypt_Encrypt(0, decfile, enc2, kkey, idk);
			fclose(decfile);
			fclose(enc2);
		
			/*char allbuffer[il_el];
			unsigned char *tofn;
			string sb = "";

			enc2 = fopen("enc2", "rb");
			fread(allbuffer, 1, il_el, enc2);
			fclose(enc2);

			sb = allbuffer;
			sb[sb.length()] = '\0';
			tofn = (unsigned char *)sb.c_str();

			decfile = fopen(path, "wb");
			fwrite(tofn, sizeof(unsigned char *), il_el, decfile);
			fclose(decfile);*/

			libvlc_instance_t *instance;
			libvlc_media_player_t *mp;
			libvlc_media_t *m;

			instance = libvlc_new(0, NULL);

	
			cout << argv[1] << endl;

			m = libvlc_media_new_path(instance, "enc2");
			cout << "blee" << endl;

			mp = libvlc_media_player_new_from_media(m);

			libvlc_media_release(m);

			libvlc_media_player_play(mp);

			sleep(1000);

			libvlc_media_player_stop(mp);

			libvlc_media_player_release(mp);

			libvlc_release(instance);
		}
	}

	return 0;
}
