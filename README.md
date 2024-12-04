## Step by Step Importing setup

1. Ensure that you have installed the Code composer installed and also install de c2000 SDK. it will download the main archives to run the compiler and programs for the F28379D

2. Clone this repo and import the desired folders separately into Code Composer. The template folders are DSP_CPU 1 and 2.

3. Try to build the program. If you get some errors, look at the project folder inside the Project Explorer window, right click on it > **Properties**. Or just select the imported folder and press alt+enter.

### Properties > CCS General

Ensure that this configuration is applied

1. Product > Select C2000WARE
2. Project > Select F28379D
3. Project > Assign linker cmd file to the cmd file that is in the CPU 1 folder

### Properties > Resource > Linked

Here you must create project variables that will link to the main folders of F28379D libraries. Or you may ensure that those variables are linking to existing folders.

1. 	Variable Name: INSTALLROOT_F28379D: Inside c2000 SDK/device_support/f2837xd

	If you make a normal instalation process, this will be already ok.

2. 	Variable Name: INSTALLROOT_TO_FPU: Inside c2000 SDK/libraries/dsp/fpu/c28

	If you make a normal installation process, this will be already ok.

### Properties > CCS Build > Processor options

* 	Ensure that CLA, FPU32, and TMU are selected.
