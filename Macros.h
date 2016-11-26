#define MAX_NEURON_PL 30
#define MIN_LAYER_CNT 4
#define LAYER_CNT 4
#define CONNECTION_JUMP 2
#define PREDEFINED_THRESHOLD 1.15
#define FADETIME_PER_CLOCK 0.000001
#define BUFFERSIZE 256
#define CNTRL_SEND_FIFO "/tmp/cntrl_send_fifo"
#define CNTRL_ASK_FIFO "/tmp/cntrl_ask_fifo"

#ifndef NET_LANGUAGE
#define NET_LANGUAGE

enum NetLanguage : int {
    NET = 0,
    LAYER = 1,
    NEURON = 2,
    CONNECTION = 3,
    CMD_FIRE = 4
};


#endif
