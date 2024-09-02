/*********************************************************************
This file is a part of FItsSec project: Implementation of ETSI TS 103 097
Copyright (C) 2015  Denis Filatov (danya.filatov()gmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed under GNU GPLv3 in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/gpl-3.0.txt>.
@license GPL-3.0+ <http://www.gnu.org/licenses/gpl-3.0.txt>

In particular cases this program can be distributed under other license
by simple request to the author.
*********************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define _FILE_OFFSET_BITS 64

#include "cstr.h"
#include "cmem.h"
#include "fitsec.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <inttypes.h>
#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#endif
int lenCert = 3960; // 187

MyFSCertificate MyInstallCertificate(char *data, size_t cert_length, const char *vkey, size_t vkey_length, const char *ekey, size_t ekey_length, int *perror);
EtsiTs103097Certificate *Emulated_InstallCertificate(char *data, size_t cert_length, const char *vkey, size_t vkey_length, const char *ekey, size_t ekey_length, int *perror);
void printCertificate(unsigned char *data, int len)
{
	printf("\n	CERTIFICATE: \n");
	for (int i = 0; i < len; i++)
	{
		printf("%02x ", data[i]);
	}
	printf("\n");
}

static char _data[4096]; // for keys only
static FSHashedId8 _load_data(FitSec *e, FSTime32 curTime, pchar_t *path, pchar_t *fname)
{
	char *data, *end;
	char *vkey = NULL, *ekey = NULL;
	size_t vkey_len = 0, ekey_len = 0;
	pchar_t *ext;
	FSHashedId8 digest = (FSHashedId8)0;
	int error = 0;
	printf("\npath in _load_data = %s\n", path);
	end = cstraload(&data, path);
	// printCertificate(data);
	if (end > data)
	{
		if ((data[0] & 0x7F) == 0 && data[1] == 0x03)
		{
			// looks like certificate
			// look for keys
			size_t cert_len = end - data;

			ext = cstrpathextension(fname);
			printf("\n print extension = %s\n", ext); // ext = .oer         ext-3  =  RCA.oer / _AT.oer / _AA.oer

			// cmemcmp() controlla se le stringhe passate alla funzione non sono uguali
			if ((ext - fname) > 3 && cmemcmp("_EA", ext - 3, 3) && cmemcmp("_AA", ext - 3, 3) && cmemcmp("_RCA", ext - 4, 4))
			{
				pchar_cpy(ext, ".vkey");								  // strcpy
				printf("\n print extension after pchar_cpy = %s\n", ext); // ext = .vkey
				vkey = _data;
				end = cstrnload(vkey, sizeof(_data), path);
				printf("\n path = %s\n", path); // path = ../../POOL_CAM/CERT_IUT_A_AT.vkey
				if (end <= vkey)
				{
					end = vkey;
					vkey = NULL;
				}
				else
				{
					vkey_len = end - vkey;
				}

				pchar_cpy(ext, ".ekey");
				ekey = end;
				end = cstrnload(ekey, sizeof(_data) - (end - _data), path);
				printf("\n path = %s\n", path); // path = ../../POOL_CAM/CERT_IUT_A_AT.ekey
				if (end <= ekey)
				{
					end = ekey;
					ekey = NULL;
				}
				else
				{
					ekey_len = end - ekey;
				}
				errno = 0;
			}
			*ext = 0;
			/** Install certificate (root, AA, local AT pseudonymes, any other)
			 *  Install any kind of certificates: AA, AT, EC, EA, Root, etc.
			 *  Local AT certificates shall be followed by private keys.
			 *  TODO: support for HSM key sorage
			 *  @param e     The FitSec engine
			 *  @param cert         The buffer with the certificate data
			 *  @param cert_length  The buffer size
			 *  @param vkey         The verification private key.
			 *                      Required for AT certificates. Must be NULL for other types.
			 *
			 *  @param vkey_length  The size of the key
			 *  @param ekey         The encryption private key.fitsec.
			 *                      Optional for AT certificates. Must be NULL for other types.
			 *  @param ekey_length  The size of the key
			 *  @param perror       The status of the procedure. See fitsec_error.h.
			 *  @return             The certificate structure or NULL if certificate is not installed.
			 */
			printf("\n	 encryption key len = %ld -- verification key len = %ld\n\n", ekey_len, vkey_len);
			printf("\n   cert len = %d\n", cert_len);
			/*	ekey_len+=32;
				vkey_len+=32;
				cert_len+=64;	*/

			printCertificate(data, cert_len);
			EtsiTs103097Certificate *certif = Emulated_InstallCertificate(data, cert_len, vkey, vkey_len, ekey, ekey_len, &error);
			// MyFSCertificate certificate = MyInstallCertificate(data, cert_len, vkey, vkey_len, ekey, ekey_len, &error);
			const FSCertificate *c = FitSec_InstallCertificate(e, data, cert_len, vkey, vkey_len, ekey, ekey_len, &error);
			digest = FSCertificate_Digest(c);
			const char *name = FSCertificate_Name(c);
			printf("\n 	digest of %s = %lx\n\n", name, digest);
			if (name == NULL)
			{
				FSCertificate_SetName(c, fname);
			}
			printf("%5.5s %20.20s [%016" PRIX64 "] %s- %s\n", FitSec_Name(e), fname, cint64_hton(digest), vkey ? "(local) " : "", error ? FitSec_ErrorMessage(error) : "CERT");
		}
		else if (data[0] == 0x03)
		{
			printf("%-2s %20.20s Install trust info:\n", FitSec_Name(e), fname);
			error = FitSec_ApplyTrustInformation(e, curTime, data, end - data);
			printf("%-2s %20.20s End trust info: %s\n", FitSec_Name(e), fname, FitSec_ErrorMessage(error));
			digest = 1;
		}
		else
		{
			printf("%-2s %20.20s Unknown format\n", FitSec_Name(e), fname);
		}
		free(data);
	}
	else
	{
		printf("%-2s: %s: Empty File\n", FitSec_Name(e), fname);
	}
	return digest;
}

static int _FitSec_LoadTrustData(FitSec *e, FSTime32 curTime, pchar_t *path, int plen, pchar_t *end);

int FitSec_LoadTrustData(FitSec *e, FSTime32 curTime, const pchar_t *_path)
{
	size_t plen;
	pchar_t *path;

	plen = _path ? pchar_len(_path) : 0;
	path = malloc((plen + 256) * sizeof(pchar_t));
	if (path == NULL)
		return -1;
	if (plen)
	{
		memcpy(path, _path, plen * sizeof(pchar_t));
		while (plen && (path[plen - 1] == '/' || path[plen - 1] == '\\'))
			plen--;
	}
	if (plen == 0)
		path[plen++] = '.';
	path[plen] = 0;
	int ret = _FitSec_LoadTrustData(e, curTime, path, plen, path + plen + 255); // path = ../../POOL_CAM
	free(path);
	return ret;
}

#ifdef WIN32
static int _FitSec_LoadTrustData(FitSec *e, FSTime32 curTime, pchar_t *path, int plen, pchar_t *end)
{
	WIN32_FIND_DATA fd;
	HANDLE h;
	int count = 0;
	printf("\n\n IM IN _FitSec_LoadTrustData WIN32\n\n");
	fd.dwFileAttributes = GetFileAttributes(path);
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		pchar_cpy(path + plen, "/*");
		plen++;
		h = FindFirstFile(path, &fd);
		if (INVALID_HANDLE_VALUE != h)
		{
			do
			{
				int dlen = pchar_len(fd.cFileName);
				if (path + plen + dlen <= end)
				{
					pchar_t *ext = pchar_rchr(fd.cFileName, '.');
					if (!ext ||
						0 == strcmp(ext, ".oer") ||
						0 == strcmp(ext, ".crl") ||
						0 == strcmp(ext, ".ctl") ||
						0 == strcmp(ext, ".lcr") ||
						0 == strcmp(ext, ".oer"))
					{
						pchar_cpy(path + plen, fd.cFileName);
						count += _FitSec_LoadTrustData(e, curTime, path, plen + dlen, end);
					}
				}
			} while (FindNextFile(h, &fd));
			FindClose(h);
		}
	}
	else
	{
		FSHashedId8 digest = _load_data(e, curTime, path, path + plen);
		if (digest != (FSHashedId8)-1)
		{
			count++;
		}
	}
	return count;
}

#else

// Funzione di confronto per ordinare i file
int compare_files(const void *a, const void *b)
{
	const char *file_a = *(const char **)a;
	const char *file_b = *(const char **)b;

	// Definisci la logica di ordinamento basata sui nomi specifici
	if (strstr(file_a, "RCA") && !strstr(file_b, "RCA"))
		return -1;
	if (strstr(file_b, "RCA") && !strstr(file_a, "RCA"))
		return 1;

	if (strstr(file_a, "AA") && !strstr(file_b, "AA"))
		return -1;
	if (strstr(file_b, "AA") && !strstr(file_a, "AA"))
		return 1;
	if (strstr(file_b, "AA") && strstr(file_a, "AA"))
		return 1;

	if (strstr(file_a, "AT") && !strstr(file_b, "AT"))
		return -1;
	if (strstr(file_b, "AT") && !strstr(file_a, "AT"))
		return 1;
	if (strstr(file_b, "AT") && strstr(file_a, "AT"))
		return 1;

	// Se non sono né RCA, né AA, né AT, ordina alfabeticamente
	return strcmp(file_a, file_b);
}

static int _FitSec_LoadTrustData(FitSec *e, FSTime32 curTime, pchar_t *path, int plen, pchar_t *end)
{
	printf("\n\n IM IN _FitSec_LoadTrustData\n\n");
	int count = 0;
	struct stat st;
	if (0 == stat(path, &st))
	{ // function stat checks if the path exists
		if (S_ISREG(st.st_mode))
		{										 // check if the path is a regular file
			pchar_t *fn = pchar_rchr(path, '/'); // find the last occurence of "/" into the path to obtain the name of the file
			printf("\nREGULAR FILE: path: %s , fn: %s\n", path, fn);
			if (fn && fn[1] != 0)
				fn++;
			else
				fn = path;
			printf("\npath: %s , fn: %s\n", path, fn);
			if (0 == _load_data(e, curTime, path, fn))
			{ // call _load_data to fill the struct FitSec with the file
				errno = 0;
				count++;
			}
			else if (errno)
			{
				perror(path);
			}
		}
		else if (S_ISDIR(st.st_mode))
		{ // check if the path is a directory
			printf("\nthe path is a directory: %s\n", path);
			DIR *d;
			struct dirent *de;
			d = opendir(path); // open directory
			if (d)
			{
				path[plen++] = '/';
				int max_num_files = 10;
				char *files[max_num_files];
				int file_count = 0;
				/*Constructs the full path to the file and calls _FitSec_LoadTrustData recursively
				for each file, incrementing count with the result of the recursive call*/
				while (NULL != (de = readdir(d)))
				{
					pchar_t *ext = pchar_rchr(de->d_name, '.');
					if (!ext ||
						0 == strcmp(ext, ".oer") ||
						0 == strcmp(ext, ".crl") ||
						0 == strcmp(ext, ".ctl") ||
						0 == strcmp(ext, ".lcr") ||
						0 == strcmp(ext, ".crt"))
					{
						if (file_count < max_num_files) // save all file names
						{
							files[file_count] = strdup(de->d_name);
							file_count++;
						}
					}
				}
				// order files -> RCA,AA,AT
				qsort(files, file_count, sizeof(char *), compare_files);

				for (int i = 0; i < file_count; i++)
				{
					int dlen = strlen(files[i]);
					if (path + plen + dlen < end)
					{
						pchar_cpy(path + plen, files[i]);
						count += _FitSec_LoadTrustData(e, curTime, path, plen + dlen, end);
						free(files[i]);
					}
				}
				closedir(d);
			}
			if (errno)
			{
				perror(path);
			}
		}
	}
	else
	{
		if (errno)
			perror(path);
	}
	return count;
}

/*
						80 03 00 80 a9 ba 9e f3
cb 74 4d 7c 31 83 00 00 00 00 00 25 9e 9d 80 84
22 38 60 01 03 80 01 24 81 04 03 01 ff fc 80 01
25 81 05 04 01 ff ff ff 00 01 8d 00 80 83 cf 0e
ed a0 b4 10 85 a8 b7 bc c8 f9 16 f8 34 66 31 64
7c 30 a5 0a 36 81 54 07 27 d6 0b 81 74 fb 80 80
82 91 2a b6 34 8d 5a 8b 82 2e 9c a6 13 bf 34 25
7c d7 cb 50 09 12 42 ee e3 bf 60 60 44 11 1a a2
d0 80 80 e1 ca 87 f2 3e 48 ae c3 dc 3c 9b f2 ce
42 59 63 a5 99 db dc 6e 2b 55 8b 9d 21 53 15 7f
88 24 1a 96 2a 36 5a b2 61 0c 58 1a b2 27 f3 fd
df 69 e3 ed 24 19 f2 4e 2b 75 07 39 03 b9 f3 51
47 34 bf
*/

// Funzione per leggere dati da un buffer
#define READ_DATA(dest, src, len) \
	memcpy((dest), (src), (len)); \
	(src) += (len);

// Funzione per emulare FitSec_InstallCertificate
EtsiTs103097Certificate *Emulated_InstallCertificate(char *data, size_t cert_length, const char *vkey, size_t vkey_length, const char *ekey, size_t ekey_length, int *perror)
{
	uint8_t *ptr = data;
	EtsiTs103097Certificate *cert = (EtsiTs103097Certificate *)malloc(sizeof(EtsiTs103097Certificate));
	if (!cert)
	{
		*perror = -1; // Errore: memoria non disponibile
		return NULL;
	}

	memset(cert, 0, sizeof(EtsiTs103097Certificate));
	ptr++;
	// Inizializza le strutture di base
	cert->version = *ptr++;
	cert->type = *ptr++;
	cert->issuer.issuerType = *ptr++;
	if (cert->issuer.issuerType == 0x81)
	{
		*ptr++;
	}
	else
	{
		cert->issuer.sha256.Digest = malloc(8);
		READ_DATA(cert->issuer.sha256.Digest, ptr, 8);
	}

	// toBeSigned
	*ptr++;
	// Avanza il puntatore e assegna l'idType
	cert->toBeSigned.id_Value.idType = *ptr++;
	printf("idType: %x\n", cert->toBeSigned.id_Value.idType);

	if (cert->toBeSigned.id_Value.idType == 0x83)
	{
		*ptr++;
	}
	else
	{
		// Assegna la lunghezza del nome
		cert->toBeSigned.id_Value.id.name.len = *ptr++;
		printf("name.len: %x\n", cert->toBeSigned.id_Value.id.name.len);

		// Alloca memoria e legge il valore del nome
		cert->toBeSigned.id_Value.id.name.val = malloc(cert->toBeSigned.id_Value.id.name.len);
		READ_DATA(cert->toBeSigned.id_Value.id.name.val, ptr, cert->toBeSigned.id_Value.id.name.len);

		// Stampa il valore del nome come stringa
		printf("name.val: ");
		for (int i = 0; i < cert->toBeSigned.id_Value.id.name.len; i++)
		{
			printf("%02x ", cert->toBeSigned.id_Value.id.name.val[i]);
		}
		printf("\n");
	}

	// Legge il cracaId
	printf("cracaId: ");
	for (int i = 0; i < 3; i++)
	{
		cert->toBeSigned.cracaId[i] = *ptr++;
		printf("%02x ", cert->toBeSigned.cracaId[i]);
	}
	printf("\n");

	// Legge il crlSeries
	printf("crlSeries: ");
	for (int i = 0; i < 2; i++)
	{
		if (*ptr == 0x00)
			cert->toBeSigned.crlSeries[i] = *ptr++;
		else
			cert->toBeSigned.crlSeries[i] = 0x00;
		printf("%02x ", cert->toBeSigned.crlSeries[i]);
	}
	printf("\n");

	// Legge il validityPeriod start
	printf("validityPeriod.start: ");
	for (int i = 0; i < 4; i++)
	{
		cert->toBeSigned.validityPeriod.start[i] = *ptr++;
		printf("%02x ", cert->toBeSigned.validityPeriod.start[i]);
	}
	printf("\n");

	// Legge il durationType
	cert->toBeSigned.validityPeriod.durationType = *ptr++;
	printf("durationType: %x\n", cert->toBeSigned.validityPeriod.durationType);

	if (cert->toBeSigned.validityPeriod.durationType == 0x84)
	{
		// Legge la durata in ore
		printf("duration.hours: ");
		for (int i = 0; i < 2; i++)
		{
			cert->toBeSigned.validityPeriod.duration.hours[i] = *ptr++;
			printf("%02x ", cert->toBeSigned.validityPeriod.duration.hours[i]);
		}
		printf("\n");
	}

	// Legge l'assuranceLevel
	cert->toBeSigned.assuranceLevel = *ptr;
	printf("assuranceLevel: %x\n", cert->toBeSigned.assuranceLevel);
	ptr += 2;
	int num_appPerms = *ptr++;
	
	printf("num_appPerms: %d\n", num_appPerms);

	cert->toBeSigned.appPermissions = malloc(num_appPerms * sizeof(PsidSsp));

	for (int i = 0; i < num_appPerms; i++)
	{
		int ifBitmap = *ptr++;
		printf("ifBitmap[%d]: %x\n", i, ifBitmap);

		if (*ptr == 0x01)
		{
			ptr++;
			cert->toBeSigned.appPermissions[i].psid = *ptr++;
			printf("psid[%d] (1-byte): %x\n", i, cert->toBeSigned.appPermissions[i].psid);
		}
		else if (*ptr == 0x02)
		{
			ptr++;
			cert->toBeSigned.appPermissions[i].psid = (*ptr << 8) | *(ptr + 1);
			printf("psid[%d] (2-byte): %x\n", i, cert->toBeSigned.appPermissions[i].psid);
			ptr += 2;
		}

		if (ifBitmap != 0x00)
		{
			ptr += 2; 
			cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength = *ptr++;
			printf("bitmapSspLength[%d]: %d\n", i, cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength);

			cert->toBeSigned.appPermissions[i].ssp.bitmapSsp = malloc(cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength);

			for (int j = 0; j < cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength; j++)
			{
				cert->toBeSigned.appPermissions[i].ssp.bitmapSsp[j] = *ptr++;
				printf("bitmapSsp[%d][%d]: %x\n", i, j, cert->toBeSigned.appPermissions[i].ssp.bitmapSsp[j]);
			}
		}
		else
		{
			printf("Skipping, no bitmap SSP present for appPermissions[%d]\n", i);
		}
	}

if (*ptr == 0x01) {
    printf("\ncertIssuePermissions detected\n");
    ptr++;
    
    int numGroupPerms = *ptr++;
    printf("numGroupPerms: %d\n", numGroupPerms);
    
    cert->toBeSigned.certIssuePermissions.numGroupPermissions = numGroupPerms;
    cert->toBeSigned.certIssuePermissions.psidGroupPermissions = malloc(numGroupPerms * sizeof(PsidGroupPermissions));
    
    ptr += 3;
    
    for (int j = 0; j < numGroupPerms; j++) {
        cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.numRanges = *ptr;
        int numRanges = *ptr++;
        printf("Group %d - numRanges: %d\n", j, numRanges);
        
        cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges = malloc(numRanges * sizeof(PsidSspRange));
        
        for (int k = 0; k < numRanges; k++) {
			printf("\n\n k = %d \n\n", k);
            if (*ptr == 0x80) {
                ptr++;
                if (*ptr == 0x01) {
					ptr++;
                    cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = *ptr++;
                    printf("psid (1-byte): 0x%02X\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid);
                } else {
					ptr++;
                    cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = (*ptr << 8) | *(ptr + 1);
                    printf("psid (2-byte): 0x%04X\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid);
                    ptr += 2;
                }
                ptr += 2;
                
                cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValueLength = *ptr;
                printf("sspValueLength: %d\n", *ptr);
                
                cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValue = malloc(*ptr * sizeof(uint8_t));
                ptr++;
                
                for (int z = 0; z < cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValueLength; z++) {
                    cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValue[z] = *ptr++;
                    printf("sspValue[%d]: 0x%02X\n", z, cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValue[z]);
                }
                
                cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmaskLength = *ptr;
                printf("sspBitmaskLength: %d\n", *ptr);
                
                cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmask = malloc(*ptr * sizeof(uint8_t));
                ptr++;
                
                for (int z = 0; z < cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmaskLength; z++) {
                    cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmask[z] = *ptr++;
                    printf("sspBitmask[%d]: 0x%02X\n", z, cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmask[z]);
                }
				//printf("ptr = %x\n",*ptr);
            } else if (*ptr == 0x00) {
                ptr++;
                if (*ptr == 0x01) {
					ptr++;
                    cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = *ptr++;
                    printf("psid (1-byte): 0x%02X\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid);
                } else {
                    cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = (*ptr << 8) | *(ptr + 1);
                    printf("psid (2-byte): 0x%04X\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid);
                    ptr += 2;
                }
            }
        }
        
        if (*ptr == 0x01) {
            ptr++;
            cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].minChainLength = *ptr++;
            printf("minChainLength: %d\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].minChainLength);
            cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].eeType = *ptr++;
            printf("eeType: %x\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].eeType);
        } else {
            cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].minChainLength = 1;
            cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].chainLengthRange = 0;
            cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].eeType = *ptr++;
            printf("Default minChainLength: 1, chainLengthRange: 0, eeType: %x\n", cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].eeType);
        }

		if(j+1<numGroupPerms)
			ptr+=3;
    }
}
printf("\n\n			ptr = %x\n", *ptr);

	/*
		// Parsing dell'Issuer
		cert->issuer.name = (char *)ptr;
		ptr += strlen((char *)ptr) + 1;  // Salta il nome del certificatore (issuer)

		cert->issuer.issuerType.sha256AndDigest = *ptr++;

		// Parsing del campo ToBeSigned
		cert->toBeSigned.id = *ptr++;
		READ_DATA(cert->toBeSigned.cracaId, ptr, 7);
		READ_DATA(&cert->toBeSigned.crlSeries, ptr, sizeof(cert->toBeSigned.crlSeries));

		// Validity Period
		READ_DATA(&cert->toBeSigned.validityPeriod.start, ptr, sizeof(cert->toBeSigned.validityPeriod.start));
		READ_DATA(&cert->toBeSigned.validityPeriod.duration.hours, ptr, sizeof(cert->toBeSigned.validityPeriod.duration.hours));

		// App Permissions
		cert->toBeSigned.numAppPermissions = 1;  // Supponendo che ci sia un singolo permesso
		cert->toBeSigned.appPermissions = (PsidSsp *)malloc(sizeof(PsidSsp));
		if (!cert->toBeSigned.appPermissions) {
			*error = -2;  // Errore: memoria non disponibile per i permessi
			free(cert);
			return NULL;
		}
		READ_DATA(&cert->toBeSigned.appPermissions->psid, ptr, sizeof(cert->toBeSigned.appPermissions->psid));
		cert->toBeSigned.appPermissions->ssp.bitmapSspLength = *ptr++;
		cert->toBeSigned.appPermissions->ssp.bitmapSsp = (uint8_t *)malloc(cert->toBeSigned.appPermissions->ssp.bitmapSspLength);
		READ_DATA(cert->toBeSigned.appPermissions->ssp.bitmapSsp, ptr, cert->toBeSigned.appPermissions->ssp.bitmapSspLength);

		// Cert Issue Permissions
		cert->toBeSigned.certIssuePermissions.numGroupPermissions = 1;  // Supponendo che ci sia un singolo gruppo di permessi
		cert->toBeSigned.certIssuePermissions.psidGroupPermissions = (PsidGroupPermissions *)malloc(sizeof(PsidGroupPermissions));
		if (!cert->toBeSigned.certIssuePermissions.psidGroupPermissions) {
			*error = -3;  // Errore: memoria non disponibile per i permessi di emissione
			free(cert->toBeSigned.appPermissions);
			free(cert);
			return NULL;
		}
		// (Continua il parsing degli altri campi come minChainLength, chainLengthRange, eeType, ecc.)

		// Parsing EncryptionKey e VerifyKeyIndicator
		cert->toBeSigned.encryptionKey.supportedSymmAlg = *ptr++;
		memcpy(&cert->toBeSigned.encryptionKey.publicKey, ekey, sizeof(FSPublicKey));
		memcpy(&cert->toBeSigned.verifyKeyIndicator.verificationKey, vkey, sizeof(FSPublicKey));

		// Parsing della Signature
		// Supponiamo che la firma sia di tipo ECDSA con NIST P-256
		cert->signature.curve = FS_NISTP256;  // Supponendo che si usi questa curva
		READ_DATA(&cert->signature.point, ptr, sizeof(cert->signature.point));
		cert->signature.s = (uint8_t *)malloc(32);  // Supponendo che la firma sia 256-bit
		if (!cert->signature.s) {
			*error = -4;  // Errore: memoria non disponibile per la firma
			free(cert->toBeSigned.certIssuePermissions.psidGroupPermissions);
			free(cert->toBeSigned.appPermissions);
			free(cert);
			return NULL;
		}
		READ_DATA(cert->signature.s, ptr, 32);

		// Se ci sono altri campi da analizzare, aggiungili qui

		*error = 0;  // Nessun errore
		*/
	return cert;
}

MyFSCertificate MyInstallCertificate(char *data, size_t cert_length, const char *vkey, size_t vkey_length, const char *ekey, size_t ekey_length, int *perror)
{
	MyFSCertificate cert;
	/*
	int j = 1;
	cert.version = data[j++]; // j=1
	printf("cert.version = %x\n", cert.version);

	cert.type = data[j++]; // j=2
	printf("cert.type = %x\n", cert.type);

	cert.issuer.type = data[j++]; // j=3
	printf("cert.issuer.type = %x\n", cert.issuer.type);

	for (int i = 0; i < 8; i++)
	{
		cert.issuer.sha256Digest[i] = data[j++]; // j=4...11
		printf("cert.issuer.sha256Digest[%d] = %x\n", i, cert.issuer.sha256Digest[i]);
	}

	// j=12 now
	j++;

	cert.toBeSigned.id.val = data[j++]; // j=13
	printf("cert.toBeSigned.id.idType = %x\n", cert.toBeSigned.id.val);


	for (int i = 0; i < 3; i++)
	{
		cert.toBeSigned.cracaId[i] = data[j++]; // j=14,15,16
		printf("cert.toBeSigned.cracaId[%d] = %x\n", i, cert.toBeSigned.cracaId[i]);
	}

	cert.toBeSigned.crlSeries = (data[j] << 8) | data[j + 1];
	j += 2; // j=17,18
	printf("cert.toBeSigned.crlSeries = %x\n", cert.toBeSigned.crlSeries);

	for (int i = 0; i < 4; i++)
	{
		cert.toBeSigned.validityPeriod.start[i] = data[j++];
		printf("cert.toBeSigned.validityPeriod.start[%d] = %x\n",i, cert.toBeSigned.validityPeriod.start[i]);
	} // j=19,20,21,22

	cert.toBeSigned.validityPeriod.duration.type = data[j++]; // j=23
	printf("cert.toBeSigned.validityPeriod.duration.type = %x\n", cert.toBeSigned.validityPeriod.duration.type);

	cert.toBeSigned.validityPeriod.duration.hours = (data[j] << 8) | data[j + 1];
	j += 2; // j=24,25
	printf("cert.toBeSigned.validityPeriod.duration.hours = %x\n", cert.toBeSigned.validityPeriod.duration.hours);

	cert.toBeSigned.assuranceLevel = data[j++]; // j=26
	printf("cert.toBeSigned.assuranceLevel = %x\n", cert.toBeSigned.assuranceLevel);
	j++;

	cert.toBeSigned.appPermissions.appPermissionsCount = data[j++];
	printf("cert.toBeSigned.appPermissions.appPermissionsCount = %d\n", cert.toBeSigned.appPermissions.appPermissionsCount);

	for(int i = 0; i < cert.toBeSigned.appPermissions.appPermissionsCount; i++) {
		cert.toBeSigned.appPermissions.item[i].sspType = data[j++];  // SSP Type
		printf("appPermissions.items[%d].sspType = %x\n", i, cert.toBeSigned.appPermissions.item[i].sspType);

		// PSID (encoded in a compact format with MSB signaling continuation)
		if (data[j] & 0x80) {  // Se il bit più significativo è 1, il PSID è su 2 byte
			cert.toBeSigned.appPermissions.item[i].psid = ((data[j] & 0x7F) << 8) | data[j + 1];
			j += 2;
		} else {  // PSID su 1 byte
			cert.toBeSigned.appPermissions.item[i].psid = data[j++];
		}
		printf("appPermissions.items[%d].psid = %x\n", i, cert.toBeSigned.appPermissions.item[i].psid);

		// SSP (se presente)
		if (data[j] & 0x80) {  // Verifica se c'è un SSP
			uint8_t sspLength = data[j++] & 0x7F;  // Ottieni la lunghezza dell'SSP
			cert.toBeSigned.appPermissions.item[i].bitmapLength = sspLength;
			for (int k = 0; k < sspLength; k++) {
				cert.toBeSigned.appPermissions.item[i].bitmap[k] = data[j++];
			}
			printf("appPermissions.items[%d].bitmap = ", i);
			for (int k = 0; k < sspLength; k++) {
				printf("%02x", cert.toBeSigned.appPermissions.item[i].bitmap[k]);
			}
			printf("\n");
		} else {
			cert.toBeSigned.appPermissions.item[i].bitmapLength = 0;  // Nessun SSP
		}
	}*/
	return cert;
}

#endif

/*all'interno del mio certificato c'è questo campo che vedo cosi definito in wireshark
appPermission: 3 Items
	Item 0
		PsidSsp
			psid: psid-ca-basic-services (36)
			ssp: bitmapSsp (1)
				bitmapSsp: 01fffc
	Item 1
		PsidSsp
			psid: psid-den-basic-services(37)
			ssp: bitmapSsp (1)
				bitmapSsp: 01ffffff
	Item 2
		PsidSsp
			psid: psid-geonetworking-management-communications (141)

i dati binari che fanno riferimento a questa struttura sono:
01 03 80 01 24 81 04 03 01 ff fc 80 01 25 81 05 04 01 ff ff ff 00 01 8d

non ho il codice in cui i dati vengono inseriti nell'apposita struttura,
mi scrivi il codice sia delle strutture e sia delle assegnazioni dei vari campi ad esse?
*/