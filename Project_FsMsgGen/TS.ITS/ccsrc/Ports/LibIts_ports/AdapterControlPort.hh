
//#include "TRI_mapper_API.hh"

#ifdef _NO_SOFTLINKS_
/* to be used when built without generating softlinks to the working directory */
//#include "CALM_ports/AdapterControlPort_CALM.partH"
#ifdef LibItsCam__TestSystem_HH
#include "CAM_ports/AdapterControlPort_CAM.hh"
#endif
#ifdef LibItsDenm__TestSystem_HH
#include "DENM_ports/AdapterControlPort_DENM.hh"
#endif
#ifdef LibItsGeoNetworking__TestSystem_HH
#include "GN_ports/AdapterControlPort_GN.hh"
#endif
#ifdef LibItsIvim__TestSystem_HH
#include "IVIM_ports/AdapterControlPort_IVIM.hh"
#endif
#ifdef LibItsMapemSpatem__TestSystem_HH
#include "MapemSpatem_ports/AdapterControlPort_MapemSpatem.hh"
#endif
#ifdef LibItsSremSsem__TestSystem_HH
#include "SremSsem_ports/AdapterControlPort_SremSsem.hh"
#endif
#ifdef LibItsRtcmem__TestSystem_HH
#include "Rtcmem_ports/AdapterControlPort_Rtcmem.hh"
#endif
#ifdef LibItsV2G__TestSystem_HH
#include "V2G_ports/AdapterControlPort_V2G.hh"
#endif
//#ifdef LibItsPki__TestSystem_HH
//#include "Pki_ports/AdapterControlPort_Pki.hh"
//#endif

#else //_NO_SOFTLINKS_

#ifdef LibItsCam__TestSystem_HH
#include "AdapterControlPort_CAM.hh"
#endif
#ifdef LibItsDenm__TestSystem_HH
#include "AdapterControlPort_DENM.hh"
#endif
#ifdef LibItsIvim__TestSystem_HH
#include "AdapterControlPort_IVIM.hh"
#endif
#ifdef LibItsMapemSpatem__TestSystem_HH
#include "AdapterControlPort_MapemSpatem.hh"
#endif
#ifdef LibItsSremSsem__TestSystem_HH
#include "AdapterControlPort_SremSsem.hh"
#endif
#ifdef LibItsRtcmem__TestSystem_HH
#include "AdapterControlPort_Rtcmem.hh"
#endif
#ifdef LibItsGeoNetworking__TestSystem_HH
#include "AdapterControlPort_GN.hh"
#endif
/*
#include "AdapterControlPort_Evcsn.partH"
#include "AdapterControlPort_IVIM.partH"
#include "AdapterControlPort_MapemSpatem.partH"
#include "AdapterControlPort_SremSsem.partH"
*/
//#include "AdapterControlPort_V2G.partH"
#endif //_NO_SOFTLINKS_
