/* schedule.c
 * This file contains the primary logic for the 
 * scheduler.
 */
#include "schedule.h"
#include "macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWTASKSLICE (NS_TO_JIFFIES(100000000))

/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;

/* External Globals
 * jiffies - A discrete unit of time used for scheduling.
 *			 There are HZ jiffies in a second, (HZ is 
 *			 declared in macros.h), and is usually
 *			 1 or 10 milliseconds.
 */
extern long long jiffies;

/*-----------------Initilization/Shutdown Code-------------------*/
/* This code is not used by the scheduler, but by the virtual machine
 * to setup and destroy the scheduler cleanly.
 */
 
/* initscheduler
  * Sets up and allocates memory for the scheduler, as well
  * as sets initial values. This function should also
  * set the initial effective priority for the "seed" task 
  * and enqueu it in the scheduler.
  * INPUT:
  * newrq - A pointer to an allocated rq to assign to your
  *			local rq.
  * seedTask - A pointer to a task to seed the scheduler and start
  * the simulation.
  */
 //INYEONG
void initschedule(struct runqueue *newrq, struct task_struct *seedTask)
{
	// init rq
	rq = newrq;

	// active list 생성
	rq->active  = (struct sched_array *) malloc( sizeof(struct sched_array));
	INIT_LIST_HEAD(&rq->active->list);

	// init task 부터 시작
	enqueue_task(seedTask, rq->active);

	// time slice setting
	seedTask->first_time_slice = NEWTASKSLICE;
	seedTask->time_slice = NEWTASKSLICE;

	rq->nr_running++;
}

/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */
void killschedule()
{	
	// runqueue에서 사용한 active list 전부 메모리 free
  	struct sched_array *tmp, *next;
	list_for_each_entry_safe(tmp, next, &(rq->active->list), list){
		free(&tmp->list); 
	}

	free(rq->active);
}

/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task with the shortest runtime(time slice) remaining
 */
void schedule()
{
	// start line
	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - > Now running(rq->nr_running) : %lu", rq->nr_running);
	
	printf("\n# BEFORE :\n");
	printActiveList();

	// rq->active에 task가 남아 있다면 스케줄링 진행
  	if (rq->nr_running > 0){
		struct sched_array *tmp, *next = list_entry(rq->active->list.next, struct sched_array, list);
		
		if (rq->curr != NULL){
			// rq->curr의 time slice가 끝났음에도 아직 task가 덜 끝난 경우 기본단위(10)으로 setting
			if( rq->curr->time_slice == 0 ){
				rq->curr->time_slice = rq->curr->first_time_slice;
			}

			// curr task 변경
			list_del( &rq->curr->array->list);
			list_add_tail(&rq->curr->array->list, &rq->active->list);

			// 가장 time_slice가 작은 task를 탐색
			list_for_each_entry(tmp, &(rq->active->list), list){
				if (next->task->time_slice > tmp->task->time_slice){
					next = tmp; //found
				}
			}
			
			// 스케줄링 완료
			rq->curr->need_reschedule = 0;
		}

		// 만약 다음번 실행할 task와 next->task가 다를 경우 context_switch
		if(rq->curr != next->task ){
			context_switch(next->task);
			rq->curr = next->task;
			rq->nr_switches++;
		}
	}

	printf("\n# AFTER :\n");
	printActiveList();
}

// INYEONG
void printActiveList()
{
	printf("[rq->active] :\n");
	struct sched_array *tmp;

	// first time slice 10 고정확인
	list_for_each_entry(tmp, &rq->active->list, list) {
		printf("    %x timeslice : %u\tneed_reschedule : %d", (unsigned int) tmp->task, tmp->task->time_slice, tmp->task->need_reschedule);
		
		if (tmp->task->need_reschedule){
			printf("\t<------ need to reschedule\n");
		}
		else{
			printf("\n");
		}
	}
}

/* enqueue_task
 * Enqeueus a task in the passed sched_array
 */
void enqueue_task(struct task_struct *p, struct sched_array *array)
{
	struct sched_array *new = (struct sched_array *) malloc(sizeof(struct sched_array));
	p->array = new;
	new->task = p;
	list_add(&new->list, &array->list);
}

/* dequeue_task
 * Removes a task from the passed sched_array
 */
void dequeue_task(struct task_struct *p, struct sched_array *array)
{
	if (rq->curr = p){
		rq->curr = NULL;
	}
	list_del(&(p->array->list));
}

/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{	
	// child first time slice 설정 (기본값 10)
	p->first_time_slice = rq->curr->first_time_slice;

	// parent time slice forking
	p->time_slice = rq->curr->time_slice;

	// curr task가 time slice 다 사용 시 다시 초기설정(10)
	if (rq->curr->time_slice <= 0)
	{
		rq->curr->time_slice = rq->curr->first_time_slice;
	}
}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)
{
  	p->time_slice--; // time slice 하나 감소 

	if ( p->time_slice <= 0 ){ // time slice를 다 사용한 경우 reschedule
		p->need_reschedule = 1;
	}
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{
	// active queue에 추가 후 counter 증가
	enqueue_task(p, rq->active);
	rq->nr_running++;
}

/* __activate_task
 * Activates the task in the scheduler
 * by adding it to the active array.
 */
void __activate_task(struct task_struct *p)
{
	// active queue에 추가 후 counter 증가
	enqueue_task(p, rq->active);
	rq->nr_running++;
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{		
	// activate_task == __activate_task (실제 스케줄러의 구현 방식 차용)
  	__activate_task(p);	
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	// queue에서 제거 한 후 counter 감소
  	dequeue_task(p, NULL);
  	rq->nr_running--;
}
