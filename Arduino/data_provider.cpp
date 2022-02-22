
// Inspired by Magic Wand Example
#include "data_provider.h"
#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
//#define BUFF_SIZE 5718
#define BUFF_SIZE 6012
int drop_count=1;
int buff_ind=0;
float mem_buff[BUFF_SIZE]={0.0};
bool init_data_flag = true;
int sample_every_n = 5;
//double summed;


TfLiteStatus SetupIMU(tflite::ErrorReporter* error_reporter) {
  if (!(IMU.begin())) {
  TF_LITE_REPORT_ERROR(error_reporter, "Failed to initialize IMU");
    return kTfLiteError;
  }
  IMU.setContinuousMode();
  TF_LITE_REPORT_ERROR(error_reporter, "System Initialized!");
  return kTfLiteOk;
}

bool ReadData(tflite::ErrorReporter* error_reporter, float* input, int length) {
  float xA, yA, zA , xG, yG, zG;
  
  bool new_data=false;
  //while (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    while (IMU.accelerationAvailable()) {
    if (!IMU.readAcceleration(xA, yA, zA)) {
      TF_LITE_REPORT_ERROR(error_reporter, "Failed to read data");
      break;
    }
    if (!IMU.readGyroscope(xG, yG, zG)) {
      TF_LITE_REPORT_ERROR(error_reporter, "Failed to read data");
      break;
    }
    if (drop_count!=sample_every_n)
    {
      drop_count+=1;
      continue;
    }
    mem_buff[buff_ind++]=xA;
    mem_buff[buff_ind++]=yA;
    mem_buff[buff_ind++]=zA;
    mem_buff[buff_ind++]=xG;
    mem_buff[buff_ind++]=yG;
    mem_buff[buff_ind++]=zG;

    drop_count=1;
    if (buff_ind>=BUFF_SIZE)
    {
      buff_ind=0;
    }
    new_data=true;
  }

  if(!new_data)
  {
    return false;
  }
  if (init_data_flag && buff_ind>=(int)(BUFF_SIZE/3))
  {
    init_data_flag = false;
  }
  if (init_data_flag)
  {
    return false;
  }
//  summed=0;
  for (int i=0; i<length;++i)
  {
    int circ_ind=buff_ind+i-length;
    if (circ_ind<0)
    {
      circ_ind+=BUFF_SIZE;
    }
    input[i]=mem_buff[circ_ind];
    //summed+=mem_buff[circ_ind];
  }
  return true;
}
