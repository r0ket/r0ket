/**************************************************************************/
/*! 
    @file     iap.c
    Source:   http://knowledgebase.nxp.com/showthread.php?t=1594
*/
/**************************************************************************/
#include "iap.h"
 
IAP_return_t iap_return;
 
#define IAP_ADDRESS 0x1FFF1FF1
uint32_t param_table[5];
 
/**************************************************************************/
/*! 
    Sends the IAP command and stores the result
*/
/**************************************************************************/
void iap_entry(uint32_t param_tab[], uint32_t result_tab[])
{
  void (*iap)(uint32_t[], uint32_t[]);
  iap = (void (*)(uint32_t[], uint32_t[]))IAP_ADDRESS;
  iap(param_tab,result_tab);
}

/**************************************************************************/
/*! 
    Returns the CPU's unique 128-bit serial number (4 words long)

    @section Example

    @code
    #include "core/iap/iap.h"

    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();

    if (iap_return.ReturnCode == 0)
    {
      printf("Serial Number: %08X %08X %08X %08X %s", 
              iap_return.Result[0],
              iap_return.Result[1],
              iap_return.Result[2],
              iap_return.Result[3], 
              CFG_PRINTF_NEWLINE);
    }
    @endcode
*/
/**************************************************************************/
IAP_return_t iapReadSerialNumber(void)
{
  // ToDo: Why does IAP sometime cause the application to halt when read???
  param_table[0] = IAP_CMD_READUID;
  iap_entry(param_table,(uint32_t*)(&iap_return));
  return iap_return;
}

