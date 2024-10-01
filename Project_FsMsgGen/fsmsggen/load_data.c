/*********************************************************************
This file is a part of FItsSec project: Implementation of ETSI TS 103 097
Copyright (C) 2015  Denis Filatov (danya.filatov()gmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed under GNU GPLv3 in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOd.  See the
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
#include "extensions.h"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>

#define OQS_SIG_dilithium_2_length_public_key 1312
#define OQS_SIG_dilithium_2_length_secret_key 2528
#define OQS_SIG_dilithium_2_length_signature 2420

#endif

int _tbsHashType = 0;
char _tbsHash[512]; // has space for issuer hash + signer hash
int _tbsHashLength = 0;

char _signerHashBuf[256]; // has space for issuer hash
const char *_signerHashRCA = &_signerHashBuf[0];
const char *_signerHashAA = &_signerHashBuf[0];
int _signerHashLength = 0;

int _num_appPerms;
int lenCert = 3960; // 187

char *signerAA;
char *signerAT;


EtsiExtendedCertificate *Emulated_InstallCertificate(char *data, size_t *struct_len, size_t cert_length, const char *vkey, size_t vkey_length, const char *ekey, size_t ekey_length, int *perror, int flag_PQC, char *entity);
void freeCertificate(EtsiExtendedCertificate *cert, int flag_PQC);

void printCertificate(unsigned char *data, int len)
{
	printf("\n	CERTIFICATE: \n");
	for (int i = 0; i < len; i++)
	{
		if (i % 32 == 0)
			printf("\n");
		printf("%02x ", data[i]);
	}
	printf("\n");
}

int isRCA = 0;

char *set_signer(char *path)
{
	char *entity;
	if (strstr(path, "RCA") != NULL)
	{
		entity = "RCA";
		signerAA = malloc(strlen(path) + 1);
		strcpy(signerAA, path);
	}
	else if (strstr(path, "AA") != NULL)
	{
		entity = "AA";
		signerAT = malloc(strlen(path) + 1);
		strcpy(signerAT, path);
	}
	else
	{
		if (strstr(path, "AT") != NULL){
			entity = "AT";
			pathMyCert = malloc(strlen(path)+1);
			memcpy(pathMyCert,path,strlen(path)+1);
		}
	}
	return entity;
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

	data = malloc(CERT_MAX_SIZE);
	end = cstrnload(data, CERT_MAX_SIZE, path);

	// set the entity of the path as the signer of the lower entity in the PKI
	char *entity = set_signer(path);

	if (strstr(path, "Dilithium") != NULL)
		flag_PQC = 1;
	// printCertificate(data);
	if (end > data)
	{
		if ((data[0] & 0x7F) == 0 && data[1] == 0x03)
		{
			// looks like certificate
			// look for keys
			size_t cert_len = end - data;

			ext = cstrpathextension(fname);
			// cmemcmp() checks if the strings are not the same
			if ((ext - fname) > 3 && cmemcmp("_EA", ext - 3, 3) && cmemcmp("_AA", ext - 3, 3) && cmemcmp("_RCA", ext - 4, 4))
			{
				pchar_cpy(ext, ".vkey");
				vkey = _data;
				end = cstrnload(vkey, sizeof(_data), path);

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
			isRCA++; 

			// printCertificate(data, cert_len);

			size_t certif_len;
			if (flag_PQC)
			{
				EtsiExtendedCertificate *certif = Emulated_InstallCertificate(data, &certif_len, cert_len, vkey, vkey_len, ekey, ekey_len, &error, flag_PQC, entity);
				if (certif == NULL)
				{
					printf(" ERROR: PQC certificate not valid");
				}
			}
			else
			{
				const FSCertificate *c = FitSec_InstallCertificate(e, data, cert_len, vkey, vkey_len, ekey, ekey_len, &error);
				digest = FSCertificate_Digest(c);
				const char *name = FSCertificate_Name(c);
				if (name == NULL)
				{
					FSCertificate_SetName(c, fname);
				}
				printf("%5.5s %20.20s [%016" PRIX64 "] %s- %s\n", FitSec_Name(e), fname, cint64_hton(digest), vkey ? "(local) " : "", error ? FitSec_ErrorMessage(error) : "CERT");
			}
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
	size_t plen, plen_pqc;
	pchar_t *path, *path_pqc;

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

	plen_pqc = _path ? pchar_len(_path) + 4 : 0;
	path_pqc = malloc((plen + 252) * sizeof(pchar_t));

	if (path_pqc == NULL)
		return -1;
	if (plen_pqc)
	{
		strcpy(path_pqc, path);
		strcat(path_pqc, "_PQC");
		while (plen_pqc && (path_pqc[plen_pqc - 1] == '/' || path_pqc[plen_pqc - 1] == '\\'))
			plen--;
	}
	if (plen_pqc == 0)
		path_pqc[plen_pqc++] = '.';
	path_pqc[plen_pqc] = 0;

	int ret = _FitSec_LoadTrustData(e, curTime, path, plen, path + plen + 255); //,path_pqc, plen_pqc, path_pqc + plen_pqc + 251); // path = ../../POOL_CAM
	// int ret1 = _FitSec_LoadTrustData(e, curTime, path_pqc, plen_pqc, path_pqc + plen_pqc + 255);
	free(path);
	free(path_pqc);
	free(signerAT);
	free(signerAA);
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

// Comparison function to sort files
int compare_files(const void *a, const void *b)
{
	const char *file_a = *(const char **)a;
	const char *file_b = *(const char **)b;

	// Define the sorting logic based on specific names
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

	// If neither RCA, AA, nor AT, sort alphabetically
	return strcmp(file_a, file_b);
}

static int _FitSec_LoadTrustData(FitSec *e, FSTime32 curTime, pchar_t *path, int plen, pchar_t *end)
{
	printf("\n\n  _FitSec_LoadTrustData\n\n");
	int count = 0;
	struct stat st;
	if (0 == stat(path, &st))
	{ // function stat checks if the path exists
		if (S_ISREG(st.st_mode))
		{										 // check if the path is a regular file
			pchar_t *fn = pchar_rchr(path, '/'); // find the last occurence of "/" into the path to obtain the name of the file
			if (fn && fn[1] != 0)
				fn++;
			else
				fn = path;
			if (0 == _load_data(e, curTime, path, fn))
			{ // call _load_data to fill the struct FitSec with the content of the file
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

// Funzione per leggere dati da un buffer
#define READ_DATA(dest, src, len) \
	memcpy((dest), (src), (len)); \
	(src) += (len);

// Funzione per emulare FitSec_InstallCertificate
EtsiExtendedCertificate *Emulated_InstallCertificate(char *data, size_t *struct_len, size_t cert_length, const char *vkey, size_t vkey_length, const char *ekey, size_t ekey_length, int *perror, int flag_PQC, char *entity)
{
	uint8_t *ptr = data;
	EtsiExtendedCertificate *cert = (EtsiExtendedCertificate *)malloc(sizeof(EtsiExtendedCertificate));
	size_t total_length = sizeof(EtsiExtendedCertificate);
	if (!cert)
	{
		*perror = -1; // Errore: memoria non disponibile
		return NULL;
	}

	memset(cert, 0, sizeof(EtsiExtendedCertificate));
	ptr++;
	// Inizializza le strutture di base
	cert->version = *ptr++;
	cert->type = *ptr++;
	cert->issuer.issuerType = *ptr++;
	if (cert->issuer.issuerType == 0x81)
	{
		ptr++;
	}
	else
	{
		cert->issuer.sha256.Digest = malloc(8);
		total_length += 8;
		if (isRCA == 1) // if turn = 1 RCA certificate, otherwise AA or AT certificate
		{	 // RCA
			READ_DATA(cert->issuer.sha256.Digest, ptr, 8);
		}
		else // AA, AT
		{
			for (int i = 0; i < 8; i++)
			{
				cert->issuer.sha256.Digest[i] = *ptr++;
			}
		}
	}

	// toBeSigned
	char *_toBeSigned = ptr;
	int lenTBS = 0;
	ptr++;
	lenTBS++;
	
	cert->toBeSigned.id_Value.idType = *ptr++;
	lenTBS++;
	
	if (cert->toBeSigned.id_Value.idType == 0x83)
	{
		ptr++;
		lenTBS++;
	}
	else
	{
		cert->toBeSigned.id_Value.id.name.len = *ptr++;
		lenTBS++;
		
		cert->toBeSigned.id_Value.id.name.val = malloc(cert->toBeSigned.id_Value.id.name.len);
		total_length += cert->toBeSigned.id_Value.id.name.len;
		READ_DATA(cert->toBeSigned.id_Value.id.name.val, ptr, cert->toBeSigned.id_Value.id.name.len);
		lenTBS += cert->toBeSigned.id_Value.id.name.len;
	}

	for (int i = 0; i < 3; i++)
	{
		cert->toBeSigned.cracaId[i] = *ptr++;
		lenTBS++;
	}

	for (int i = 0; i < 2; i++)
	{
		if (*ptr == 0x00)
		{
			cert->toBeSigned.crlSeries[i] = *ptr++;
			lenTBS++;
		}
		else
			cert->toBeSigned.crlSeries[i] = 0x00;
	}

	for (int i = 0; i < 4; i++)
	{
		cert->toBeSigned.validityPeriod.start[i] = *ptr++;
		lenTBS++;
	}

	cert->toBeSigned.validityPeriod.durationType = *ptr++;
	lenTBS++;

	if (cert->toBeSigned.validityPeriod.durationType == 0x84)
	{
		for (int i = 0; i < 2; i++)
		{
			cert->toBeSigned.validityPeriod.duration.hours[i] = *ptr++;
			lenTBS++;
		}
	}

	cert->toBeSigned.assuranceLevel = *ptr;
	ptr += 2;
	lenTBS += 2;
	int num_appPerms = *ptr++;
	lenTBS++;
	_num_appPerms = num_appPerms;
	
	cert->toBeSigned.appPermissions = malloc(num_appPerms * sizeof(PsidSsp));
	total_length += (num_appPerms * sizeof(PsidSsp));

	for (int i = 0; i < num_appPerms; i++)
	{
		int ifBitmap = *ptr++;
		lenTBS++;
	
		if (*ptr == 0x01)
		{
			ptr++;
			lenTBS++;
			cert->toBeSigned.appPermissions[i].psid = *ptr++;
			lenTBS++;
			cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength = 0;
		}
		else if (*ptr == 0x02)
		{
			ptr++;
			lenTBS++;
			cert->toBeSigned.appPermissions[i].psid = (*ptr << 8) | *(ptr + 1);
			ptr += 2;
			lenTBS += 2;
			cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength = 0;
		}

		if (ifBitmap != 0x00)
		{
			ptr += 2;
			lenTBS += 2;
			cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength = *ptr++;
			lenTBS++;
		
			cert->toBeSigned.appPermissions[i].ssp.bitmapSsp = malloc(cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength);
			total_length += cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength;

			for (int j = 0; j < cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength; j++)
			{
				cert->toBeSigned.appPermissions[i].ssp.bitmapSsp[j] = *ptr++;
				lenTBS++;
			}
		}
	//	else
	//		printf("Skipping, no bitmap SSP present for appPermissions[%d]\n", i);
		
	}

	if (*ptr == 0x01)
	{
		ptr++;
		lenTBS++;

		int numGroupPerms = *ptr++;
		lenTBS++;
		
		cert->toBeSigned.certIssuePermissions.numGroupPermissions = numGroupPerms;
		cert->toBeSigned.certIssuePermissions.psidGroupPermissions = malloc(numGroupPerms * sizeof(PsidGroupPermissions));
		total_length += (numGroupPerms * sizeof(PsidGroupPermissions));

		ptr += 3;
		lenTBS += 3;

		for (int j = 0; j < numGroupPerms; j++)
		{
			cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.numRanges = *ptr;
			int numRanges = *ptr++;
			lenTBS++;
			
			cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges = malloc(numRanges * sizeof(PsidSspRange));
			total_length += (numRanges * sizeof(PsidSspRange));

			for (int k = 0; k < numRanges; k++)
			{
				if (*ptr == 0x80)
				{
					ptr++;
					lenTBS++;
					if (*ptr == 0x01)
					{
						ptr++;
						lenTBS++;
						cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = *ptr++;
						lenTBS++;
					}
					else
					{
						ptr++;
						lenTBS++;
						cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = (*ptr << 8) | *(ptr + 1);
						ptr += 2;
						lenTBS += 2;
					}
					ptr += 2;
					lenTBS += 2;

					cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.initialized = 1;
					cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValueLength = *ptr;
					
					cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValue = malloc(*ptr * sizeof(uint8_t));
					total_length += cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValueLength; // Aggiungi la lunghezza della sspValue
					ptr++;
					lenTBS++;

					for (int z = 0; z < cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValueLength; z++)
					{
						cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValue[z] = *ptr++;
						lenTBS++;
					}

					cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmaskLength = *ptr;
					
					cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmask = malloc(*ptr * sizeof(uint8_t));
					total_length += cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmaskLength;
					ptr++;
					lenTBS++;

					for (int z = 0; z < cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmaskLength; z++)
					{
						cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmask[z] = *ptr++;
						lenTBS++;
					}
				}
				else if (*ptr == 0x00)
				{
					ptr++;
					lenTBS++;
					if (*ptr == 0x01)
					{
						ptr++;
						lenTBS++;
						cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = *ptr++;
						lenTBS++;
					}
					else
					{
						cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].psid = (*ptr << 8) | *(ptr + 1);
						ptr += 2;
						lenTBS += 2;
					}
					cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.initialized = 0;
				}
			}

			if (*ptr == 0x01)
			{
				ptr++;
				lenTBS++;
				cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].minChainLength = *ptr++;
				lenTBS++;
				cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].eeType = *ptr++;
				lenTBS++;
			}
			else
			{
				cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].minChainLength = 1;
				cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].chainLengthRange = 0;
				cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].eeType = *ptr++;
				lenTBS++;
			}

			if (j + 1 < numGroupPerms)
			{
				ptr += 3;
				lenTBS += 3;
			}
		}
	}

	if (*ptr == 0x00)
	{
		// Fill supportedSymmAlg
		cert->toBeSigned.encryptionKey.supportedSymmAlg = FS_AES_128_CCM;
		ptr++;
		lenTBS++;

		// Check for public key type
		if (*ptr == 0x80)
		{
			ptr++;
			lenTBS++;
			// Set the curve type
			cert->toBeSigned.encryptionKey.publicKey.curve = FS_NISTP256;
			
			// Check for the point type
			if (*ptr == 0x83)
			{
				ptr++;
				lenTBS++;

				// Set the point type based on the value
				cert->toBeSigned.encryptionKey.publicKey.point.type = FS_COMPRESSED_LSB_Y_1;
				
				// Allocate memory for the x coordinate (compressed format only has the x coordinate)
				cert->toBeSigned.encryptionKey.publicKey.point.x = malloc(32 * sizeof(uint8_t)); // NIST P-256 has 32-byte coordinates
				total_length += 32;
				// Copy the x coordinate from the provided data
				for (int i = 0; i < 32; i++)
				{
					cert->toBeSigned.encryptionKey.publicKey.point.x[i] = *ptr++;
					lenTBS++;
				}
				
				// Since it's compressed-y-1, y coordinate is not used
				cert->toBeSigned.encryptionKey.publicKey.point.y = NULL;
			
				// If you have a `k` parameter to be set, do it here
				cert->toBeSigned.encryptionKey.publicKey.k = NULL; // Assuming no value is provided
			}
			else if (*ptr == 0x82)
			{
				ptr++;
				lenTBS++;
				// Set the point type based on the value
				cert->toBeSigned.encryptionKey.publicKey.point.type = FS_COMPRESSED_LSB_Y_0;
			
				// Allocate memory for the x coordinate (compressed format only has the x coordinate)
				cert->toBeSigned.encryptionKey.publicKey.point.x = malloc(32 * sizeof(uint8_t)); // NIST P-256 has 32-byte coordinates
				total_length += 32;
				// Copy the x coordinate from the provided data
				for (int i = 0; i < 32; i++)
				{
					cert->toBeSigned.encryptionKey.publicKey.point.x[i] = *ptr++;
					lenTBS++;
				}
				
				// Since it's compressed-y-1, y coordinate is not used
				cert->toBeSigned.encryptionKey.publicKey.point.y = NULL;
			
				// If you have a `k` parameter to be set, do it here
				cert->toBeSigned.encryptionKey.publicKey.k = NULL; // Assuming no value is provided
			}
		}
	}
	else
	{
		cert->toBeSigned.encryptionKey.supportedSymmAlg = None;
	}

	if (flag_PQC)
	{
		if (*ptr == 0x80)
		{
			ptr++;
			lenTBS++;
			if (*ptr == 0x85)
			{
				cert->toBeSigned.verifyKeyIndicator.val.DILverificationKey.publicKey = malloc(OQS_SIG_dilithium_2_length_public_key);
				total_length += OQS_SIG_dilithium_2_length_public_key;
				ptr += 4;
				lenTBS += 4;
				READ_DATA(cert->toBeSigned.verifyKeyIndicator.val.DILverificationKey.publicKey, ptr, OQS_SIG_dilithium_2_length_public_key);
				lenTBS += OQS_SIG_dilithium_2_length_public_key;
			}
		}
	}
	else
	{
		if (*ptr == 0x80)
		{
			ptr++;
			lenTBS++;
			if (*ptr == 0x80)
			{
				cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.curve = FS_NISTP256;
				ptr++;
				lenTBS++;

				// Check for the point type
				if (*ptr == 0x82)
				{
					ptr++;
					lenTBS++;

					// Set the point type based on the value
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.type = FS_COMPRESSED_LSB_Y_0;
					
					// Allocate memory for the x coordinate (compressed format only has the x coordinate)
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.x = malloc(32 * sizeof(uint8_t)); // NIST P-256 has 32-byte coordinates
					total_length += 32;
					// Copy the x coordinate from the provided data
					for (int i = 0; i < 32; i++)
					{
						cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.x[i] = *ptr++;
						lenTBS++;
					}

					// Since it's compressed-y-0, y coordinate is not used
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.y = NULL;

					// If you have a `k` parameter to be set, do it here
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.k = NULL; // Assuming no value is provided
				}
				else if (*ptr == 0x83)
				{
					ptr++;
					lenTBS++;

					// Set the point type based on the value
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.type = FS_COMPRESSED_LSB_Y_1;
					
					// Allocate memory for the x coordinate (compressed format only has the x coordinate)
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.x = malloc(32 * sizeof(uint8_t)); // NIST P-256 has 32-byte coordinates
					total_length += 32;
					// Copy the x coordinate from the provided data
					for (int i = 0; i < 32; i++)
					{
						cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.x[i] = *ptr++;
						lenTBS++;
					}
					
					// Since it's compressed-y-1, y coordinate is not used
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.y = NULL;
					
					// If you have a `k` parameter to be set, do it here
					cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.k = NULL; // Assuming no value is provided
				}
			}
		}
	}
	if (flag_PQC)
	{
		if (*ptr == 0x85)
		{

			cert->sig.DILsignature.signature = malloc(OQS_SIG_dilithium_2_length_signature);
			total_length += OQS_SIG_dilithium_2_length_signature;
			ptr += 4;
			
			for (int i = 0; i < OQS_SIG_dilithium_2_length_signature; i++)
				cert->sig.DILsignature.signature[i] = *ptr++;
			}
	}
	else
	{
		if (*ptr == 0x80)
		{
			
			cert->sig.FSsignature.curve = FS_NISTP256;
			ptr++; 

			if (*ptr == 0x80)
			{
				ptr++; 
				cert->sig.FSsignature.point.type = FS_X_COORDINATE_ONLY; 
				cert->sig.FSsignature.point.x = malloc(32 * sizeof(uint8_t));
				total_length += 32;
				for (int i = 0; i < 32; i++)
				{
					cert->sig.FSsignature.point.x[i] = *ptr++;
				}
				
				cert->sig.FSsignature.point.y = NULL;

				cert->sig.FSsignature.s = malloc(32 * sizeof(uint8_t));
				total_length += 32;
				for (int i = 0; i < 32; i++)
				{
					cert->sig.FSsignature.s[i] = *ptr++;
				}
			}
		}
	}

	// OQS_API OQS_STATUS
	verifySignature(data, cert_length, _toBeSigned, lenTBS, cert->sig.DILsignature.signature, entity);

	*struct_len = total_length;

	if (isRCA == 3) 
		extendedCert = &cert;

	ptr = NULL;

	if (strcmp(entity, "AT") == 0)
	{
		myCert.size = cert_length;
		for (int i = 0; i < cert_length; i++)
			myCert.buf[i] = data[i];
	}
	return cert;
}

void verifySignature(uint8_t *data, size_t cert_length, uint8_t *_toBeSigned, int lenTBS, uint8_t *sig, char *entity)
{
	if (strcmp(entity, "RCA") == 0)
	{
		// computation digest of RCA
		data[cert_length - 1] = 0x80;
		sha256_calculate(_signerHashBuf, data, cert_length);
		_signerHashRCA = &_signerHashBuf[0];
		_signerHashLength = sha256_hash_size;
	}
	else if (strcmp(entity, "AA") == 0)
	{
		// TO BE SIGNED HASH
		_tbsHashLength = 32;
		sha256_calculate(_tbsHash, (const char *)_toBeSigned, lenTBS);

		char hash[48];
		int hashlen = 0;

		// calculate JOINT HASH (toBeSigned Hash computed in ToBeSignedCertificate_oer_encoder + signerHash)
		memcpy(_tbsHash + _tbsHashLength, _signerHashRCA, _signerHashLength);
		sha256_calculate(hash, _tbsHash, _tbsHashLength + _signerHashLength);
		hashlen = sha256_hash_size;

		// computation digest of AA
		data[cert_length - 1] = 0x80;
		sha256_calculate(_signerHashBuf, data, cert_length);
		_signerHashAA = &_signerHashBuf[0];
		_signerHashLength = sha256_hash_size;

		char *pubKeySigner = search_public_Dilithium_key(signerAA);
		// verification function
		OQS_STATUS result = OQS_SIG_dilithium_2_verify(hash, hashlen, sig, OQS_SIG_dilithium_2_length_signature, pubKeySigner);
		if (result != OQS_SUCCESS){
			fprintf(stderr, "\nERROR: OQS_SIG_dilithium_2_verify failed!\n");
		}
	}
	else
	{
		// TO BE SIGNED HASH
		_tbsHashLength = 32;
		sha256_calculate(_tbsHash, (const char *)_toBeSigned, lenTBS);

		char hash[48];
		int hashlen = 0;
		// calculate joint hash (toBeSigned Hash computed in ToBeSignedCertificate_oer_encoder + signerHash)
		memcpy(_tbsHash + _tbsHashLength, _signerHashAA, _signerHashLength);
		sha256_calculate(hash, _tbsHash, _tbsHashLength + _signerHashLength);
		hashlen = sha256_hash_size;

		char *pubKeySigner = search_public_Dilithium_key(signerAT);

		// verification function
		OQS_STATUS result = OQS_SIG_dilithium_2_verify(hash, hashlen, sig, OQS_SIG_dilithium_2_length_signature, pubKeySigner);
		if (result != OQS_SUCCESS){
			fprintf(stderr, "\nERROR: OQS_SIG_dilithium_2_verify failed!\n");
		}
	}
}

void freeCertificate(EtsiExtendedCertificate *cert, int flag_PQC)
{
	free(cert->issuer.sha256.Digest);
	if (cert->toBeSigned.id_Value.idType != 0x83)
		free(cert->toBeSigned.id_Value.id.name.val);
	for (int i = 0; i < _num_appPerms; i++)
	{
		if (cert->toBeSigned.appPermissions[i].ssp.bitmapSspLength != 0)
			free(cert->toBeSigned.appPermissions[i].ssp.bitmapSsp);
	}
	free(cert->toBeSigned.appPermissions);

	for (int j = 0; j < cert->toBeSigned.certIssuePermissions.numGroupPermissions; j++)
	{
		printf("\n j: %d\n", j);
		for (int k = 0; k < cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.numRanges; k++)
		{
			printf("\n k: %d\n", k);
			if (cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.initialized)
			{
				free(cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspValue);
				free(cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges[k].sspRange.sspBitmask);
			}
		}
		free(cert->toBeSigned.certIssuePermissions.psidGroupPermissions[j].subjectPermissions.explicitPermissions.psidSspRanges);
	}
	free(cert->toBeSigned.certIssuePermissions.psidGroupPermissions);

	if (cert->toBeSigned.encryptionKey.supportedSymmAlg != None)
	{
		free(cert->toBeSigned.encryptionKey.publicKey.point.x);
	}
	if (flag_PQC)
	{
		printf("start free pqc\n");
		free(cert->toBeSigned.verifyKeyIndicator.val.DILverificationKey.publicKey);
		printf("half free pqc\n");
		free(cert->sig.DILsignature.signature);
		printf("end free pqc\n");
	}
	else
	{
		free(cert->toBeSigned.verifyKeyIndicator.val.FSverificationKey.point.x);
		free(cert->sig.FSsignature.point.x);
		free(cert->sig.FSsignature.s);
	}
	free(cert);
	printf("\n end free cert\n");
}

#endif
