#include <assert.h>
#include <string.h>

#include "amxutil.h"
#include "amxext.h"
#include "amx.h"

#include "nrf_delay.h"

static const unsigned char test_script_data[252]=
{
  0xFC,0x00,0x00,0x00,0xE0,0xF1,0x0B,0x0B,0x04,0x00,0x08,
  0x00,0x6C,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,
  0x00,0x00,0xFC,0x01,0x00,0x00,0x04,0x00,0x00,0x00,0x3C,
  0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x4C,0x00,0x00,0x00,
  0x54,0x00,0x00,0x00,0x54,0x00,0x00,0x00,0x54,0x00,0x00,
  0x00,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x5D,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x65,0x00,0x00,0x00,0x1F,0x00,0x6C,0x63,
  0x64,0x5F,0x62,0x6C,0x00,0x77,0x61,0x69,0x74,0x5F,0x6D,
  0x73,0x00,0x65,0x78,0x74,0x00,0x00,0x00,0x00,0xAD,0x00,
  0x00,0x00,0x1E,0x00,0x00,0x00,0x8E,0x00,0x00,0x00,0x7B,
  0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0x01,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x6D,0x00,0x00,
  0x00,0xFC,0xFF,0xFF,0xFF,0x03,0x00,0x00,0x00,0xFC,0xFF,
  0xFF,0xFF,0x84,0x00,0x04,0x00,0x60,0x00,0x00,0x00,0x50,
  0x00,0x00,0x00,0x8E,0x00,0x01,0x00,0x70,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x8E,0x00,0xF4,
  0x01,0x70,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x04,0x00,
  0x00,0x00,0x8E,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x8E,0x00,0xF4,0x01,
  0x70,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x04,0x00,0x00,
  0x00,0x22,0x00,0x00,0x00,0xA4,0xFF,0xFF,0xFF,0x9C,0x00,
  0x04,0x00,0x66,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
};

int AMXAPI amxutil_freeProgram(AMX *amx)
{
  if (amx->base!=NULL) {
      amx_Cleanup(amx);
      free(amx->base);
      memset(amx, 0, sizeof(AMX));
  } /* if */
  return AMX_ERR_NONE;
}

void amxutil_runTestScript(void) {
    int err = AMX_ERR_NONE;
    AMX amx;
    
    memset(&amx, 0, sizeof amx);
  
    unsigned char script[252];
    memcpy(script, test_script_data, 252);
  
    err = amx_Init(&amx, script);
    
    amxext_init(&amx);
        
    cell ret;
    err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
        
    while (err == AMX_ERR_SLEEP)
    {
        nrf_delay_ms(10);
        err = amx_Exec(&amx, &ret, AMX_EXEC_CONT);
    }
    amxext_cleanup(&amx);
        
    amxutil_freeProgram(&amx);
}
