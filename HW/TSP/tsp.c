#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tsp.h"

struct PA{
  int *source;
  int **map;
  int *result;
  int begin;
  int end;
};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct PA PA;

/*set type to recursive
static void thread_init(void)  {
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

  pthread_mutex_init(&lock, &attr);

  pthread_mutexattr_destroy(&attr);
}
*/

/*to swap two element*/
void Swap(int *l, int *r){
  int t = *l;
  *l = *r;
  *r = t;
}

/*to get all the permutation*/
/*void Permutation(int source[], int **map, int* result, int begin, int end){*/
void* Permutation(){
  int i;
  int sum = 0;
  pthread_mutex_lock(&lock);
  if (PA.begin >= PA.end){ /*find a permutation*/
    for (i = 0; i < PA.end; i++){
      /*printf("%d: ", PA.source[i]);*/
      if (i < PA.end-1){
        if (PA.map[PA.source[i]][PA.source[i+1]] == 0){
          pthread_mutex_unlock(&lock);
          return NULL;
        }
        sum += PA.map[PA.source[i]][PA.source[i+1]];
        /*printf("%d ", PA.map[PA.source[i]][PA.source[i+1]]);*/
      }else if (i == PA.end -1){
        if (PA.map[PA.source[i]][PA.source[0]] == 0){
          pthread_mutex_unlock(&lock);
          return NULL;
        }
        sum += PA.map[PA.source[i]][PA.source[0]];
        /*printf("%d ", PA.map[PA.source[i]][PA.source[0]]);*/
      }
    }
    /*printf("\n");*/
    /*printf("sum: %d\n", sum);*/
    if (*(PA.result) == 0){
      *(PA.result) = sum;
    }else{
      if (sum <= *(PA.result)){
        *(PA.result) = sum;
      }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
  }else{ /*not done, go on*/
    for (i = PA.begin; i < PA.end; i++){
      if (PA.begin != i){
        Swap(&(PA.source[PA.begin]), &(PA.source[i])); /*swap the first element and the current one*/
      }
      /*deal with the rest of the elements*/
      PA.begin += 1;
      pthread_mutex_unlock(&lock);
      Permutation();
      pthread_mutex_lock(&lock);
      PA.begin -= 1;
      if (PA.begin != i){
        Swap(&(PA.source[PA.begin]), &(PA.source[i]));
      }

    }
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main(){
  /*all the declaration*/
  char input[1024];
  int thread_num;
  int city_num;
  int start_point;
  int **map;
  int is_n;
  int *index;
  int result[1];


  int i;
  int j;
  int row;
  int col;
  pthread_t* PermutationThread;

  /*thread_init();*/

  /*-------------reading in-------------*/
  i = 0;
  while (scanf("%s",input)){
    if (i == 0){
      thread_num = atoi(input);
      /*printf("thread_num: %d\n", thread_num);*/
      i += 1;
    }else if (i == 1){
      city_num = atoi(input);
      /*printf("city_num: %d\n", city_num);*/
      map = (int **)malloc(sizeof(int*) * city_num); /* create a 2D array to store the map */
      j = 0;
      for (j = 0; j < city_num; j++){
        map[j] = (int *)malloc(sizeof(int) * city_num);
      }
      for (row = 0; row < city_num; row++){ /* intialize the 2D map */
        for (col = 0; col < city_num; col++){
          map[row][col] = 0;
        }
      }
      row = 0;/* inintialize the row */
      col = 0;/* inintialize the col */
      is_n = city_num;
      i += 1;
    }else if (i == 2){
      start_point = atoi(input);
      if (start_point < 0 || start_point >= city_num){
        for(j = 0; j < city_num; j++){/*free the map*/
          free(map[j]);
        }
        free(map);
        printf("-1\n");
        return 0;
      }
      /*printf("start_point: %d\n",start_point );*/
      i += 1;
    }else if(i > 2){
      /*build the 2-D array*/
      map[row][col] = atoi(input);
      if (map[row][col] < 0 || map[row][col] > 1000){
        for(j = 0; j < city_num; j++){/*free the map*/
          free(map[j]);
        }
        free(map);
        printf("-1\n");
        return 0;
      }
      is_n += 1;
      col += 1;
      if (is_n % city_num == 0){ /* while reach the line limit, change the line */
        row += 1;
        is_n = 0;
        col = 0;
      }
      if (row == city_num){
        break;
      }
    }
  }
  /*print the 2D array
  for (row = 0; row < city_num; row++){
    for (col = 0; col < city_num; col++){
      printf("%d ", map[row][col]);
    }
    printf("\n");
  }
  */

  /*set all the parameters*/
  /* create a list from 0 to city_num-1 */
  index = (int*)malloc(sizeof(int) * city_num);
  for (i = 0; i < city_num; i++){
    index[i] = i;
  }
  result[0] = 0;
  PA.source = index;
  PA.map = map;
  PA.result = result;
  PA.begin = 0;
  PA.end = city_num;
  PermutationThread = (pthread_t *)malloc(sizeof(pthread_t) * thread_num); /* create a list of threads id */

  /*printf("------\n");*/
  /*Permutation();  get all the permutatoins */

  for (i = 0; i < thread_num; i++) {
    /*printf("%d\n", i);*/
    pthread_create(&PermutationThread[i], NULL, &Permutation, NULL);/* create the thread */
  }

  for (i = 0; i < thread_num; i++) {
    /*printf("%d\n", i);*/
    pthread_join(PermutationThread[i], NULL); /* join the main thread */
  }

  /*printf("%d\n", *result);*/
  if (*result == 0){
    free(index);/*free the index*/
    free(PermutationThread);/*free the list of threads id*/
    for(j = 0; j < city_num; j++){/*free the map*/
      free(map[j]);
    }
    free(map);
    printf("-1\n");
    return 0; /* there is no feasible loop*/
  }

  free(index);/*free the index*/
  free(PermutationThread);/*free the list of threads id*/
  for(j = 0; j < city_num; j++){/*free the map*/
    free(map[j]);
  }
  free(map);
  printf("%d\n", *result);/*print the result*/
  return 0;
}
