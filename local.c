/* Project #2; Azzam, Sahel */
/* Modified from CS5331 Lecture Slides - CSIM Mailboxes Example */

// #include "csim.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Mock CSIM types and constants
typedef long FACILITY;
typedef long MBOX;
typedef long TABLE;
typedef double TIME;
typedef long EVENT;
#define NIL ((msg_t)0)
#define TIMED_OUT 0
#define EVENT_OCCURRED 1

// Mock global variables
TIME sim_clock = 0.0;

/* Defines from slide */
#define TRACE
#define SIMTIME 1000.0
#define NUM_NODES 5L
#define TIME_OUT 2.5
#define T_DELAY 0.1
#define TRANS_TIME 0.2
#define INTER_ARRIVAL_MEAN 5.0
#define HELLO 1L
#define HELLO_ACK 2L

// FACILITY f;

typedef struct msg *msg_t;
struct msg {
    long type;
    long from;
    long to;
    TIME start_time;
    msg_t link;
    long retry_count;
};

/* Node structure */
struct nde {
    FACILITY cpu;
    MBOX input;
};

struct nde node[NUM_NODES];
FACILITY network[NUM_NODES][NUM_NODES];
TABLE resp_tm;
FILE *fp;

double current_loss_prob = 0.0;
long total_hello_count = 0;
long successful_transmissions = 0;
long failed_transmissions = 0;
long event_count = 0;
int show_events = 0;

// Mock CSIM functions
void create(char *name) { }
FACILITY facility(char *name) { return 1; }
MBOX mailbox(char *name) { return 1; }
void hold(double time) { sim_clock += time; }
void use(FACILITY f, double time) { sim_clock += time; }
void reserve(FACILITY f) { }
void release(FACILITY f) { }
void set_output_file(FILE *fp) { }
void max_facilities(long n) { }
void max_servers(long n) { }
void max_mailboxes(long n) { }
void max_events(long n) { }
TABLE table(char *name) { return 1; }
double exponential(double mean) { return -mean * log((double)rand() / RAND_MAX); }
long sim_random(long min, long max) { return min + rand() % (max - min + 1); }
double uniform01() { return (double)rand() / RAND_MAX; }
void *do_malloc(size_t size) { return malloc(size); }

// Define macros to use sim_ versions
#define clock sim_clock
#define random sim_random

// Mock message passing - simplified for local testing
long timed_receive(MBOX mbox, long *msg_id, double timeout) {
    // Simulate timeout based on current loss probability
    double timeout_prob = 0.2 + current_loss_prob; // Higher loss = more timeouts
    if (uniform01() < timeout_prob) {
        *msg_id = 0;
        return TIMED_OUT;
    }
    
    // Create a mock Hello_Ack message for successful case
    msg_t mock_msg = (msg_t)malloc(sizeof(struct msg));
    mock_msg->type = HELLO_ACK;
    mock_msg->from = sim_random(0, NUM_NODES - 1);
    mock_msg->to = sim_random(0, NUM_NODES - 1);
    mock_msg->start_time = sim_clock;
    mock_msg->retry_count = 0;
    *msg_id = (long)mock_msg;
    return EVENT_OCCURRED;
}

void send(MBOX mbox, long msg_id) {
    // Simulate network delay
    sim_clock += 0.1;
}

/* function declarations */
void init();
void my_report();
void proc(long n);
void send_msg(msg_t m);
void form_ack(msg_t m);
void decode_msg(char *str, msg_t m, long n);
void return_msg(msg_t m);
msg_t new_hello_msg(long from);
void reset_counters();
void run_simulation(double loss_prob);
msg_t msg_queue;


/* main simulation function */
void sim()
{
    create("sim");
    init();
    hold(SIMTIME);
    my_report();
}

/* initialization function */
void init()
{
    long i, j;
    char str[24];
    
    printf("TRACE: init() started\n");
    
    fp = fopen("xxx.out", "w");
    set_output_file(fp);
    max_facilities(NUM_NODES*NUM_NODES+NUM_NODES);
    max_servers(NUM_NODES*NUM_NODES+NUM_NODES);
    max_mailboxes(NUM_NODES);
    max_events(2*NUM_NODES*NUM_NODES);
    resp_tm = table("msg rsp tm");
    msg_queue = NIL;
    
    printf("TRACE: creating node facilities and mailboxes\n");
    for(i = 0; i < NUM_NODES; i++) 
    {
        sprintf(str, "cpu.%ld", i);
        node[i].cpu = facility(str);
        sprintf(str, "input.%ld", i);
        node[i].input = mailbox(str);
        printf("TRACE: created node %ld facilities\n", i);
    }
    
    printf("TRACE: creating network facilities\n");
    for(i = 0; i < NUM_NODES; i++) 
    {
        for(j = 0; j < NUM_NODES; j++) 
        {
            sprintf(str, "net%ld.%ld", i, j);
            network[i][j] = facility(str);
        }
    }
    
    printf("TRACE: starting processes\n");
    for(i = 0; i < NUM_NODES; i++) 
    {
        printf("TRACE: starting proc(%ld)\n", i);
        proc(i);
    }
    printf("TRACE: init() completed\n");
}



/* process function - INCOMPLETE */
void proc(long n)
{
    msg_t m;
    long s, t;
    long msg_id;
    
    create("proc");
    printf("TRACE: proc %ld started\n", n);
    
    while(clock < SIMTIME) 
    {
        printf("TRACE: proc %ld - loop start, clock=%.1f\n", n, clock);

        // s = timed_receive(node[n].input, &m, TIME_OUT);
        hold(exponential(INTER_ARRIVAL_MEAN));
        printf("TRACE: proc %ld - after exponential hold\n", n);

        if (clock >= SIMTIME)
        {
            printf("TRACE: proc %ld - time limit reached\n", n);
            break;
        }

        printf("TRACE: proc %ld - creating new hello msg\n", n);
        m = new_hello_msg(n);
        printf("TRACE: proc %ld - hello msg created, sending to %ld\n", n, m->to);
        total_hello_count++;

        if (show_events && event_count < 50)
        {
            printf("%.1f node.%ld sends a Hello Message to node.%ld\n", clock, n, m->to);
            event_count++;
        }

        printf("TRACE: proc %ld - about to send_msg\n", n);
        send_msg(m);
        printf("TRACE: proc %ld - sent msg, now waiting for response\n", n);
        
        s = timed_receive(node[n].input, &msg_id, TIME_OUT);
        printf("TRACE: proc %ld - timed_receive returned %ld, msg_id=%ld\n", n, s, msg_id);
        
        if (msg_id != 0) 
        {
            m = (msg_t)msg_id;
            printf("TRACE: proc %ld - converted msg_id to pointer\n", n);
        } 
        else 
        {
            printf("TRACE: proc %ld - msg_id is 0, creating dummy msg\n", n);
            m = new_hello_msg(n); // Create dummy for timeout case
        }

        switch(s) 
        {
            case TIMED_OUT:
            printf("TRACE: proc %ld - TIMED_OUT case\n", n);

            if (show_events && event_count < 50)
            {
                printf("%.1f node.%ld re-sends a Hello Message to node.%ld\n", clock, n, m->to);
                event_count++;
            }

            m->retry_count = 1;
            printf("TRACE: proc %ld - about to resend msg\n", n);
            send_msg(m);

            s = timed_receive(node[n].input, &msg_id, TIME_OUT);
            printf("TRACE: proc %ld - second timed_receive returned %ld\n", n, s);
            
            if (msg_id != 0) {
                m = (msg_t)msg_id;
            }
            
            if (s == TIMED_OUT)
            {
                printf("TRACE: proc %ld - second timeout, failing\n", n);
                failed_transmissions++;
                return_msg(m);
            }

            else if (s == EVENT_OCCURRED && m->type == HELLO_ACK)
            {
                printf("TRACE: proc %ld - got HELLO_ACK after retry\n", n);
                successful_transmissions++;
                if (show_events && event_count < 50)
                {
                    printf("%.1f node.%ld receives a Hello_Ack Message from node.%ld\n", clock, n, m->from);
                    event_count++;
                }
                return_msg(m);
            }
            break;

            case EVENT_OCCURRED:
            printf("TRACE: proc %ld - EVENT_OCCURRED case, msg type=%ld\n", n, m->type);
            t = m->type;
            switch(t)
            {
                case HELLO:
                printf("TRACE: proc %ld - processing HELLO\n", n);
                use(node[n].cpu, T_DELAY);
                form_ack(m);
                if (show_events && event_count < 50)
                {
                    printf("%.1f node.%ld replies a Hello_Ack Message to node.%ld\n", clock, n, m->to);
                    event_count++;   
                }
                send_msg(m);
                break;

                case HELLO_ACK:
                printf("TRACE: proc %ld - processing HELLO_ACK\n", n);
                successful_transmissions++;
                if (show_events && event_count < 50)
                {
                    printf("%.1f node.%ld receives a Hello_Ack Message from node.%ld\n", clock, n, m->from);
                    event_count++;
                }
                return_msg(m);
                break;

                default:
                    printf("%.1f node.%ld: ***unexpected message type\n", clock, n);
                    return_msg(m);
                    break;
            }
            break;
        }
        printf("TRACE: proc %ld - end of loop iteration\n", n);
    }
    printf("TRACE: proc %ld - exiting\n", n);
}

/* send message function */
void send_msg(msg_t m)
{
    long from, to;
    
    from = m->from;
    to = m->to;

if (uniform01() < current_loss_prob)
{
    if (show_events && event_count < 50)
    {
        printf("%.1f Packet lost: %s from node.%ld to node.%ld\n", clock, (m->type == HELLO ) ? "Hello" : "Hello_Ack", from, to);
        event_count++;
    }
    return_msg(m);
    return;
}

    use(node[from].cpu, T_DELAY);
    reserve(network[from][to]);
        hold(TRANS_TIME);
        send(node[to].input, (long)m);
    release(network[from][to]);
}

/* new message function - INCOMPLETE */
msg_t new_hello_msg(long from)
{
    msg_t m;    
    long i;
    
    if(msg_queue == NIL) 
    {
        m = (msg_t)malloc(sizeof(struct msg));
    } 
    else 
    {
        m = msg_queue;
        msg_queue = msg_queue->link;
    }
    
    do 
    {
        i = random(0, NUM_NODES - 1);
    } while(i == from);
    m->from = from;
    m->to = i;
    m->type = HELLO;
    m->start_time = clock;
    m->retry_count = 0;
    return(m);
}

/* return message function */
void return_msg(msg_t m)
{
    m->link = msg_queue;
    msg_queue = m;
}

/* form reply function */
void form_ack(msg_t m)
{
    long from, to;
    
    from = m->from;
    to = m->to;
    m->from = to;
    m->to = from;
    m->type = HELLO_ACK;
}

/* decode message function - shows printf format but incomplete */
void decode_msg(char *str, msg_t m, long n)
{
    printf("%6.3f node %2ld: %s - msg: type = %s, from = %ld, to = %ld\n", clock, n, str, (m->type == HELLO) ? "Hello" : "Hello_Ack", m->from, m->to);
}

/* reset counters for new simulation run */
void reset_counters()
{
    total_hello_count = 0;
    successful_transmissions = 0;
    failed_transmissions = 0;
    event_count = 0;
}

/* report function for generating final statistics */
void my_report()
{
    double success_percent = 0.0;
    double failure_percent = 0.0;
    
    if(total_hello_count > 0) 
    {
        success_percent = (double)successful_transmissions / total_hello_count * 100.0;
        failure_percent = (double)failed_transmissions / total_hello_count * 100.0;
    }
    
    printf("\n=== Simulation Results ===\n");
    printf("Loss Probability: %.1f\n", current_loss_prob);
    printf("Total Hello Attempts: %ld\n", total_hello_count);
    printf("Successful Transmissions: %ld (%.2f%%)\n", successful_transmissions, success_percent);
    printf("Failed Transmissions: %ld (%.2f%%)\n", failed_transmissions, failure_percent);
    printf("==========================\n\n");
}

/* Run single simulation with specified loss probability */
void run_simulation(double loss_prob)
{
    printf("TRACE: run_simulation(%.1f) started\n", loss_prob);
    current_loss_prob = loss_prob;
    show_events = (loss_prob == 0.3);  // Only show events for loss_prob = 0.3
    
    printf("\n--- Running simulation with loss probability %.1f ---\n", loss_prob);
    if(show_events) 
    {
        printf("Event snapshot (max 50 events):\n");
    }
    
    printf("TRACE: about to reset_counters\n");
    reset_counters();
    printf("TRACE: calling sim() directly (no CSIM reset)\n");
    sim();
    printf("TRACE: sim() completed\n");
}

/* Main function */
int main()
{
    double loss_probabilities[] = {0.1, 0.2, 0.3, 0.4, 0.5};
    int num_sims = 5;
    int i;
    
    // Seed random number generator for local testing
    srand(time(NULL));
    
    printf("Project #2: Heartbeat Protocol Simulation (LOCAL TEST)\n");
    printf("==========================================\n");
    
    printf("\nRunning simulations for all loss probabilities...\n");
    
    for(i = 0; i < num_sims; i++) 
    {
        run_simulation(loss_probabilities[i]);
    }
    
    printf("\nAll simulations completed. Use the results above to create your graphs.\n");
    printf("Graph 1: Percentage of successful transmissions vs. packet loss probability\n");
    printf("Graph 2: Percentage of failed transmissions vs. packet loss probability\n");
    
    return 0;
} 