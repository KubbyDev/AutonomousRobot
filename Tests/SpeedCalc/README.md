This test script should be used to calibrate the control algorithm
First set the time to 3000 and measure the distance traveled (-> D) and the rotation made (-> R)
Then set the time to 1000 and measure the distance traveled (-> d) and the rotation made (-> r)

T and t are the times (3000 and 1000 but in seconds -> 3 and 1)

vmax = (D-d)/2
turnmax = (R-r)/2
ta = (2\*D - 6\*d)/(D - d)

Vmax should go to ROBOT_SPEED
ta should go to ACCEL_DURATION (convert it from seconds to microseconds)
