#include "Arduino.h"
#include <TensorFlowLite.h>
#include "data_provider.h"
#include "model.h"
#include "detector.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
//#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

unsigned long previous_time=0;
bool fall_det=false;

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* model_input = nullptr;
int input_length;

constexpr int kTensorArenaSize = 80 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace
void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR,HIGH);
  digitalWrite(LEDG,HIGH);
  digitalWrite(LEDB,HIGH);
  
  pinMode(LED_BUILTIN, OUTPUT);
  static tflite::MicroErrorReporter micro_error_reporter;  // NOLINT
  error_reporter = &micro_error_reporter;
  
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  /*static tflite::MicroMutableOpResolver<6> micro_op_resolver;  // NOLINT
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddLogistic(); // For Sigmoid
  micro_op_resolver.AddRelu();
  micro_op_resolver.AddReshape(); // For Flatten
  // Build an interpreter to run the model with.*/
  static tflite::AllOpsResolver micro_op_resolver;
  static tflite::MicroInterpreter static_interpreter(
    model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;
  
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN,HIGH);
  //  return;
  }
  
  model_input = interpreter->input(0);
  if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] != 952) ||
      (model_input->dims->data[2] != totChannels) ||
      (model_input->type != kTfLiteFloat32)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");
    return;
  }

  input_length = model_input->bytes / sizeof(float);

  TfLiteStatus setup_status = SetupIMU(error_reporter);
  if (setup_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Set up failed\n");
  }
}

void loop()
{
  bool got_data = ReadData(error_reporter, model_input->data.f, input_length);
  if(!got_data)
  {TF_LITE_REPORT_ERROR(error_reporter, "New Data Not Available\n");
    return;

  }
  // If there was no new data, wait until next time.
  
  // Run inference, and report any error.
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) 
  {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on index: %d\n",
                         buff_ind);
    return;
  }
  //if (fall_det==true && (millis()-previous_time< 3000))
  //{
   // return; 
  //}
  static int8_t detection = detector(interpreter->output(0)->data.f);
  digitalWrite(LEDG,LOW);
  delay(100);
  digitalWrite(LEDG,HIGH);
  delay(100);
  TF_LITE_REPORT_ERROR(error_reporter, "Detection: %f\n",
                         interpreter->output(0)->data.f);
  //TF_LITE_REPORT_ERROR(error_reporter, "Input Array Sum: %f\n",
  //                       summed);
  if (detection == 1) {
    fall_det=false;
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB,HIGH);
  }
  
  if (detection == 0 )
  {
    fall_det=true;
    //previous_time=millis();
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR,HIGH);
  }

}
