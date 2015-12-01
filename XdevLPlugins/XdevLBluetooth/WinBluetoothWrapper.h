#ifndef WIN_BLUETOOTH_WRAPPER_H
#define WIN_BLUETOOTH_WRAPPER_H

typedef ULONGLONG BTH_ADDR;


#ifndef AF_BTH
#define AF_BTH  32
#endif
#ifndef PF_BTH
#define PF_BTH  AF_BTH
#endif
#ifndef NS_BTH
#define NS_BTH  16
#endif

#define BTHPROTO_RFCOMM  0x0003
#define BTHPROTO_L2CAP   0x0100

#define SOL_RFCOMM  BTHPROTO_RFCOMM
#define SOL_L2CAP   BTHPROTO_L2CAP
#define SOL_SDP     0x0101

#define SO_BTH_AUTHENTICATE 0x80000001
#define SO_BTH_ENCRYPT      0x00000002
#define SO_BTH_MTU          0x80000007
#define SO_BTH_MTU_MAX      0x80000008
#define SO_BTH_MTU_MIN      0x8000000a

#define BT_PORT_ANY        ((ULONG)-1)
#define BT_PORT_MIN        0x1
#define BT_PORT_MAX        0xffff
#define BT_PORT_DYN_FIRST  0x1001

#define BLUETOOTH_MAX_NAME_SIZE             (248)
#define BLUETOOTH_MAX_PASSKEY_SIZE          (16)
#define BLUETOOTH_MAX_PASSKEY_BUFFER_SIZE   (BLUETOOTH_MAX_PASSKEY_SIZE + 1)
#define BLUETOOTH_MAX_SERVICE_NAME_SIZE     (256)
#define BLUETOOTH_DEVICE_NAME_SIZE          (256)

typedef HANDLE      HBLUETOOTH_DEVICE_FIND;
// ***************************************************************************
//
//  Device Enumeration
//
//  Description:
//      Enumerates the Bluetooth devices. The types of returned device depends
//      on the flags set in the BLUETOOTH_DEVICE_SEARCH_PARAMS (see structure
//      definition for details).
//
//  Sample Usage:
//      HBLUETOOTH_DEVICE_FIND hFind;
//      BLUETOOTH_DEVICE_SEARCH_PARAMS btsp = { sizeof(btsp) };
//      BLUETOOTH_DEVICE_INFO btdi = { sizeof(btdi) };
//
//      btsp.fReturnAuthenticated = TRUE;
//      btsp.fReturnRemembered    = TRUE;
//
//      hFind = BluetoothFindFirstDevice( &btsp, &btdi );
//      if ( NULL != hFind )
//      {
//          do
//          {
//              //
//              //  TODO:   Do something useful with the device info.
//              //
//
//          } while( BluetoothFindNextDevice( hFind, &btdi ) );
//
//          BluetoothFindDeviceClose( hFind );
//      }
//
// ***************************************************************************

typedef struct _BLUETOOTH_DEVICE_SEARCH_PARAMS {
    DWORD   dwSize;                 //  IN  sizeof this structure

    BOOL    fReturnAuthenticated;   //  IN  return authenticated devices
    BOOL    fReturnRemembered;      //  IN  return remembered devices
    BOOL    fReturnUnknown;         //  IN  return unknown devices
    BOOL    fReturnConnected;       //  IN  return connected devices

    BOOL    fIssueInquiry;          //  IN  issue a new inquiry
    UCHAR   cTimeoutMultiplier;     //  IN  timeout for the inquiry

    HANDLE  hRadio;                 //  IN  handle to radio to enumerate - NULL == all radios will be searched

} BLUETOOTH_DEVICE_SEARCH_PARAMS;

typedef HANDLE      HBLUETOOTH_DEVICE_FIND;



typedef struct _SOCKADDR_BTH
{
    USHORT      addressFamily; 
    BTH_ADDR    btAddr;        
    GUID        serviceClassId;
    ULONG       port;          
} SOCKADDR_BTH, *PSOCKADDR_BTH;

DEFINE_GUID(RFCOMM_PROTOCOL_UUID, 0x00000003, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB);

typedef struct _BLUETOOTH_ADDRESS {
    union {
        BTH_ADDR ullLong;       //  easier to compare again BLUETOOTH_NULL_ADDRESS
        BYTE    rgBytes[ 6 ];   //  easier to format when broken out
    };

} BLUETOOTH_ADDRESS_STRUCT;

#define BLUETOOTH_ADDRESS BLUETOOTH_ADDRESS_STRUCT


// ***************************************************************************
//
//  Device Information Stuctures
//
// ***************************************************************************

typedef struct _BLUETOOTH_DEVICE_INFO {
    DWORD   dwSize;                             //  size, in bytes, of this structure - must be the sizeof(BLUETOOTH_DEVICE_INFO)

    BLUETOOTH_ADDRESS Address;                  //  Bluetooth address

    ULONG   ulClassofDevice;                    //  Bluetooth "Class of Device"

    BOOL    fConnected;                         //  Device connected/in use
    BOOL    fRemembered;                        //  Device remembered
    BOOL    fAuthenticated;                     //  Device authenticated/paired/bonded

    SYSTEMTIME  stLastSeen;                     //  Last time the device was seen
    SYSTEMTIME  stLastUsed;                     //  Last time the device was used for other than RNR, inquiry, or SDP

    WCHAR   szName[ BLUETOOTH_MAX_NAME_SIZE ];  //  Name of the device

} BLUETOOTH_DEVICE_INFO_STRUCT;

#define BLUETOOTH_DEVICE_INFO BLUETOOTH_DEVICE_INFO_STRUCT

HBLUETOOTH_DEVICE_FIND
WINAPI
BluetoothFindFirstDevice(
     const   BLUETOOTH_DEVICE_SEARCH_PARAMS * pbtsp,
    BLUETOOTH_DEVICE_INFO *   pbtdi
    );

  typedef struct __bd_addr {
  union {
    struct {
      unsigned int LAP : 24; 
      unsigned int UAP : 8; 
    };
  unsigned int SAP; 
  };
  unsigned short  NAP; 
  } BD_ADDR;

typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

#endif
