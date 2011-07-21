#define sspInit _hideaway_sspInit
#define sspSend _hideaway_sspSend
#define sspReceive _hideaway_sspReceive
#define sspSendReceive _hideaway_sspSendReceive

#include "../../../../firmware/core/ssp/ssp.c"

#undef sspInit
#undef sspSend
#undef sspReceive
#undef sspSendReceive

void sspInit (uint8_t portNum, sspClockPolarity_t polarity, sspClockPhase_t phase) {
}

void sspSend (uint8_t portNum, const uint8_t *buf, uint32_t length) {
}

void sspReceive (uint8_t portNum, uint8_t *buf, uint32_t length) {
}

void sspSendReceive(uint8_t portNum, uint8_t *buf, uint32_t length) {
}
