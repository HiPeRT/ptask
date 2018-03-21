#include <ptask.h>
#include <calibrate.h>


void body()
{
    int idx = ptask_get_index();
    for (int i=0; i<100; i++) {
        printf("task %d, iter: %d\n", idx, i);
        work_for(i/10, MILLI);

        ptask_wait_for_period();
    }
}

int main()
{
    ptask_init(SCHED_DEADLINE, PARTITIONED, PRIO_INHERITANCE);

    tpars p;
    ptask_param_init(p);
    ptask_param_period    (p, 10, MILLI);
    ptask_param_deadline  (p,  4, MILLI);
    ptask_param_processor (p,  0);
    ptask_param_runtime   (p,  3, MILLI);
    ptask_param_measure   (p);
    ptask_param_activation(p, DEFERRED);
    int t_id = ptask_create_param(body, &p);
    if (t_id < 0) {
        printf("Error in creating the task\n");
        return 1;
    } else {
        printf("activate\n");
        ptask_activate_at(t_id, ptask_gettime(MILLI) + 1000, MILLI);
    }

    pthread_join(ptask_get_threadid(t_id), 0);
    

    tspec awg  = ptask_get_avg  (t_id);
    tspec wcet = ptask_get_wcet (t_id);
    tspec tot  = ptask_get_total(t_id);
    int exec = ptask_get_numinstances   (t_id);
    int miss = ptask_get_deadline_misses(t_id);
    printf("wcet: %ld us\nawg: %ld us\ntot: %ld us\nexecuted: %d\nmiss: %d\n", 
        tspec_to(&wcet, MICRO), tspec_to(&awg,  MICRO), tspec_to(&tot,  MICRO), exec, miss);
    return 0;
}
