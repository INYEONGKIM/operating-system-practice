# SRJF Scheduler 구현
* 실행 후 출력 예시는 log.txt에 담겨 있습니다.
* list.h에 등록된 메크로 함수들을 이용하기 위해 schedule.h의 sched_array에 list와 task를 추가하여 rq를 관리하였습니다.
* schedule()함수는 현재 rq에 남아있는 task가 있는 경우에 동작하며 현재 rq에 남아있는 task 중에 가장 작은 time slice를 가진 task를 rqCurr(작업해야 할 task)로 변경하는 함수 입니다.
* 자세한 task의 작업 할당량과 감소관련 부분은 cpu.c에서 관리할 것으로 사료되었습니다. 따라서 가장 작은 time slice를 지닌 task를 cpu.c에서 다음 번 작업해야 할 task과 context switch 해주는 것까지가 SRJF schedule() 함수의 최종 역할으로 판단 후 구현했습니다.
* 추가로 rqActive의 task들의 상태 변화를 시각적으로 빠르게 파악하기 위해 printActiveList() 함수를 추가하였습니다. 해당 함수는 schedule() 시작과 끝 부분에 한 번 씩 호출됩니다.
* 코드의 중간마다 주요부분에는 주석으로 설명을 덧붙였습니다.
* 해당 과제에는 김인영 학우와 김건기 학우가 많이 기여하였습니다.
* 한 학기동안 고생 많으셨습니다. 감사합니다.