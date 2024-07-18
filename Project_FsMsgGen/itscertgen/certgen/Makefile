######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2015
##  This code is provided under the MIT license agreement.
######################################################################
PROJECTROOT      = ..
#BUILDROOT        = $(PROJECTROOT)/build
CSHAREDDIR       = $(PROJECTROOT)/cshared
PROJECT          = certgen
DEBUG            = yes
bins             = certgen keygen certidx
sources         := ecc_openssl.c mkgmtime.c
sources-certgen := certgen.c
sources-keygen  := keygen.c
sources-certidx := certidx.c cert_utils.c
cflags          += -Wno-pointer-sign -Wno-enum-compare
sources-WIN32   := applink.c
#sources-CYGWIN := applink.c
packages        += cshared openssl
predirs         := asncodec
includes        += asncodec
libs            += $(outdir)/libItsCertAsn.a
include $(CSHAREDDIR)/common.mk
