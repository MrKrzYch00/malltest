#define _BSD_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct KrzYThread {
  size_t x;
  size_t mallsize;
  size_t i;
  int is_running;
} KrzYThread;

typedef struct KrzYOptions {
  size_t numthreads;
  size_t memory;
  size_t numiterations;
} KrzYOptions;

static void KrzYInitOptions(KrzYOptions* options) {
  options->numthreads = 1;
  options->memory = 65535;
  options->numiterations = 10000;
}

static void *threading(void *a) {
  KrzYThread *b = (KrzYThread *)a;
  size_t y = b->i;
  unsigned char n;
  unsigned int i;
  size_t m;
  size_t *lul = NULL;
  char *lulch = NULL;
  if(y == 0) y = (size_t)-1;
  for(b->x = 0; b->x < y; ++(b->x)) {
    lul = malloc(b->mallsize * sizeof(size_t));
    lulch = (char*)lul;
    for(n = 0; n < 8; ++n) {
      for(m = 0; m < b->mallsize; ++m) {
       for(i = 1; i < sizeof(size_t); ++i) {
        lulch[m*i] = 1 << n;
       }
      }
      for(m = (b->mallsize - 1); m != 0; --m) {
       for(i = 1; i < sizeof(size_t); ++i) {
        if(lulch[m*i] != (1 << n)) {
         fprintf(stderr,"ERROR !\n");
        }
       }
      }
    }
    free(lul);
  }
  b->is_running = 0;

  return 0;
}

int main(int argc, char* argv[]) {
  int j;
  KrzYOptions options;
  KrzYInitOptions(&options);
  for (j = 1; j < argc; j++) {
   const char* arg = argv[j];
   if (arg[0] == '-' && arg[1] == '-' && arg[2] == 'i'
    && arg[3] >= '0' && arg[3] <= '9') {
     options.numiterations = atoi(arg + 3);
   } else if (arg[0] == '-' && arg[1] == '-' && arg[2] == 'm'
           && arg[3] >= '0' && arg[3] <= '9') {
     options.memory = atoi(arg + 3);
   } else if (arg[0] == '-' && arg[1] == '-' && arg[2] == 't'
           && arg[3] >= '0' && arg[3] <= '9') {
     options.numthreads = atoi(arg + 3);
   } else if (arg[0] == '-' && (arg[1] == 'h' || arg[1] == '?' || (arg[1] == '-'
          && (arg[2] == 'h' || arg[2] == '?')))) {
     fprintf(stderr,"MallTest: Threaded Malloc Tester v1.00 by Mr_KrzYch00\n\n"
                    "  --h           shows this help (--?, -h, -?)\n"
                    "  --i#          perform # iterations (d: 10000; 0 => 4.2 billion)\n"
                    "  --m#          use # bytes of memory/thread (d:65535)\n"
                    "  --t#          use # threads, (d:1)\n\n");
     fprintf(stderr,"WARNING! This works like a stress test. High CPU/memory\n"
                    "temperatures as well as crashes, reboots or/and hardware\n"
                    "malfunction is to be expected!\n\n");

     fprintf(stderr,"This software is provided \"as is\" without warranty of any kind,\n"
                    "either expressed or implied. In no event author is reponsible\n"
                    "for any damage the usage of this software may cause.\n\n");
      return EXIT_FAILURE;
   }
  }
  {
   size_t i;
   size_t threnum = 0;
   size_t showcntr = 0;
   size_t cntrrunning = 0;
   size_t numthreads = options.numthreads;
   KrzYThread *t = malloc(sizeof(KrzYThread) * numthreads);
   pthread_t *thr = malloc(sizeof(pthread_t) * numthreads);
   pthread_attr_t *thr_attr = malloc(sizeof(pthread_attr_t) * numthreads);
   for(i=0;i<numthreads;++i) {
    pthread_attr_init(&(thr_attr[i]));
    pthread_attr_setdetachstate(&(thr_attr[i]), PTHREAD_CREATE_DETACHED);
    t[i].mallsize = options.memory;
    t[i].i = options.numiterations;
    t[i].is_running = 1;
    pthread_create(&thr[i], &(thr_attr[i]), threading, (void *)&t[i]);
   }
   for(;threnum<numthreads;) {
    if(t[threnum].is_running==1) {
     cntrrunning = 0;
     usleep(250000);
     fprintf(stderr,"THR %d | %d\r",(int)threnum,t[threnum].x);
     if(showcntr>4) {
      ++threnum;
      if(threnum>=numthreads)
       threnum=0;
      showcntr=1;
     } else {
      ++showcntr;
     }
    } else {
     if(cntrrunning > numthreads)
      break;
     ++threnum;
     ++cntrrunning;
    }
   }
  }
  fprintf(stderr,"Finished !   \n\n");
  return EXIT_SUCCESS;
}