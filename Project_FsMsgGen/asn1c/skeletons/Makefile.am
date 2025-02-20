
CFLAGS = $(filter-out $(CODE_COVERAGE_CFLAGS), @CFLAGS@)

dist_pkgdata_DATA =				\
		${srcdir}/README		\
		${srcdir}/*.[ch]		\
		${srcdir}/file-dependencies

smodsdir=$(pkgdatadir)

nobase_dist_smods_DATA =     \
	standard-modules/README  \
	standard-modules/ASN1-Object-Identifier-Module.asn1 \
	standard-modules/ASN1C-UsefulInformationObjectClasses.asn1


uninstall-local:
	-@echo -n " "
	-rm -f -r $(DESTDIR)$(pkgdatadir)

check_LTLIBRARIES =             \
    libasn1cskeletons.la        \
    libasn1cskeletons_c89.la

libasn1cskeletons_c89_la_CFLAGS = $(SKELETONS_CFLAGS)
libasn1cskeletons_c89_la_SOURCES = $(libasn1cskeletons_la_SOURCES)

libasn1cskeletons_la_CFLAGS = $(TESTSUITE_CFLAGS)
libasn1cskeletons_la_SOURCES =                  \
    ANY.c ANY.h                                 \
    BIT_STRING.c BIT_STRING.h                   \
    BIT_STRING_oer.c                            \
    BMPString.c BMPString.h                     \
    BOOLEAN.c BOOLEAN.h                         \
    ENUMERATED.c ENUMERATED.h                   \
    GeneralString.c GeneralString.h             \
    GeneralizedTime.c GeneralizedTime.h         \
    GraphicString.c GraphicString.h             \
    IA5String.c IA5String.h                     \
    INTEGER.c INTEGER.h                         \
    INTEGER_oer.c                               \
    ISO646String.c ISO646String.h               \
    NULL.c NULL.h                               \
    NativeEnumerated.c NativeEnumerated.h       \
    NativeEnumerated_oer.c                      \
    NativeInteger.c NativeInteger.h             \
    NativeInteger_oer.c                         \
    NativeReal.c NativeReal.h                   \
    NumericString.c NumericString.h             \
    OBJECT_IDENTIFIER.c OBJECT_IDENTIFIER.h     \
    OCTET_STRING.c OCTET_STRING.h               \
    OCTET_STRING_oer.c                          \
    OPEN_TYPE.c OPEN_TYPE.h OPEN_TYPE_oer.c     \
    ObjectDescriptor.c ObjectDescriptor.h       \
    PrintableString.c PrintableString.h         \
    REAL.c REAL.h                               \
    RELATIVE-OID.c RELATIVE-OID.h               \
    T61String.c T61String.h                     \
    TeletexString.c TeletexString.h             \
    UTCTime.c UTCTime.h                         \
    UTF8String.c UTF8String.h                   \
    UniversalString.c UniversalString.h         \
    VideotexString.c VideotexString.h           \
    VisibleString.c VisibleString.h             \
    asn_SEQUENCE_OF.c asn_SEQUENCE_OF.h         \
    asn_SET_OF.c asn_SET_OF.h                   \
    asn_application.c asn_application.h         \
    asn_config.h                                \
    asn_system.h asn_codecs.h                   \
    asn_codecs_prim.c asn_codecs_prim.h         \
    asn_internal.h asn_internal.c               \
    asn_bit_data.c asn_bit_data.h               \
    asn_random_fill.c asn_random_fill.h         \
    ber_decoder.c ber_decoder.h                 \
    ber_tlv_length.c ber_tlv_length.h           \
    ber_tlv_tag.c ber_tlv_tag.h                 \
    constr_CHOICE.c constr_CHOICE.h             \
    constr_CHOICE_oer.c                         \
    constr_SEQUENCE.c constr_SEQUENCE.h         \
    constr_SEQUENCE_oer.c                       \
    constr_SEQUENCE_OF.c constr_SEQUENCE_OF.h   \
    constr_SET.c constr_SET.h                   \
    constr_SET_OF.c constr_SET_OF.h             \
    constr_SET_OF_oer.c                         \
    constr_TYPE.c constr_TYPE.h                 \
    constraints.c constraints.h                 \
    der_encoder.c der_encoder.h                 \
    oer_decoder.c oer_decoder.h                 \
    oer_encoder.c oer_encoder.h                 \
    oer_support.c oer_support.h                 \
    per_decoder.c per_decoder.h                 \
    per_encoder.c per_encoder.h                 \
    per_opentype.c per_opentype.h               \
    per_support.c per_support.h                 \
    aper_decoder.c aper_decoder.h               \
    aper_encoder.c aper_encoder.h               \
    aper_opentype.c aper_opentype.h             \
    aper_support.c aper_support.h               \
    uper_decoder.c uper_decoder.h               \
    uper_encoder.c uper_encoder.h               \
    uper_opentype.c uper_opentype.h             \
    uper_support.c uper_support.h               \
    xer_decoder.c xer_decoder.h                 \
    xer_encoder.c xer_encoder.h                 \
    jer_encoder.c jer_encoder.h                 \
    xer_support.c xer_support.h                 \
    ANY_aper.c                                  \
    ANY_ber.c                                   \
    ANY_uper.c                                  \
    ANY_xer.c                                   \
    ANY_jer.c                                   \
    BIT_STRING_print.c                          \
    BIT_STRING_rfill.c                          \
    BIT_STRING_uper.c                           \
    BIT_STRING_xer.c                            \
    BIT_STRING_jer.c                            \
    BMPString_print.c                           \
    BMPString_xer.c                             \
    BOOLEAN_aper.c                              \
    BOOLEAN_ber.c                               \
    BOOLEAN_oer.c                               \
    BOOLEAN_print.c                             \
    BOOLEAN_rfill.c                             \
    BOOLEAN_uper.c                              \
    BOOLEAN_xer.c                               \
    BOOLEAN_jer.c                               \
    ENUMERATED_aper.c                           \
    ENUMERATED_oer.c                            \
    ENUMERATED_uper.c                           \
    GeneralizedTime_ber.c                       \
    GeneralizedTime_print.c                     \
    GeneralizedTime_rfill.c                     \
    GeneralizedTime_xer.c                       \
    GeneralizedTime_jer.c                       \
    INTEGER_aper.c                              \
    INTEGER_ber.c                               \
    INTEGER_print.c                             \
    INTEGER_rfill.c                             \
    INTEGER_uper.c                              \
    INTEGER_xer.c                               \
    INTEGER_jer.c                               \
    NULL_aper.c                                 \
    NULL_ber.c                                  \
    NULL_oer.c                                  \
    NULL_print.c                                \
    NULL_rfill.c                                \
    NULL_uper.c                                 \
    NULL_xer.c                                  \
    NULL_jer.c                                  \
    NativeEnumerated_aper.c                     \
    NativeEnumerated_uper.c                     \
    NativeEnumerated_xer.c                      \
    NativeEnumerated_jer.c                      \
    NativeInteger_aper.c                        \
    NativeInteger_ber.c                         \
    NativeInteger_print.c                       \
    NativeInteger_rfill.c                       \
    NativeInteger_uper.c                        \
    NativeInteger_xer.c                         \
    NativeInteger_jer.c                         \
    NativeReal_aper.c                           \
    NativeReal_ber.c                            \
    NativeReal_oer.c                            \
    NativeReal_print.c                          \
    NativeReal_rfill.c                          \
    NativeReal_uper.c                           \
    NativeReal_xer.c                            \
    NativeReal_jer.c                            \
    OBJECT_IDENTIFIER_print.c                   \
    OBJECT_IDENTIFIER_rfill.c                   \
    OBJECT_IDENTIFIER_xer.c                     \
    OBJECT_IDENTIFIER_jer.c                     \
    OCTET_STRING_aper.c                         \
    OCTET_STRING_ber.c                          \
    OCTET_STRING_print.c                        \
    OCTET_STRING_rfill.c                        \
    OCTET_STRING_uper.c                         \
    OCTET_STRING_xer.c                          \
    OCTET_STRING_jer.c                          \
    OPEN_TYPE_aper.c                            \
    OPEN_TYPE_ber.c                             \
    OPEN_TYPE_uper.c                            \
    OPEN_TYPE_xer.c                             \
    REAL_aper.c                                 \
    REAL_oer.c                                  \
    REAL_print.c                                \
    REAL_rfill.c                                \
    REAL_uper.c                                 \
    REAL_xer.c                                  \
    REAL_jer.c                                  \
    RELATIVE-OID_print.c                        \
    RELATIVE-OID_rfill.c                        \
    RELATIVE-OID_xer.c                          \
    RELATIVE-OID_jer.c                          \
    UTCTime_print.c                             \
    UTCTime_rfill.c                             \
    UTCTime_xer.c                               \
    UTCTime_jer.c                               \
    UTF8String_print.c                          \
    UTF8String_rfill.c                          \
    UniversalString_print.c                     \
    UniversalString_xer.c                       \
    UniversalString_jer.c                       \
    asn_codecs_prim_ber.c                       \
    asn_codecs_prim_xer.c                       \
    constr_CHOICE_aper.c                        \
    constr_CHOICE_ber.c                         \
    constr_CHOICE_print.c                       \
    constr_CHOICE_rfill.c                       \
    constr_CHOICE_uper.c                        \
    constr_CHOICE_xer.c                         \
    constr_CHOICE_jer.c                         \
    constr_SEQUENCE_OF_aper.c                   \
    constr_SEQUENCE_OF_ber.c                    \
    constr_SEQUENCE_OF_uper.c                   \
    constr_SEQUENCE_OF_xer.c                    \
    constr_SEQUENCE_OF_jer.c                    \
    constr_SEQUENCE_aper.c                      \
    constr_SEQUENCE_ber.c                       \
    constr_SEQUENCE_print.c                     \
    constr_SEQUENCE_rfill.c                     \
    constr_SEQUENCE_uper.c                      \
    constr_SEQUENCE_xer.c                       \
    constr_SEQUENCE_jer.c                       \
    constr_SET_OF_aper.c                        \
    constr_SET_OF_ber.c                         \
    constr_SET_OF_print.c                       \
    constr_SET_OF_rfill.c                       \
    constr_SET_OF_uper.c                        \
    constr_SET_OF_xer.c                         \
    constr_SET_OF_jer.c                         \
    constr_SET_ber.c                            \
    constr_SET_print.c                          \
    constr_SET_rfill.c                          \
    constr_SET_xer.c                            \
    constr_SET_jer.c

check_PROGRAMS =                    \
    check-converter_example         \
    check-converter_c89_example
LDADD = -lm

check_converter_example_CFLAGS = $(TESTSUITE_CFLAGS) -DNO_ASN_PDU
check_converter_example_SOURCES = converter-example.c
check_converter_example_LDADD = libasn1cskeletons.la

check_converter_c89_example_CFLAGS = $(TESTSUITE_CFLAGS) -DNO_ASN_PDU
check_converter_c89_example_SOURCES = converter-example.c
check_converter_c89_example_LDADD = libasn1cskeletons_c89.la

if EXPLICIT_M32
check_LTLIBRARIES += libasn1cskeletons_c89_32.la
libasn1cskeletons_c89_32_la_CFLAGS = $(CFLAGS_M32) -Wno-format
libasn1cskeletons_c89_32_la_SOURCES = $(libasn1cskeletons_la_SOURCES)

check_PROGRAMS += check-converter_c89_32_example
check_converter_c89_32_example_CFLAGS = $(CFLAGS_M32) -DNO_ASN_PDU
check_converter_c89_32_example_SOURCES = converter-example.c
check_converter_c89_32_example_LDADD = libasn1cskeletons_c89_32.la
endif

