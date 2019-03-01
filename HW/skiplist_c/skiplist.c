#include"skiplist.h"
#include<stdlib.h>
#include<stdio.h>
/*
*/
#define RAND_HALF (RAND_MAX)/2

/*
 * Create an empty list
 *      height_limit: maximum height of the list allowed
 *      cmp/alloc/del: three function pointers for comparing/allocing memory/deleting memory.
 * Return Value:
 *      return a pointer to skip_list (list)
 */
list createSkipList(int height_limit,
                         int (*cmp)(const void *a, const void *b),
                         void *(*alloc)(const void *),
                         void (*del)(void *)){
  int i;
  /*constrct an empty list*/
  skip_list *newlist;
  newlist = (skip_list*)malloc(sizeof(skip_list));

    newlist->level = 1;
    newlist->height_limit = height_limit;
  /*malloc a new header*/

    /*
    struct skip_list_node{
      int *val;
      int *key;
      struct skip_list_node **forward
    } *newlist->header;
    */
    newlist->header = (skip_list_node *)malloc(sizeof(skip_list_node));
    /*set the initial value of the new header */
    newlist->header->val = NULL;
    newlist->header->key = NULL;
    newlist->header->forward =(struct skip_list_node **)malloc(sizeof(struct  skip_list_node *)*height_limit);
    for (i=0; i<height_limit; i++){
      newlist->header->forward[i] = NULL;
    }
    /* use the function pointer*/
    newlist->cmp = cmp;
    newlist->alloc = alloc;
    newlist->del = del;

  return newlist;
}

/*
 * A function to search for the value corresponding to the key.
 * Return value:
 *      If the key exists, return a void * pointer to the value
 *      If the key does not exist, return NULL
 */
void *searchNode(list s, void *key){
  int Lv = s->level;
  skip_list_node *tmp = s->header;
  /*printf("whole level: %d\n", Lv);*/
  /*printf("%s\n","start searching" );*/
  while (/*tmp->forward[Lv - 1] != NULL &&*/Lv > 0){
    if (tmp->forward[Lv - 1] == NULL){
      Lv -= 1;
      /*printf("%s\n","the end" );*/
    }else{
      if (s->cmp(key,tmp->forward[Lv - 1]->key) == 1){
        tmp = tmp->forward[Lv - 1];
        /*printf("%s\n","lager" );*/
        /*printf("%s\n","move to next node" );*/
      }else if (s->cmp(key,tmp->forward[Lv - 1]->key) ==0){
        /*printf("%s\n","find it");*/
        /*printf("%d\n",*(int*)tmp->forward[Lv - 1]->key );*/
        return tmp->forward[Lv - 1]->val;
      }else{
        /*printf("%s\n","less" );*/
        Lv -= 1;
        /*printf("down to level: %d\n",Lv );*/
      }
    }

  }
  /*printf("%s\n","not found");*/
  return NULL;

}

/*
 * A function to insert a new node with key & value determined by "key" into a list.
 * Return value:
 *      If the key does not exist before, insert the new key & value and return 0
 *      If the key already exists before, replace the value with the new one, and return 1
 *      return -1 on any kind of failure
 */
int insertNode(list s, void *key, void *val){
  int i;
  int size = 1;
  /* get prepared for tracing */
  skip_list_node ***trace;
  int Lv = s->level;
  skip_list_node *tmp;
  skip_list_node *newnode;

  if (key == NULL){
    return -1;
  }
  if (val == NULL){
    return -1;
  }

  /*malloc a new node */
  newnode= (skip_list_node*)malloc(sizeof(skip_list_node));
  /*set the initial value*/
  newnode->val = s->alloc(val);
  /*newnode->val = val;*/
  newnode->key = s->alloc(key);
  /*newnode->key = key;*/
  /*malloc the forward*/


  trace = (skip_list_node ***)malloc(sizeof(skip_list_node **)*s->height_limit);
  /*start tracing...*/
  i = 0;
  for (;i<s->height_limit;i++){
    trace[i] = &s->header->forward[i];
  }


  /*start locating*/
  tmp = s->header;

    while (/*tmp->forward[Lv - 1] != NULL &&*/ Lv > 0){
      /*printf("%s\n","start locating" );*/
      if (tmp->forward[Lv - 1] == NULL){
        trace[Lv - 1] = &tmp->forward[Lv - 1];
        Lv -= 1;
        /*printf("%s\n","the end" );*/
      }else{
        if (s->cmp(key,tmp->forward[Lv - 1]->key) == 1){
          /*printf("%d\n", *(int *)tmp->forward[Lv - 1]->key);*/
          tmp = tmp->forward[Lv - 1];
          /*printf("%s\n", "larger");*/
        }else if (s->cmp(key,tmp->forward[Lv - 1]->key) ==0){
          s->del(tmp->forward[Lv - 1]->val);
          s->del(newnode->key);
          s->del(newnode->val);
          free(newnode);
          tmp->forward[Lv- 1]->val = s->alloc(val);
          /*tmp->forward[Lv - 1]->val = val;*/
          /*printf("%s\n","equal" );*/
          free(trace);
          return 1;
        }else{
          /*printf("%s\n","less" );*/
          /*printf("%d\n", *(int *)tmp->forward[Lv - 1]->key);*/
          trace[Lv - 1] = &tmp->forward[Lv - 1];/* store the address of the pointer to the next node */
          /*printf("Lv:%d in front:%d\n",Lv, *(int *)(*trace[Lv-1])->key );*/
          Lv -= 1;

        }
      }
    }
/*
    printf("%s\n","************" );
      i = 0;
      while (*trace[i] != NULL){
        printf("%d :%d\n",i,*(int *)(*trace[i])->key );
        i+=1;
      }
    printf("%s\n","*************" );
*/
/*
    newnode->forward[0] = *trace[0];
    *trace[0]= newnode;
*/
    /*printf("%d\n", *(int*)newnode->forward[0]->key);*/

    /*printf("%d\n",*(int*)s->header->forward[0]->key );*/
    /*printf("%d\n",*(int*)s->header->forward[0]->forward[0]->key);*/



  /*int size = 1;*/
/*
  newnode->forward[0] = *trace[0];
  *trace[0]= newnode;
*/

/*
  printf("%d\n",*(int *)(*trace[0])->forward[0]->key );
  */

  /*printf("%s\n","locating done." );*/

/*
        i = 0;
        while (trace[i] != NULL){
          printf("%d\n",i );
          i+=1;
        }
      printf("%s\n","************8" );

*/

  while (rand() < RAND_HALF && size < s->height_limit){
    /*int i = 0;*/
    /*
    for (;i < size; i++){
      free(newnode->forward[i]);
    }
    */
    /*printf("%s\n","...promoting" );*/
    size += 1;
  }

  newnode->forward = (skip_list_node **)malloc(sizeof(struct skip_list_node *) * size);
  /*newnode->forward[0] = NULL;*/

    /*use trace to change pointers*/
    i = 0;
    for (;i < size; i++){
      /*
      if (*trace[i] == NULL){
        s->header->forward[i] = newnode;
      }else{
        newnode->forward[i] = *trace[i];
        *trace[i] = newnode;
      }
      */
      /*printf("%s\n", "matching");*/
      newnode->forward[i] = *trace[i];
      *trace[i] = newnode;
    }
    /*printf("%s\n","matching done" );*/

  /*printf("node level :%d\n", size);*/
  if (size > s->level){
    s->level = size;
  }

/*****old version for llist level*********
  i = 0;
  s->level = 0;
  while (s->header->forward[i]->key != NULL){
    s->level +=1;
    printf("level count: %d\n", s->level );
    i++;
  }
*/

    /*printf("%s\n","**********" );*/

  /*printf("     Current skiplist level: %d\n", s->level);*/
  /*free and return*/
  free(trace);
  /*printf("insert completed ,list level:%d\n", s->level);*/
  return 0;
}

/*
 * A function to remove a node with value given by "key" from a list.
 * Return value:
 *      return 0 on success
 *      return -1 on failure
 */
int deleteNode(list s, void *key){
  int i;
  /*this time use 2 trace aray*/
  skip_list_node ***trace_1;
  skip_list_node ***trace_2;

  int Lv = s->level;
  /*declear the temparary node pointers*/
  skip_list_node *thatisit;
  skip_list_node *tmp;

  if (key == NULL){
    return -1;
  }


  /*printf("list level:%d\n",Lv );*/
  /*initialize trace_1*/
  trace_1 = (skip_list_node ***)malloc(sizeof(skip_list_node **)*s->height_limit);
  i = 0;
  for (;i < s->height_limit;i++){
    trace_1[i] = NULL;
  }
  /*initialize trace_1*/
  trace_2 = (skip_list_node ***)malloc(sizeof(skip_list_node **)*s->height_limit);
  i = 0;
  for (;i < s->height_limit;i++){
    trace_2[i] = NULL;
  }
  /*assign header to tmp*/
  tmp = s->header;

  /*thatisit = (skip_list_node*)malloc(sizeof(skip_list_node));*/


  while (/*tmp->forward[Lv - 1] != NULL &&*/ Lv > 0){
    if (tmp->forward[Lv - 1] == NULL){
      /*printf("from lv:%d to lv:%d\n",Lv,Lv-1 );*/
      Lv -= 1;
      /*printf("%s\n","the end" );*/
    }else{
      /*printf("%s\n", "locating...");*/
      if (s->cmp(key,tmp->forward[Lv - 1]->key) == 1){
        /*printf("%s\n","lager" );*/
        /*printf("%d\n",*(int*)tmp->forward[Lv - 1]->key );*/
        tmp = tmp->forward[Lv - 1];
      }else if (s->cmp(key,tmp->forward[Lv - 1]->key) ==0){
        /*printf("%s\n","equal" );*/
        thatisit =  tmp->forward[Lv - 1];
        /*printf("%d\n",*(int*)tmp->forward[Lv - 1]->key );*/
        /*printf("%d\n",*(int*)thatisit->key );*/
        trace_1[Lv - 1] = &tmp->forward[Lv - 1];
        trace_2[Lv - 1] = &tmp->forward[Lv - 1]->forward[Lv - 1];
        /*printf("from lv:%d to lv:%d\n",Lv,Lv-1 );*/
        Lv -=1;
      }else{
        /*printf("%s\n","less" );*/
        /*printf("%d\n",*(int*)tmp->forward[Lv - 1]->key );*/
        /*printf("from lv:%d to lv:%d\n",Lv,Lv-1 );*/
        Lv -= 1;
      }
    }
  }

  if (trace_1[0] == NULL){



    free(trace_1);
    free(trace_2);
    return -1;
  }

/*
  printf("%d\n",*(int*)thatisit->key );
  */

  /*printf("%s\n","****" );*/
  /*int i = 0;*/
/*
  i = 0;
  while (thatisit->forward[i]){
    free(thatisit->forward[i]);
    i += 1;
  }
  */


  i = 0;
  for (;i < s->height_limit; i++){
/*
    printf("%d\n", i);
*/
    if (trace_1[i] != NULL){
      *trace_1[i] = *trace_2[i];
    }
  }
  /*counting the level*/
  /**old version**/
  i = 0;
  s->level = 0;
  while (s->header->forward[i] != NULL && s->level < s->height_limit){
    /*printf("%d\n",i );*/
    s->level +=1;
    i++;
  }
  /*printf("%d\n",s->level );*/


  if (s->level == 0){
    s->level = 1;
  }

  /*printf("level after delete:%d\n",s->level );*/

  /*free and return*/
  s->del(thatisit->key);
  s->del(thatisit->val);
  free(thatisit->forward);
  free(thatisit);
  free(trace_1);
  free(trace_2);
  return 0;
}


/*
 * A function to free all the dynamically allocated memory used by the tree.
 */
void freeSkipList(list s){
  /*struct skip_list_node *last = s->header;*/
  node now = s->header;
  node last;
  /*use 2 tmp to travel the whole list*/
  while (now->forward[0]!=NULL){
    last = now;
    now = now->forward[0];
    s->del(last->key);
    s->del(last->val);
    free(last->forward);
    free(last);
  }

  s->del(now->key);
  s->del(now->val);
  free(now->forward);
  free(now);
  free(s);
}

int compareInt(const void *a, const void *b){
  /*cast void* to int * */
  /*larger*/
  if (*(int*)a > *(int*)b){
    return 1;
  /*equal*/
  }else if(*(int*)a == *(int*)b){
    return 0;
  /*less*/
  }else{
    return -1;
  }
}
/*to free the int pointer */
void deleteInt(void *a){
  free((int*)a);

}
/*to alloc for the int pointer */
void *allocInt(const void *a){
  void *space;
  space =malloc(sizeof(int));
  *(int *)space = *(int*)a;

  return space;
}
