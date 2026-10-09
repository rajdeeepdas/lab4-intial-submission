#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define min(a,b) (((a)<(b))?(a):(b))

// total jobs
int numofjobs = 0;

struct job {
    // job id is ordered by the arrival; jobs arrived first have smaller job id, always increment by 1
    int id;
    int arrival; // arrival time; safely assume the time unit has the minimal increment of 1
    int length;
    int tickets; // number of tickets for lottery scheduling
    // TODO: add any other metadata you need to track here
    int start;
    int completion;
    struct job *next;
};

// the workload list
struct job *head = NULL;


void append_to(struct job **head_pointer, int arrival, int length, int tickets){

    struct job *new_job = malloc(sizeof(struct job));
    assert(new_job != NULL);

    new_job->id = numofjobs;
    new_job->arrival = arrival;
    new_job->length = length;
    new_job->tickets = tickets;
    new_job->start = -1;
    new_job->completion = -1;
    new_job->next = NULL;

    if (*head_pointer == NULL) {
        *head_pointer = new_job;
    } else {
        struct job *current = *head_pointer;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_job;
    }

    numofjobs++;
    return;
}


void read_job_config(const char* filename)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tickets  = 0;

    char* delim = ",";
    char *arrival = NULL;
    char *length = NULL;

    // TODO, error checking
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // TODO: if the file is empty, we should just exit with error
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if( line[read-1] == '\n' )
            line[read-1] =0;
        arrival = strtok(line, delim);
        length = strtok(NULL, delim);
        tickets += 100;

        append_to(&head, atoi(arrival), atoi(length), tickets);
    }

    fclose(fp);
    if (line) free(line);
}


void policy_SJF()
{
    printf("Execution trace with SJF:\n");

    // TODO: implement SJF policy

    printf("End of execution with SJF.\n");

}


void policy_STCF()
{
    printf("Execution trace with STCF:\n");

    // TODO: implement STCF policy

    printf("End of execution with STCF.\n");
}


void policy_RR(int slice)
{
    printf("Execution trace with RR:\n");

    // TODO: implement RR policy

    printf("End of execution with RR.\n");
}


void policy_LT(int slice)
{
    printf("Execution trace with LT:\n");

    // Leave this here, it will ensure the scheduling behavior remains deterministic
    srand(42);

    // In the following, you'll need to:
    // Figure out which active job to run first
    // Pick the job with the shortest remaining time
    // Considers jobs in order of arrival, so implicitly breaks ties by choosing the job with the lowest ID

    // To achieve consistency with the tests, you are encouraged to choose the winning ticket as follows:
    // int winning_ticket = rand() % total_tickets;
    // And pick the winning job using the linked list approach discussed in class, or equivalent

    printf("End of execution with LT.\n");

}


void policy_FIFO(){
    printf("Execution trace with FIFO:\n");

    int current_time = 0;
    struct job *current = head;

    while (current != NULL) {
        if (current_time < current->arrival) {
            current_time = current->arrival;
        }

        current->start = current_time;
        printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
               current_time, current->id, current->arrival, current->length);
        current_time += current->length;
        current->completion = current_time;

        current = current->next;
    }

    printf("End of execution with FIFO.\n");
}

void analyze_FIFO(){
    printf("Begin analyzing FIFO:\n");

    double total_response = 0;
    double total_turnaround = 0;
    double total_wait = 0;
    struct job *current = head;

    while (current != NULL) {
        int response = current->start - current->arrival;
        int turnaround = current->completion - current->arrival;
        int wait = response;

        printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n",
               current->id, response, turnaround, wait);

        total_response += response;
        total_turnaround += turnaround;
        total_wait += wait;
        current = current->next;
    }

    printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n",
           total_response / numofjobs, total_turnaround / numofjobs, total_wait / numofjobs);
    printf("End analyzing FIFO.\n");
}


int main(int argc, char **argv){

    static char usage[] = "usage: %s analysis policy slice trace\n";

    int analysis;
    char *pname;
    char *tname;
    int slice;


    if (argc < 5)
    {
        fprintf(stderr, "missing variables\n");
        fprintf(stderr, usage, argv[0]);
		exit(1);
    }

    // if 0, we don't analysis the performance
    analysis = atoi(argv[1]);

    // policy name
    pname = argv[2];

    // time slice, only valid for RR
    slice = atoi(argv[3]);

    // workload trace
    tname = argv[4];

    read_job_config(tname);

    if (strcmp(pname, "FIFO") == 0){
        policy_FIFO();
        if (analysis == 1){
            analyze_FIFO();
        }
    }
    else if (strcmp(pname, "SJF") == 0)
    {
        // TODO
    }
    else if (strcmp(pname, "STCF") == 0)
    {
        // TODO
    }
    else if (strcmp(pname, "RR") == 0)
    {
        // TODO
    }
    else if (strcmp(pname, "LT") == 0)
    {
        // TODO
    }

	exit(0);
}