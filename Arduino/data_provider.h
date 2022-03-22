


#define totChannels 6 

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
extern double summed;
extern int buff_ind;
extern TfLiteStatus SetupIMU(tflite::ErrorReporter* error_reporter);
extern bool ReadData(tflite::ErrorReporter* error_reporter, float* input, int length);
