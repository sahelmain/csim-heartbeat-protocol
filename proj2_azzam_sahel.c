// Project #2; Azzam, Sahel

#include "csim.h"


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
struct msg 
{
    long type;
    long from;
    long to;
    TIME start_time;
    msg_t link;
    long retry_count;
};


struct nde 
{
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



void sim()
{
    char sim_name[20];
    sprintf(sim_name, "sim%.1f", current_loss_prob);
    create(sim_name);
    init();
    hold(SIMTIME);
    my_report();
}

/* initialization function */
void init()
{
    long i, j;
    char str[24];
    
    fp = fopen("xxx.out", "w");
    set_output_file(fp);
    max_facilities(NUM_NODES*NUM_NODES+NUM_NODES);
    max_servers(NUM_NODES*NUM_NODES+NUM_NODES);
    max_mailboxes(NUM_NODES);
    max_events(2*NUM_NODES*NUM_NODES);
    resp_tm = table("msg rsp tm");
    msg_queue = NIL;
    
    for (i = 0; i < NUM_NODES; i++) 
    {
        sprintf(str, "cpu.%ld", i);
        node[i].cpu = facility(str);
        sprintf(str, "input.%ld", i);
        node[i].input = mailbox(str);
    }
    
    for (i = 0; i < NUM_NODES; i++) 
    {
        for(j = 0; j < NUM_NODES; j++) 
        {
            sprintf(str, "net%ld.%ld", i, j);
            network[i][j] = facility(str);
        }
    }
    
    for (i = 0; i < NUM_NODES; i++) 
    {
        proc(i);
    }
}




void proc(long n)
{
    msg_t m;
    long s, t;
    long msg_id;
    char proc_name[20];
    
    sprintf(proc_name, "proc%ld_%.1f", n, current_loss_prob);
    create(proc_name);
    
    while(clock < SIMTIME) 
    {
        hold(exponential(INTER_ARRIVAL_MEAN));

        if (clock >= SIMTIME)
        {
            break;
        }

        m = new_hello_msg(n);
        total_hello_count++;

        if (show_events && event_count < 50)
        {
            printf("node.%ld sends a hello to node.%ld at %.1f seconds\n", n, m->to, clock);
            event_count++;
        }

        // this is the original message that we are sending out 
        msg_t original_msg = m;
        long original_to = m->to;

        send_msg(m);
        
        // this is the wait for the HELLO_ACK 
        int success = 0;
        TIME wait_start = clock;
        
        while ((clock - wait_start) < TIME_OUT && !success)
        {
            TIME remaining = TIME_OUT - (clock - wait_start);
            s = timed_receive(node[n].input, &msg_id, remaining);
            
            if (s == TIMED_OUT) 
            {
                break;
            }
            
            if (msg_id != 0)
            {
                m = (msg_t)msg_id;
                if (m->type == HELLO_ACK && m->from == original_to)
                {
                    success = 1;
                    successful_transmissions++;
                    if (show_events && event_count < 50)
                    {
                        printf("node.%ld receives a hello_ack from node.%ld at %.1f seconds\n", n, m->from, clock);
                        event_count++;
                    }
                    return_msg(m);
                    break;
                }
                else if (m->type == HELLO)
                {
                    // this is the incoming HELLO message
                    use(node[n].cpu, T_DELAY);
                    form_ack(m);
                    if (show_events && event_count < 50)
                    {
                        printf("node.%ld replies a hello_ack to node.%ld at %.1f seconds\n", n, m->to, clock);
                        event_count++;
                    }
                    send_msg(m);
                    // this is the continue waiting for our HELLO_ACK, don't count as failed transmission
                }
                else
                {
                    return_msg(m);
                }
            }
        }
        
        if (success == 0)
        {
            // first attempt failed,  retry
            if (show_events && event_count < 50)
            {
                printf("node.%ld re-sends a hello to node.%ld at %.1f seconds\n", n, original_to, clock);
                event_count++;
            }

            m = new_hello_msg(n);
            m->to = original_to;
            m->retry_count = 1;
            send_msg(m);

            // Wait for HELLO_ACK message from retry
            wait_start = clock;
            while ((clock - wait_start) < TIME_OUT && !success)
            {
                TIME remaining = TIME_OUT - (clock - wait_start);
                s = timed_receive(node[n].input, &msg_id, remaining);
                
                if (s == TIMED_OUT) 
                {
                    break;
                }
                
                if (msg_id != 0)
                {
                    m = (msg_t)msg_id;
                    if (m->type == HELLO_ACK && m->from == original_to)
                    {
                        success = 1;
                        successful_transmissions++;
                        if (show_events && event_count < 50)
                        {
                            printf("node.%ld receives a hello_ack from node.%ld at %.1f seconds\n", n, m->from, clock);
                            event_count++;
                        }
                        return_msg(m);
                        break;
                    }
                    else if (m->type == HELLO)
                    {
                        use(node[n].cpu, T_DELAY);
                        form_ack(m);
                        if (show_events && event_count < 50)
                        {
                            printf("node.%ld replies a hello_ack to node.%ld at %.1f seconds\n", n, m->to, clock);
                            event_count++;
                        }
                        send_msg(m);
                    }
                    else
                    {
                        return_msg(m);
                    }
                }
            }
            
            if (success == 0)
            {
                failed_transmissions++;
                return_msg(m);
            }
        }
    }
}

// function to send message 
void send_msg(msg_t m)
{
    long from, to;
    
    from = m->from;
    to = m->to;

if (uniform01() < current_loss_prob)
{
    if (show_events && event_count < 50)
    {
        printf("packet lost: %s from node.%ld to node.%ld at %.1f seconds\n", (m->type == HELLO ) ? "hello" : "hello_ack", from, to, clock);
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

// function to create new message 
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

// function to return message 
void return_msg(msg_t m)
{
    m->link = msg_queue;
    msg_queue = m;
}

// function to form reply 
void form_ack(msg_t m)
{
    long from, to;
    
    from = m->from;
    to = m->to;
    m->from = to;
    m->to = from;
    m->type = HELLO_ACK;
}

// function to decode message 
void decode_msg(char *str, msg_t m, long n)
{
    printf("%6.3f node %2ld: %s - msg: type = %s, from = %ld, to = %ld\n", clock, n, str, (m->type == HELLO) ? "Hello" : "Hello_Ack", m->from, m->to);
}

// function to reset counters 
void reset_counters()
{
    clock = 0.0;  // Reset simulation clock for new run
    total_hello_count = 0;
    successful_transmissions = 0;
    failed_transmissions = 0;
    event_count = 0;
}

// function to report 
void my_report()
{
    double success_percent = 0.0;
    double failure_percent = 0.0;
    
    if(total_hello_count > 0) 
    {
        success_percent = (double)successful_transmissions / total_hello_count * 100.0;
        failure_percent = (double)failed_transmissions / total_hello_count * 100.0;
    }
    
    printf("\n=== simulation results ===\n");
    printf("loss probability: %.1f\n", current_loss_prob);
    printf("total hello attempts: %ld\n", total_hello_count);
    printf("successful transmissions: %ld (%.2f%%)\n", successful_transmissions, success_percent);
    printf("failed transmissions: %ld (%.2f%%)\n", failed_transmissions, failure_percent);
    printf("==========================\n\n");
}

// function to run simulation 
void run_simulation(double loss_prob)
{
    current_loss_prob = loss_prob;
    show_events = 1;  // show events for all loss probabilities
    
    printf("\n--- Now running simulation with the loss probability %.1f ---\n", loss_prob);
    if(show_events) 
    {
        printf("event snapshot (loss probability %.1f):\n", loss_prob);
    }
    
    reset_counters();
    sim();
}

// main function 
int main()
{
    double loss_prob = 0.3;  // change this value to test different loss probabilities
    
    printf("project #2: heartbeat protocol simulation\n");
    printf("==========================================\n");
    
    printf("\nrunning simulation with packet loss probability %.1f...\n", loss_prob);
    run_simulation(loss_prob);
    
    printf("\nsimulation fully done!\n");
    
    return 0;
}