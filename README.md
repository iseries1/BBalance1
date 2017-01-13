# BBalance1
Balance Robot Version 1

The BRobot code has an accelerometer and gyro IMU the determines the angle the robot it at.  This drives
the two stepper motors to adjust the angle back to zero.

This code breaks out the code for the two stepper motors as one class and the IMU as the other class.

This can make it easier to understand the two functions and build new functions and understanding.

The code also outputs at regular intervals the values of the accelerometer and gyro.
It also outputs the calculated angle and PID value that can later be plotted.

Since the IMU is mounted sideways the Y access has the pitch angle and the x axes has the gyro value.

The angle is determine by using a simple complementary filter sampled at regular intervals.

