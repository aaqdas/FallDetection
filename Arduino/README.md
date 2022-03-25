
# Fall Detection on Arduino
The files in this directory are an implementation of Fall Detection on **Arduino Nano 33 BLE Sense**. We have used Tensorflow Lite for Microcontrollers to implement our deep learning model in Arduino which is powered by an nRF52840 microcontroller.

# Results
Upon providing a simulated fall, our algorithm works by generating different colours on LED. When falls are detected we **Red** output whereas for Activities of Daily Life (ADLs) we get **Blue** output.
<p float="left">
<img width="470" alt="Fall" src="https://user-images.githubusercontent.com/65295655/160168761-e9745568-c879-434d-8281-18e5e90f0c6a.png">
<img width="520" alt="ADLs" src="https://user-images.githubusercontent.com/65295655/160169349-16e68c64-736d-45f2-96e9-798107ca8eb5.png">
</p>

