/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2015
##  This code is provided under the CeCill-C license agreement.
######################################################################
*********************************************************************/
#include "ecc_api.h"
#include <stdio.h>
#include "../cshared/copts.h"
#include "../cshared/cstr.h"

static const char *_outPath = ".";
static int _certCount = 1;
static ecc_format   _outFormat = 0;
static ecc_curve_id _curve = 0;

static const char * _o_formats[] = {
	"bin", "hex", "pem", NULL
};

static const char * _o_curves[] = {
	"nist256", "bpool256", "bpool384", NULL
};

static char * _keyname = "key";

static copt_t options [] = {
	{ "h?", "help",  COPT_HELP, NULL,                  "Print this help page" },
	{ "o",  "out",   COPT_STR, (void*)&_outPath,       "Output path [current dir by default]" },
	{ "O", "format", COPT_STRENUM, (void*)_o_formats,  "Output format (bin|hex|pem)[binary by default]" },
	{ "c", "curve", COPT_STRENUM, (void*)_o_curves,    "Curve to be used (nist256|bpool256|bpool384)[nist256 by default]" },
	{ "n", "count", COPT_INT, (void*)&_certCount,      "Key pair count (1 by default)" },
	{ NULL, NULL, COPT_END, NULL, NULL }
};

int main(int argc, char** argv)
{
	argc = coptions(argc, argv, COPT_HELP_NOVALUES , options);

	if(argc < 1){
		if(argc<0 && (0-argc)<((sizeof(options)/sizeof(options[0]))-1)){
			printf("Unknown option %s\n", argv[0-argc]);
		}
		const char * a = strrchr(argv[0], '/');
		if (a == NULL) a = argv[0];
		coptions_help(stdout, a, COPT_HELP_NOVALUES, options, "<keyname>");
		return -1;
	}
	_outFormat = copts_enum_value(options, 2, _o_formats);
	_curve = copts_enum_value(options, 3, _o_curves);
	if (argc == 1){
		argc = 2;
		argv[1] = _keyname;
	}

	if(0 == ecc_api_init()){
		char s[1024], *path;
		size_t len;
		path = cvstrncpy(s, sizeof(s), _outPath, "/", NULL);
		len = s+sizeof(s)-path;

		for (int i = 1; i < argc; i++){
			const char * keyname = argv[i];
			for (int count = 0; count < _certCount; count++){
				void * key = ecc_key_gen(_curve);
				if (key){
					char * e;
					if (_certCount == 1)
						e = cstrncpy(path, len, keyname);
					else
						e = path + sprintf(path, "%s_%03d", keyname, count + 1);
					cstrncpy(e, len - (e - path), ".vkey");
					ecc_key_private_save(key, s, _outFormat);
					cstrncpy(e, len - (e - path), "_pub.vkey");
					ecc_key_public_save(key, s, _outFormat);
					ecc_key_free(key);
				}
			}
		}
	}
	ecc_api_done();
	return 0;
}
