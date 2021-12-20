# FallDetection

This repository is a modified implementation of FallAllD, M.Saleh et.Al [1] on Arduino Nano 33 BLE Sense.

## Tools Required
1. Google Colab
2. Arduino IDE
3. Arduino Nano 33 BLE Sense

## Dataset
The Jupyter Notebook utilizes the FallAllD Dataset publically available on [IEEE DataPort](http://ieee-dataport.org/2203). The notebook processes the useful data to generate a meaningful dataset.
Original Dataset contains data from IMU and Barometer however the notebook extracts only Accelerometer and Gyroscope Data as the remaining data appears to be meaningless. It can be seen from plotting the heatmap of the data.
![Heatmap of Fall Data](https://user-images.githubusercontent.com/65295655/146801710-973ec07c-1dc9-42a3-a907-aa705272cb53.png)
The time axis is shown along vertical axis while data channels along horizontal axis. Each data channel is split by a yellow line. The first three channels are acceleration, then barometer, the next three gyro and the last three are magnetometer.
It can be clearly seen that barometer shows almost no changes when a person falls. We also know that magnetometer gives us our heading from north and is therefore doesn't carry enough meaning in determining if a person has fallen.
Therefore we have utilized only Accelerometer and Magnetometer data only. We have further downsampled the data to **50 Hz** to reduce the overall computation cost. Otherwise we would have to increase the depth of our Neural Network.

## Model Architecture 
We have utilized a Convolutional Neural Network to predict the falls. The model is designed with following goals:
1. Reduce the Model Size to fit in our device.
2. Reduce the computation cost to make our predictions quicker.
3. Provide reliable predictions to detect falls.

## Model Optimizations
We convert our model to Tensorflow Lite and quantize our model by providing it a representative dataset. The quantized model is around **140 KB** in size. We then convert our model into a FlatBuffer to fit it in our device and interpret it using a Tensorflow Lite interpreter


## References
1. M. Saleh, M. Abbas and R. B. Le Jeannès, "FallAllD: An Open Dataset of Human Falls and Activities of Daily Living for Classical and Deep Learning Applications," in IEEE Sensors Journal, vol. 21, no. 2, pp. 1849-1858, 15 Jan.15, 2021, doi: 10.1109/JSEN.2020.3018335.
