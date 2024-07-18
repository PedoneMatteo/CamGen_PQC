#ifndef UpperTesterPort_HH
#define UpperTesterPort_HH

#ifdef _NO_SOFTLINKS_
/* to be used when built without generating softlinks to the working directory */

#ifdef AtsGeoNetworking
#include "GN_ports/UpperTesterPort_GN.hh"
#endif

#ifdef AtsBTP
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#endif

#ifdef AtsCAM
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "CAM_ports/UpperTesterPort_CAM.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#endif

#ifdef AtsDENM
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "DENM_ports/UpperTesterPort_DENM.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#endif

#ifdef AtsSecurity
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "CAM_ports/UpperTesterPort_CAM.hh"
#include "DENM_ports/UpperTesterPort_DENM.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#endif

#ifdef AtsPki
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#ifndef LibItsGeoNetworking__TestSystem_HH
#include "Pki_ports/UpperTesterPkiPort.hh"
#endif
#endif

#ifdef AtsIS
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#include "IVIM_ports/UpperTesterPort_IVIM.hh"
#include "MapemSpatem_ports/UpperTesterPort_MapemSpatem.hh"
#include "Rtcmem_ports/UpperTesterPort_Rtcmem.hh"
#include "SremSsem_ports/UpperTesterPort_SremSsem.hh"
#endif

#ifdef AtsIPv6OverGeoNetworking
#include "GN_ports/UpperTesterPort_GN.hh"
#endif

#ifdef AtsRSUsSimulator // This is not an Abstract Test Suit, but an helper tool to simulate basic RSU/OBU functionalities
#include "BTP_ports/UpperTesterPort_BTP.hh"
#include "CAM_ports/UpperTesterPort_CAM.hh"
#include "DENM_ports/UpperTesterPort_DENM.hh"
#include "GN_ports/UpperTesterPort_GN.hh"
#include "IVIM_ports/UpperTesterPort_IVIM.hh"
#include "MapemSpatem_ports/UpperTesterPort_MapemSpatem.hh"
#include "Rtcmem_ports/UpperTesterPort_Rtcmem.hh"
#include "SremSsem_ports/UpperTesterPort_SremSsem.hh"
#endif

#else //_NO_SOFTLINKS_

#ifdef AtsGeoNetworking
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsBTP
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsCAM
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_CAM.hh"
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsDENM
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_DENM.hh"
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsSecurity
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_CAM.hh"
#include "UpperTesterPort_DENM.hh"
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsPki
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_CAM.hh"
#include "UpperTesterPort_DENM.hh"
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsIS
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_GN.hh"
#include "UpperTesterPort_IVIM.hh"
#include "UpperTesterPort_MapemSpatem.hh"
#include "UpperTesterPort_Rtcmem.hh"
#include "UpperTesterPort_SremSsem.hh"
#endif

#ifdef AtsIPv6OverGeoNetworking
#include "UpperTesterPort_GN.hh"
#endif

#ifdef AtsRSUsSimulator // This is not an Abstract Test Suit, but an helper tool to simulate basic RSU/OBU functionalities
#include "UpperTesterPort_BTP.hh"
#include "UpperTesterPort_CAM.hh"
#include "UpperTesterPort_DENM.hh"
#include "UpperTesterPort_GN.hh"
#include "UpperTesterPort_IVIM.hh"
#include "UpperTesterPort_MapemSpatem.hh"
#include "UpperTesterPort_Rtcmem.hh"
#include "UpperTesterPort_SremSsem.hh"
#endif

#endif //_NO_SOFTLINKS_

#endif
