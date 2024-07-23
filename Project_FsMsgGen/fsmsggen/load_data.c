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

void printCertificate(unsigned char *data){
	printf("\n	CERTIFICATE: \n");
	for(int i=0; i<187; i++){
		printf("%02x ", data[i]);
	}
	printf("\n");
}

static char _data[4096]; // for keys only
static FSHashedId8 _load_data(FitSec * e, FSTime32 curTime, pchar_t * path, pchar_t * fname)
{
	char *data, *end;
	char *vkey = NULL, *ekey = NULL;
	size_t vkey_len = 0, ekey_len = 0;
	pchar_t *ext;
	FSHashedId8 digest = (FSHashedId8)0;
	int error = 0;
		printf("\npath in _load_data = %s\n",path);
	end = cstraload(&data, path);
	printCertificate(data);
	if (end > data){
        if ( (data[0] & 0x7F) == 0 && data[1] == 0x03) {
			// looks like certificate
			// look for keys
			size_t cert_len = end - data;

			ext = cstrpathextension(fname);
			printf("\n print extension = %s\n", ext);	//ext = .oer         ext-3  =  RCA.oer / _AT.oer / _AA.oer
			printf("\n	bool = %d\n",(ext - fname) > 3 && cmemcmp("_EA", ext-3, 3) && cmemcmp("_AA", ext-3, 3) && cmemcmp("_RCA", ext-4, 4));

			//cmemcmp() controlla se le stringhe passate alla funzione non sono uguali
			if((ext - fname) > 3 && cmemcmp("_EA", ext-3, 3) && cmemcmp("_AA", ext-3, 3) && cmemcmp("_RCA", ext-4, 4)) { 
				pchar_cpy(ext, ".vkey"); //strcpy
				printf("\n print extension after pchar_cpy = %s\n", ext); //ext = .vkey
					vkey = _data;
					end = cstrnload(vkey, sizeof(_data), path);
					printf("\n path = %s\n", path);	//path = ../../POOL_CAM/CERT_IUT_A_AT.vkey
				if (end <= vkey){
					end = vkey; vkey = NULL;
				}
				else{
					vkey_len = end - vkey;
				}

				pchar_cpy(ext, ".ekey");
				ekey = end;
					end = cstrnload(ekey, sizeof(_data) - (end-_data), path);
					printf("\n path = %s\n", path); //path = ../../POOL_CAM/CERT_IUT_A_AT.ekey
				if (end <= ekey){
					end = ekey; ekey = NULL;
				}
				else{
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
			const FSCertificate* c =  FitSec_InstallCertificate(e, data, cert_len, vkey, vkey_len, ekey, ekey_len, &error);
			digest = FSCertificate_Digest(c);
			const char * name = FSCertificate_Name(c);
			printf("\n 	digest of %s = %lx\n\n",name,digest);
			if(name == NULL){
				FSCertificate_SetName(c, fname);
			}
			printf("%5.5s %20.20s [%016"PRIX64"] %s- %s\n", FitSec_Name(e), fname, cint64_hton(digest), vkey?"(local) ":"", error ? FitSec_ErrorMessage(error):"CERT");
		}else if(data[0] == 0x03){
			printf("%-2s %20.20s Install trust info:\n", FitSec_Name(e), fname);
			error = FitSec_ApplyTrustInformation(e, curTime, data, end - data);
			printf("%-2s %20.20s End trust info: %s\n", FitSec_Name(e), fname, FitSec_ErrorMessage(error));
			digest = 1;
		} else {
			printf("%-2s %20.20s Unknown format\n", FitSec_Name(e), fname);
		}
		free(data);
	}
	else{
		printf("%-2s: %s: Empty File\n", FitSec_Name(e), fname);
	}
	return digest;
}

static int _FitSec_LoadTrustData(FitSec * e, FSTime32 curTime, pchar_t * path, int plen, pchar_t * end);

int FitSec_LoadTrustData(FitSec * e, FSTime32 curTime, const pchar_t * _path)
{
	size_t plen;
	pchar_t *path;

	plen = _path ? pchar_len(_path) : 0;
	path = malloc((plen + 256) * sizeof(pchar_t));
	if (path == NULL) return -1;
	if (plen){
		memcpy(path, _path, plen * sizeof(pchar_t));
		while (plen && (path[plen - 1] == '/' || path[plen - 1] == '\\')) plen--;
	}
	if (plen == 0) path[plen++] = '.';
	path[plen] = 0;
	int ret = _FitSec_LoadTrustData(e, curTime, path, plen, path+plen+255); //path = ../../POOL_CAM
	free(path);
	return ret;
}

#ifdef WIN32
static int _FitSec_LoadTrustData(FitSec * e, FSTime32 curTime, pchar_t * path, int plen, pchar_t * end)
{
	WIN32_FIND_DATA fd;
	HANDLE h;
	int count = 0;
	printf("\n\n IM IN _FitSec_LoadTrustData WIN32\n\n");
	fd.dwFileAttributes = GetFileAttributes(path);
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		pchar_cpy(path + plen, "/*"); plen++;
		h = FindFirstFile(path, &fd);
		if (INVALID_HANDLE_VALUE != h) {
			do {
				int dlen = pchar_len(fd.cFileName);
				if(path + plen + dlen <= end){
					pchar_t * ext = pchar_rchr(fd.cFileName, '.');
					if(!ext ||
						0 == strcmp(ext, ".oer") || 
						0 == strcmp(ext, ".crl") || 
						0 == strcmp(ext, ".ctl") || 
						0 == strcmp(ext, ".lcr") || 
						0 == strcmp(ext, ".oer")
					){
						pchar_cpy(path + plen, fd.cFileName);
						count +=  _FitSec_LoadTrustData(e, curTime, path, plen+dlen, end);
					}
				}
			} while (FindNextFile(h, &fd));
			FindClose(h);
		}
	}else{
		FSHashedId8 digest = _load_data(e, curTime, path, path + plen);
		if (digest != (FSHashedId8)-1) {
				count++;
		}
	}
	return count;
}

#else

static int _FitSec_LoadTrustData(FitSec * e, FSTime32 curTime, pchar_t * path, int plen, pchar_t * end)
{
	printf("\n\n IM IN _FitSec_LoadTrustData\n\n");
	int count = 0;
	struct stat st;
	if(0 == stat(path, &st)){ // function stat checks if the path exists
		if (S_ISREG(st.st_mode)) { // check if the path is a regular file
			pchar_t * fn = pchar_rchr(path, '/'); // find the last occurence of "/" into the path to obtain the name of the file
			printf("\nREGULAR FILE: path: %s , fn: %s\n", path, fn);
			if(fn && fn[1] != 0) fn++;
			else   fn = path;
			printf("\npath: %s , fn: %s\n", path, fn);
			if (0 == _load_data(e, curTime, path, fn)) { // call _load_data to fill the struct FitSec with the file 
				errno = 0;
				count++;
			}else if(errno){
				perror(path);
			}
		}else if (S_ISDIR(st.st_mode)) { // check if the path is a directory
			printf("\nthe path is a directory: %s\n", path);
			DIR * d;
			struct dirent * de;
			d = opendir(path); // open directory
			if(d){
				path[plen++] = '/';			
/*Constructs the full path to the file and calls _FitSec_LoadTrustData recursively 
for each file, incrementing count with the result of the recursive call*/
				while(NULL != (de = readdir(d))){
					pchar_t * ext = pchar_rchr(de->d_name, '.');
					if(!ext ||
						0 == strcmp(ext, ".oer") || 
						0 == strcmp(ext, ".crl") || 
						0 == strcmp(ext, ".ctl") || 
						0 == strcmp(ext, ".lcr") || 
						0 == strcmp(ext, ".crt")
					){
						int dlen = strlen(de->d_name);
						if(path  + plen + dlen < end){
							pchar_cpy(path + plen, de->d_name);
							count += _FitSec_LoadTrustData(e, curTime, path, plen+dlen, end);
						}
					}
				}
				closedir(d);
			}
			if (errno) {
				perror(path);
			}
		}
	}else{
		if(errno)
			perror(path);
	}
	return count;
}	
#endif