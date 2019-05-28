#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);

struct birthday *createBirthday(int day, int month, int year) {

  /* TODO: 생일을 위한 메모리를 할당하고, 인자들을 채워 생일을 완성하세요. */
  struct birthday *myBirthday;

  myBirthday = kmalloc(sizeof(struct birthday), GFP_KERNEL);

  if(!myBirthday){
    printk(KERN_ERR "memory alloc failed\n");
    return NULL;
  }

  myBirthday->day = day;
  myBirthday->month = month;
  myBirthday->year = year;

  return myBirthday;
}

int simple_init(void) {
  printk("INSTALL MODULE(Team01): bdlist\n");

  /* TODO: 생일 목록을 하나씩 생성하는대로 연결리스트에 연결시키세요(노드 삽입). */
  struct birthday *b1 = createBirthday(11, 3, 1995);
  struct birthday *b2 = createBirthday(18, 11, 1995);
  struct birthday *b3 = createBirthday(18, 11, 1995);
  struct birthday *b4 = createBirthday(15, 11, 1994);

  list_add(&b1->list, &birthday_list);
  list_add(&b2->list, &birthday_list);
  list_add(&b3->list, &birthday_list);
  list_add(&b4->list, &birthday_list);

  /* TODO: 완성된 연결리스트를 탐색하는 커널 함수를 사용하여 출력하세요. */
  struct birthday *tmp; //curosr
  list_for_each_entry(tmp, &birthday_list, list){
    printk("OS Module(Team01): DAY %d %d %d\n", tmp->day, tmp->month, tmp->year);
  }

  return 0;
}

void simple_exit(void) {
  /* 모듈을 제거할 때는 생성한 연결 리스트도 하나씩 제거하며 끝내도록 하세요. */
  /* 제거를 하기 전에 리스트가 "비어있을 경우""에 대한 예외처리를 하는게 좋겠죠? */
  if(list_empty(&birthday_list)) {
    printk("List is Empty\n");
    return;
  }
  /* TODO: 이제 본격적으로 연결리스트를 탐색하면서 하나씩 제거하도록 하시면 됩니다. */

  /* 다만, 제거를 하면서 연결리스트를 탐색하면 문제가 생길 것 같은데 어떤 방법으로 해결 가능한지 생각해보세요. */
  
  struct birthday *tmp;
  struct list_head *cursor, *next;

  list_for_each_safe(cursor, next, &birthday_list){
    tmp = list_entry(cursor, struct birthday, list);

    printk("OS Module(Team01): Removing %d %d %d\n", tmp->day, tmp->month, tmp->year);
    list_del(cursor);
    kfree(tmp);
  }

  printk("REMOVE MODULE(Team01): bdlist\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("make a list of birthdays and print");
MODULE_AUTHOR("Team01");
