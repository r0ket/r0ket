#include <stdint.h>
#include "random.h"
#include "xxtea.h"

#define STATE_SIZE  8
uint32_t state[STATE_SIZE];
uint32_t const I[4] = {12,13,14,15};

void randomInit(void)
{
    uint32_t i,j,x;
    for(j=0; j<STATE_SIZE; j++){
        x = 0;
        for(i=0; i<10240; i++){
            x ^= (adcRead(1)&1)<<(i%32);
        }
        state[j] = x;
    }
    xxtea_encode_words(state, STATE_SIZE, I);
}

uint32_t random(void)
{
    xxtea_encode_words(state, STATE_SIZE, I);
    return state[0];
}
