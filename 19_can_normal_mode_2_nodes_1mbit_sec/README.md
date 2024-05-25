In previous project, we tested operation of CAN Normal Mode by using two Nodes at 500 kbps of bit rate. In this project we will increase that bitrate achieve up to 1 Mbit/sec.     
     
> [>IMPORTANT]    
> Remember, in the previous project we tested the Nodes by placing them very close to each other around 10 inches, However if you increase the distance like 1 meter, 2 meter, or 10 meters between the Nodes then the higher bit rates may not work. Hence you have to play with the numbers related to CAN bit timings like Prescaler, SyncJumpWidth TimeSeg1, and TimeSeg2.     
     
Goto [bitiming can wiki](http://www.bittiming.can-wiki.info) and select _ST Microelectronics bxCAN_, Clock Rate as _25_MHz, Sample-Point as 87.5, and SJW as 1 and click _Request Table_ button. Generated table show the maximum CAN you can achieve is 500Kbps. Lets increase the Clock speed to 84MHz, thereby you can get 42MHz on APB1 Bus where CAN is hanging also change this in the code with macro we already define `SYS_CLOCK_FREQ_84_MHZ`.     







     





     





   