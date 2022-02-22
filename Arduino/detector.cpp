#include <Arduino.h>
#include "detector.h"
#define HISTORY_COUNT 5

namespace {
float detection_history[HISTORY_COUNT] = {0};
int hist_ind = 0;
}

int8_t detector(float* output){
  float avg_pred=0;
  detection_history[hist_ind]=output[0];
  hist_ind++;
  if (hist_ind >= HISTORY_COUNT)
  {
  hist_ind=0;
  }
  for (int i=0; i<HISTORY_COUNT;i++)
  {
    avg_pred+=detection_history[i];
  }
  avg_pred=avg_pred/HISTORY_COUNT;
  return avg_pred;
  if (avg_pred>0.5)
  {
    return 1;
  }
  else {
    return 0;
  }
  
}
