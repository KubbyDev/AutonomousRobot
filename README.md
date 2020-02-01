# Autonomous Robot

This project is a robot that uses an ultrasonic sensor and an Arduino to create a 2D map of its environment. It is then able to use this 2D map to move from one point to another using pathfinding. To be controlled, the robot's ESP8266 hosts a Wifi network that the user can connect to using a smartphone app. The app requests and displays the 2D map of the robot and its expected position in the map. The user can then click on the place he wants the robot to move to.

## Demo

For the moment the obstacle avoiding doesn't work and the map updating is really bad. But I managed to get the communication and the navigation working so I made this short demo video
[![Demo 1 - Navigation](https://res.cloudinary.com/marcomontalbano/image/upload/v1580560084/video_to_markdown/images/youtube--F6cUCP5h3g4-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://www.youtube.com/watch?v=F6cUCP5h3g4 "Demo 1 - Navigation")

## How does this work

Software used:  
For the ESP and Arduino programs: The Arduino IDE  
For the smartphone app: Android Studio  
For the model: Blender and PrusaSlicer  

I used an Arduino Mega+Wifi which is an Arduino Mega with an ESP8266 integrated. But the project is designed to work on an Arduino Uno with a separate ESP module (Thats what I wanted to do in the beginning but I used the Mega+Wifi for simplicity). So the control program is designed to work with only 10 digital pins and only 2048 bytes of dynamic memory.

### Environment map

At each navigation update, the last distance recorded by the ultrasonic sensor is used to update the main map (if there has been a measure since the last update). All the pixels between the position of the robot and hit point are turned off and the pixel on the hit point is turned on.  

The main map is 72 by 72 pixels. If each pixel was stored in one byte, it would take 5184 bytes which is 2.5 times the max memory of the Uno. So I made an utility file to manage boolean matrices. What it does is simply use all the bits it has, by storing 8 booleans on each byte. With that solution the space used by the main map drops to 649 bytes.

There is also a second map, in lower resolution (24*24) but with numbers (unsigned char) instead of booleans and used only for the pathfinding. This map is updated at the same time as the main map, but following a different rule: Each pixel on this map is turned on if there is at least one pixel that is on in the 5x5 square at the corresponding position in the main map. For example the pixel at (5,5) in the low res map will check all the pixel between (5\*3 -1, 5\*3 -1) and (5\*3 +3, 5\*3+3) in the main map.

The sonar launches a measure every 25 milliseconds minimum (23 ms = 4 meters round trip for sound). When it starts a measure, it will just send a sound wave. Then, when the echo of the sound wave is received, the sensor will send a signal on the echo pin. The echo pin has a PCINT set up, so when the signal is received, the Arduino stops everything and calls a function. This function simply calculates the distance based on the time the sound wave took to go back to the sensor.

### Navigation

The memory space is very limited, so I had to find a pathfinding algorithm that works directly on the low res map (without storing too much things beside). The solution I came up with is not good in terms of performance but it meets this goal. The algorithm goes through the map and for each pixel, if a neighbour pixel has a distance value that is less or equal to its own distance value, it takes the neighbours pixel's value+1. And the algorithm does it repeatedly until there is no more change. The worst case scenario's complexity is n^2, but in practise it is often much faster because a lot of pixels are updated on each iteration. Also the order of the update changes between each iteration to maximise the chances of updating multiple pixels per iteration.  

After execution of this algorithm, the low res map will have 0 on the target position, 255 on the walls and the distance to the target on every other pixel. The low res map is then used by the robot to know in which direction it has to move. It finds the pixel that has the lowest value around him and goes towards it.

To keep track of the position of the robot on the map the control algorithm has to simulate the motors. I filmed the robot during an acceleration (using SpeedCalc.ino) and then I used Avimeca (old french software) and Excel to get the speed of the robot as a function of time. I found that it accelerates linearly for the first 0.7 seconds and then goes at constant speed. So I programmed this in the PositionTracker to increase its accuracy. I also added a waiting time every time the robot changes direction to reduce inaccuracies due to residual movement.

### Control

The goal of the communication part is too make the arduino exchange data with the smartphone app.  
The arduino itself doesn't have a direct access to a wifi antenna, so we need to put an intermediary: the ESP8266. On the Mega+Wifi it is already on the board, but it is still a separated thing. To make it communicate with the Arduino we have to use Serial communication.  
So the ESP8266 hosts a webserver and a wifi network. The smartphone app can then connect to this wifi network and send HTTP requests to the web server. The ESP will receive those requests and use Serial to ask the Arduino for an answer, and send back the answer to the phone via wifi.

The ESP server can respond to 3 requests:

- /map: Returns the intern map stored on the Arduino in the form of hexadecimal digits (one digit for 4 pixels)
- /set_target?x=000&y=000: Changes the goal location of the robot
- /position: Returns the expected position and rotation of the robot in its intern map.

The smartphone app is fairly simple. It keeps a copy of the robot's map and its position in memory and dislpays it to the user. At regular intervals, it sends a position and a map request to update it. If the user clicks on the screen, it will send a set_target request with the location where the user clicked. The requests are sent with a librairy called Volley.

When the ESP receives the request, it will check for errors and then send a "command" via Serial to the Arduino. The commands that can be sent are M to get the map, P to get the position and T\<x\>,\<y\> to change the target. All the messages exchanged via Serial end by a \n.  
If the request is a set_target the response to the HTTP request will just be "Target changed" but if it is a map or a position request the ESP will record everything the Arduino sends via Serial until it finds a \n, and then send the recorded message as a response.

The Arduino is constantly listening for requests from the ESP. When it receives a request, it responds immediatly. The only important thing to note is that the writing Serial buffer is limited in size (256 bytes for the Mega I think), and the answer to a map request takes 1296+1 bytes. The solution to this problem is to make the ESP read as the Arduino writes. I even added a security in case the Arduino writes faster than the ESP can read: The Arduino writes blocks of 16 bytes and then calls flush to wait for the ESP to finish reading the current block.

## Reproduce this project

### The way I did it

You will need:

- 1 Arduino Mega with ESP module integrated  
- 1 OSOYOO Model X motors controller  
- 2 DC 3V-12V dual axis gear reducer motors (the yellow ones)  
- 1 HC-SR04 ultrasonic sensor (with support, or you will have to print it)  
- 1 Hammer wheel  
- 8 1.5V batteries with a 8 slots support or two 4 slots supports  
- 1 Switch  
- 22 M2 bolts with nuts (I used 12 mm but 8 mm is enough)
- An access to a 3D printer

If you also want to make the bonus (esthetics) part:  

- An SG90 servo motor

#### 3D Printing and Assembling

If you want to 3D print the robot you can use the 3D files in Models/. The parts in Body/ are the minimum you need to print to get the robot working. The parts in Esthetics/ are only esthetic. Thanks to Saban for the model I used for the WALL-E parts https://www.myminifactory.com/object/3d-print-wall-e-3971. You can find printing recommendations in the Models/Info/Recommendations.txt file.

The assembly is pretty hard but if you have patience and method it's doable. I recommend using a screwdriver and touching the motors with it to magnetise it so you can place bolts and nuts in the right place without too much effort.  
The 4 holes in line on the front are for the sonar. You will only use 2 but if you use an Arduino Mega you will have to use the holes that are not in the middle (you can use the ones in the middle if you use an UNO).
When placing the servo in the main box of the esthetic part, place it on the left of the supports. If you place it wrong the short arm will be too short and the long arm will be too long.

#### Software upload

First you will need to measure the robots speed. Upload the Tests/SpeedCalc/SpeedCalc.ino program in the Arduino. The robot will go forward for 3 seconds, then backwards for 3 seconds, then turn to the left for 3 seconds and turn to the right for 3 seconds. Make sure it does it in the right order. If it doesn't, invert the + and the - of the motors. Measure the distances the robot travels and the angles it makes when it turns. Then open ArduinoProgram/Data.h and put the values you found for the speed and angular speed (pay attention to the units).  
When this is done you just have to upload the code to the Arduino and the ESP.

To upload the code to the Arduino:  
On the Mega, set the switches 3 and 4 to ON, all the rest to OFF (connects the Arduino to the USB port)  
In Sketch > Include Library > Manage Librairies, install PinChangeInterrupt by NicoHood  
Make sure you selected the right board in Tools/Board.

To upload the code to the ESP:  
In File > Preferences > Additionnal Boards Manager URLS, put this: https://arduino.esp8266.com/stable/package_esp8266com_index.json  
In Tools > Board > Boards Manager, install esp8266 by ESP8266 Community  
On the Mega, set the switches 5, 6 and 7 to ON, all the rest to OFF (connects the ESP to the USB port, in firmware update mode)  
Make sure you selected the Generic ESP8266 module.

To make it run you have to set the switches 1 and 2 to ON (you can leave the 3 and 4 also ON to be able to reupload code to the Arduino without changing the switches).

#### Phone app

It is probably possible to transfer an apk or something to your phone to install the app but I don't know how to do it.  
To install the app I used Android studio. Open the ControlApp folder with android studio, plug your phone to your computer via USB (you have to activate developper mode on your phone), and press the run button. It will transfer all the necessary data to your phone and launch the app. For the next launches you don't need the computer anymore.

### Possible changes

You can replace the Mega+ESP board with a regular Mega or an Uno board with a separate ESP module. That's what I wanted to do at the beginning, but I didn't manage to upload code to the ESP with this technique (I tried to plug it to the PC with a USB adapter, and I also tried to connect it to the arduino in serial and connect the arduino to the PC. Both methods without success).  
The control algorithm works on the Mega+Wifi and is supposed to work on the normal Mega and on the Uno without changing the pin numbers. If you want to change anything, go to ArduinoProgram/Data.h. If you want to use a separate ESP module, you might also have to change the Serial connection to the ESP on the Arduino side. Go to ArduinoProgram/Communication.cpp and change the SERIALOBJECT.  
The holes in the robot's body can be used for an Uno or a Mega (and maybe some other board but they work for these ones).

You can replace the motors controller by another motors controller, it is not that important. But the holes in the robot models were made for this board so you might have to change them. The motors can be any type of motors but again, you might have to change the motors support model if you want to 3D print it. Just make sure they can endure 12V and they don't turn too fast.

The free wheel can be anything but you might have to modify the model so the front is at the same height as the back. It is important for the sonar (but if it is simpler for you, you can also change its angle).