#ifndef _CANGATEWAY_H
#define _CANGATEWAY_H

#include <Can/Can/IfxCan_Can.h>
#define CAN_PORT_NUM 6
#define UDP_CONTENT_SIZE 82
#define CANGATEWAYDEBUG 1
boolean Shell_cangateway_init(pchar args, void *data, IfxStdIf_DPipe *io);
boolean Shell_cangateway_cangen(pchar args, void *data, IfxStdIf_DPipe *io);
void IfxMultican_init_with_interrupt(uint32 baudrate,uint8 filterMode);
void IfxMultican_initNode_with_interrupt(uint32 node, uint32 classic_baudrate, boolean is_fdmode, uint32 fd_baudrate, uint32 delay_offset, uint32 samplepoint_a, uint32 samplepoint_b);
void cansendtest(void);
extern volatile boolean cangen_enable;
//extern boolean caninit_flag;
extern boolean queue_dequeue_all_print_flag;

typedef struct queue_t queue_t;
typedef struct element_t element_t;
struct element_t {
    unsigned char udp_content[UDP_CONTENT_SIZE];
};

typedef struct round_trip_measure_s {
    uint64 point_b;
    uint64 point_c;
    uint64 point_d;
    uint64 point_e;
    boolean enabled;
    uint64 tx_target_cnt;
    uint64 tx_cnt;
    uint64 tx_start;
    uint64 tx_end;
} round_trip_measure_t;

struct queue_t {
    //unsigned int size;
    unsigned int capacity;
    unsigned int head;
    unsigned int tail;
    element_t *elements;
};

extern round_trip_measure_t rt_sample;

void queue_init(queue_t** p_can_queue);
queue_t* queue_new(int c);
void queue_delete(void *self);
boolean queue_is_empty(const queue_t *self);
element_t queue_peek(const queue_t *self);
boolean queue_enqueue(queue_t *self, element_t data);
element_t queue_dequeue(queue_t *self);
boolean queue_is_full(const queue_t *self);
unsigned int queue_get_size(const queue_t * self);
extern queue_t* g_can_queue[6];
extern void queue_dequeue_all(queue_t **pself);
extern void can_latency_measure(void);
#endif
