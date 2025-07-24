/* Project #2; Azzam, Sahel */
/* Modified from CS5331 Lecture Slides - CSIM Mailboxes Example */

#include "csim.h"

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
    
    printf("TRACE: init() - starting initialization\n");
    fp = fopen("xxx.out", "w");
    set_output_file(fp);
    max_facilities(NUM_NODES*NUM_NODES+NUM_NODES);
    max_servers(NUM_NODES*NUM_NODES+NUM_NODES);
    max_mailboxes(NUM_NODES);
    max_events(2*NUM_NODES*NUM_NODES);
    resp_tm = table("msg rsp tm");
    msg_queue = NIL;
    printf("TRACE: init() - CSIM setup completed\n");
    
    printf("TRACE: init() - creating node facilities and mailboxes\n");
    for(i = 0; i < NUM_NODES; i++) 
    {
        sprintf(str, "cpu.%ld", i);
        node[i].cpu = facility(str);
        sprintf(str, "input.%ld", i);
        node[i].input = mailbox(str);
        printf("TRACE: init() - created facilities for node %ld\n", i);
    }
    
    printf("TRACE: init() - creating network facilities\n");
    for(i = 0; i < NUM_NODES; i++) 
    {
        for(j = 0; j < NUM_NODES; j++) 
        {
            sprintf(str, "net%ld.%ld", i, j);
            network[i][j] = facility(str);
        }
    }
    printf("TRACE: init() - network facilities created\n");
    
    printf("TRACE: init() - starting processes\n");
    for(i = 0; i < NUM_NODES; i++) 
    {
        printf("TRACE: init() - about to start proc(%ld)\n", i);
        proc(i);
        printf("TRACE: init() - proc(%ld) started\n", i);
    }
    printf("TRACE: init() - all processes started, init completed\n");
}



/* process function - SIMPLIFIED TEST VERSION */
void proc(long n)
{
    msg_t m;
    long s, t;
    long msg_id;
    
    printf("TRACE: proc(%ld) - starting, creating process\n", n);
    create("proc");
    printf("TRACE: proc(%ld) - process created successfully\n", n);
    
    // Only node 0 sends one Hello message for testing
    if (n == 0) 
    {
        printf("TRACE: proc(%ld) - node 0 will send one Hello message\n", n);
        
        // Small delay before sending
        printf("TRACE: proc(%ld) - holding for 1.0 seconds\n", n);
        hold(1.0);
        printf("TRACE: proc(%ld) - hold completed, clock=%.1f\n", n, clock);
        
        // Create and send one Hello message
        printf("TRACE: proc(%ld) - creating Hello message\n", n);
        m = new_hello_msg(n);
        printf("TRACE: proc(%ld) - Hello message created, sending to node %ld\n", n, m->to);
        
        total_hello_count++;
        printf("TRACE: proc(%ld) - about to call send_msg\n", n);
        send_msg(m);
        printf("TRACE: proc(%ld) - send_msg completed\n", n);
        
        printf("TRACE: proc(%ld) - waiting for response with timed_receive\n", n);
        s = timed_receive(node[n].input, &msg_id, TIME_OUT);
        printf("TRACE: proc(%ld) - timed_receive returned s=%ld, msg_id=%ld\n", n, s, msg_id);
        
        if (s == EVENT_OCCURRED && msg_id != 0) 
        {
            printf("TRACE: proc(%ld) - received Hello_Ack successfully\n", n);
            m = (msg_t)msg_id;
            successful_transmissions++;
            return_msg(m);
        }
        else 
        {
            printf("TRACE: proc(%ld) - no response received (timeout)\n", n);
            failed_transmissions++;
        }
    }
    else 
    {
        printf("TRACE: proc(%ld) - waiting for incoming messages\n", n);
        
        // Other nodes just wait for incoming Hello messages
        s = timed_receive(node[n].input, &msg_id, 10.0); // Wait up to 10 seconds
        printf("TRACE: proc(%ld) - timed_receive returned s=%ld, msg_id=%ld\n", n, s, msg_id);
        
        if (s == EVENT_OCCURRED && msg_id != 0) 
        {
            printf("TRACE: proc(%ld) - received a message\n", n);
            m = (msg_t)msg_id;
            
            if (m->type == HELLO) 
            {
                printf("TRACE: proc(%ld) - received Hello from node %ld, sending Hello_Ack\n", n, m->from);
                
                // Process the Hello and send Hello_Ack
                use(node[n].cpu, T_DELAY);
                form_ack(m);
                printf("TRACE: proc(%ld) - formed Hello_Ack, about to send\n", n);
                send_msg(m);
                printf("TRACE: proc(%ld) - Hello_Ack sent successfully\n", n);
            }
            else 
            {
                printf("TRACE: proc(%ld) - unexpected message type %ld\n", n, m->type);
                return_msg(m);
            }
        }
        else 
        {
            printf("TRACE: proc(%ld) - no incoming messages received\n", n);
        }
    }
    
    printf("TRACE: proc(%ld) - exiting\n", n);
}

/* send message function */
void send_msg(msg_t m)
{
    long from, to;
    
    printf("TRACE: send_msg() - starting\n");
    from = m->from;
    to = m->to;
    printf("TRACE: send_msg() - from=%ld, to=%ld, type=%ld\n", from, to, m->type);

    if (uniform01() < current_loss_prob)
    {
        printf("TRACE: send_msg() - packet lost due to loss probability\n");
        printf("%.1f Packet lost: %s from node.%ld to node.%ld\n", clock, (m->type == HELLO ) ? "Hello" : "Hello_Ack", from, to);
        return_msg(m);
        return;
    }

    printf("TRACE: send_msg() - packet will be transmitted\n");
    use(node[from].cpu, T_DELAY);
    printf("TRACE: send_msg() - used CPU for T_DELAY\n");
    reserve(network[from][to]);
    printf("TRACE: send_msg() - reserved network facility\n");
        hold(TRANS_TIME);
        printf("TRACE: send_msg() - held for TRANS_TIME, about to send\n");
        send(node[to].input, (long)m);
        printf("TRACE: send_msg() - message sent to mailbox\n");
    release(network[from][to]);
    printf("TRACE: send_msg() - released network facility, completed\n");
}

/* new message function - INCOMPLETE */
msg_t new_hello_msg(long from)
{
    msg_t m;    
    long i;
    
    if(msg_queue == NIL) 
    {
        m = (msg_t)do_malloc(sizeof(struct msg));
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
    printf("TRACE: run_simulation(%.1f) - starting\n", loss_prob);
    current_loss_prob = loss_prob;
    show_events = (loss_prob == 0.3);  // Only show events for loss_prob = 0.3
    
    printf("\n--- Running simplified test with loss probability %.1f ---\n", loss_prob);
    printf("TRACE: run_simulation() - about to reset counters\n");
    reset_counters();
    printf("TRACE: run_simulation() - about to call CSIM reset()\n");
    reset();  // CSIM function to reset simulation state including clock
    printf("TRACE: run_simulation() - CSIM reset() completed, about to call sim()\n");
    sim();
    printf("TRACE: run_simulation() - sim() completed\n");
}

/* Main function */
int main()
{
    printf("Project #2: Heartbeat Protocol Simulation\n");
    printf("==========================================\n");
    
    printf("\nRunning single test simulation for loss probability 0.2...\n");
    
    run_simulation(0.2);
    
    printf("\nSimulation completed.\n");
    
    return 0;
} 