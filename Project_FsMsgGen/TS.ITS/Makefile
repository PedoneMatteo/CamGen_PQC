-include config.mk

ifeq (,$(ATS))
 $(error ATS shall be defined)
endif

ifeq (,$(TOPDIR))
 TOPDIR := .
endif

ifeq (,$(TTCN3_DIR))
 $(error TTCN3_DIR shall be defined in config.mk)
endif

export ASN1C_PATH

first: all

define IncludeModule
undefine sources
undefine modules
undefine includes
include $(1)/module.mk
$$(foreach S, $$(sources),  $$(eval all_sources +=  $$(if $$(filter /%, $$(S)), $$(TOPDIR)/$$(S), $(1)/$$(S))))
$$(foreach I, $$(includes), $$(eval all_includes += $$(if $$(filter /%, $$(I)), $$(TOPDIR)/$$(I), $(1)/$$(I))))
$$(foreach M, $$(modules),  $$(eval $$(call IncludeModule, $$(if $$(filter /%, $$(M)), $$(TOPDIR)/$$(M), $(1)/$$(M)))))
endef

all_includes := $(TTCN3_DIR)/include $(TTCN3_DIR)/src $(TOPDIR)/ccsrc/Framework /usr/include/jsoncpp
defines  += TITAN_RUNTIME_2 _NO_SOFTLINKS_ $(ATS)  AS_USE_SSL
libs     += $(TTCN3_DIR)/lib/libttcn3-rt2-parallel.a -lstdc++fs

ifeq (Windows_NT,$(OS))
# Silence linker warnings.
LDFLAGS += -Wl,--enable-auto-import,--enable-runtime-pseudo-reloc
defines += WIN32
libs += $(patsubst %, -L%/lib, $(OPENSSL_DIR))
all_includes += $(addsuffix /include, $(OPENSSL_DIR))
libs += $(WPCAP_DLL_PATH)
else
defines += LINUX
libs += -lpcap -lrt -lpthread
endif

libs += -lssl -lcrypto -lxml2 -ljsoncpp

$(eval $(call IncludeModule, $(TOPDIR)/ttcn/$(ATS)))

outdir := $(TOPDIR)/build/$(ATS)
bindir := $(TOPDIR)/bin

sources     := $(sort $(all_sources))
includes    := $(outdir) $(outdir)/.. $(outdir)/../asn1 $(all_includes) $(NPCAP_INCLUDE)

ifeq (Windows_NT,$(OS))
  EXE=.exe
endif

ttcn_sources  := $(filter %.ttcn , $(sources))
ttcn3_sources := $(filter %.ttcn3, $(sources))
asn_sources   := $(filter %.asn,   $(sources))
asn1_sources  := $(filter %.asn1,  $(sources))

tt_sources    := $(ttcn_sources) $(ttcn3_sources) $(asn_sources) $(asn1_sources)
cc_sources    := $(filter %.cc,    $(sources))

gen_ttcn_sources  := $(addprefix $(outdir)/, $(notdir $(patsubst %.ttcn,  %.cc, $(ttcn_sources))))
gen_ttcn_headers  := $(addprefix $(outdir)/, $(notdir $(patsubst %.ttcn,  %.hh, $(ttcn_sources))))
gen_ttcn3_sources := $(addprefix $(outdir)/, $(notdir $(patsubst %.ttcn3, %.cc, $(ttcn3_sources))))
gen_ttcn3_headers := $(addprefix $(outdir)/, $(notdir $(patsubst %.ttcn3, %.hh, $(ttcn3_sources))))
gen_asn_sources   := $(addprefix $(outdir)/, $(notdir $(patsubst %.asn,   %.cc, $(asn_sources))))
gen_asn_headers   := $(addprefix $(outdir)/, $(notdir $(patsubst %.asn,   %.hh, $(asn_sources))))
gen_asn1_sources  := $(addprefix $(outdir)/, $(notdir $(patsubst %.asn1,  %.cc, $(asn1_sources))))
gen_asn1_headers  := $(addprefix $(outdir)/, $(notdir $(patsubst %.asn1,  %.hh, $(asn1_sources))))

gen_sources := $(gen_ttcn_sources) $(gen_ttcn3_sources) $(gen_asn_sources) $(gen_asn1_sources)
gen_headers := $(gen_ttcn_headers) $(gen_ttcn3_headers) $(gen_asn_headers) $(gen_asn1_headers)

gen_objects := $(patsubst %.cc, %.o, $(gen_sources))
cc_objects  := $(patsubst  %.cc, $(outdir)/%.o, $(cc_sources))

.PHONY: all FORCE echo

all: $(bindir) $(outdir) $(bindir)/$(ATS)$(EXE)

echo_sources:
	@echo -e "$(addsuffix \n,$(all_sources))"

echo: echo_sources
	@echo -e "sources:\n $(addsuffix \n, $(sources))"
	@echo -e "gen_sources:\n $(addsuffix \n, $(gen_sources))"
	@echo -e "gen_objects:\n $(addsuffix \n, $(gen_objects))"
	@echo -e "cc_objects:\n $(addsuffix \n, $(cc_objects))"
	@echo -e "includes:\n $(addsuffix \n, $(includes))"
	@echo -e "defines: $(addsuffix \n, $(defines))"

clean:
	rm -f $(outdir)/$(ATS)$(EXE) $(gen_objects) $(gen_sources) $(gen_headers) $(cc_objects) $(outdir)/.generate $(outdir)/../libItsAsn.so
#	rm -f $(outdir)/asn1/*.so $(outdir)/asn1/*.o $(outdir)/asn1/*.c $(outdir)/asn1/*.h $(outdir)/asn1/Makefile.am.libasncodec

regen: force_regen $(outdir)/.generate
force_regen:
	rm -f $(outdir)/.generate

t3q: all
	@echo -e "Code checking in progress..."
	@java -Xmx3g -Xss512m -jar $(T3Q_PATH)/t3q.jar --config $(T3Q_PATH)/$(T3Q_CONFIG) $(tt_sources) > $(outdir)/t3q.out 2>&1 ; cd -
	@echo -e "Code checking in done..."
	@echo -e "Output file is located here: $(outdir)/t3q.out"

t3d: all
	@echo -e "TTCN-3 doc generation in progress..."
	@java -Xmx3g -Xss512m -jar $(T3D_PATH)/t3d.jar --config $(T3D_PATH)/$(T3D_CONFIG) $(tt_sources) > $(outdir)/t3d.out 2>&1 ; cd -
	@echo -e "TTCN-3 doc generation done..."
	@echo -e "Output file is located here: $(outdir)/t3d.out"

$(outdir) $(bindir):
	mkdir -p $@

$(bindir)/$(ATS)$(EXE): $(outdir)/../asn1/libItsAsn.a $(gen_objects) $(cc_objects) 
	g++ -g -O0 -o $@ $(LDFLAGS) $(gen_objects) $(cc_objects) $(outdir)/../asn1/libItsAsn.a $(libs)

$(gen_objects) :%.o :%.cc
	g++ -g -O0 -c $(addprefix -D, $(defines)) $(addprefix -I, $(includes)) -o $@ $<

$(cc_objects) : $(outdir)/%.o : %.cc
	mkdir -p $(dir $@)
	g++ -g -O0 -c $(addprefix -D, $(defines)) $(addprefix -I, $(includes)) -o $@ $<

#$(gen_sources):
#	$(TTCN3_DIR)/bin/compiler -n -e -L -R -U none -o $(outdir) $(tt_sources)

$(gen_sources): $(outdir)/.generate

$(outdir)/.generate: Makefile $(tt_sources)
	$(TTCN3_DIR)/bin/compiler -w -d -e -f -g -l -L -R -U none -x -X  -o  $(outdir) $(tt_sources)
	touch $@

$(bindir)/libItsAsn.so: $(outdir)/asn1/libItsAsn.so
	cp -f $< $@

$(outdir)/../asn1/libItsAsn.a: FORCE
	mkdir -p $(outdir)/../asn1
	$(MAKE) -C $(outdir)/../asn1 -f ../../asn1/Makefile

$(foreach S, $(ttcn_sources),  $(eval $(outdir)/$(notdir $(patsubst %.ttcn, %.cc,  $(S))): $(S)))
$(foreach S, $(ttcn3_sources), $(eval $(outdir)/$(notdir $(patsubst %.ttcn3, %.cc, $(S))): $(S)))
$(foreach S, $(asn_sources),   $(eval $(outdir)/$(notdir $(patsubst %.asn, %.cc,   $(S))): $(S)))
$(foreach S, $(asn1_sources),  $(eval $(outdir)/$(notdir $(patsubst %.asn1, %.cc,  $(S))): $(S)))

