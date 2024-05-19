Create a CubeMx project and go to **Connectivity** > **CAN1** and click checkbox _Activated_. You will see CAN1_TX and CAN1_RX pin PA12 and PA11. These are a single ended digital signals and these pins should go to the transceiver. Below that we have _Configuration_ section where we do _Parameter Settings_, _NVIC Settings etc_. We need not to do anything here as we will do in code. Neither you need to do anything in the _Clock configuration_ Tab. Now _Generate the code_.   
      
Similar to other projects we will use our own files in Src and Inc folder (i.e. `main_app.c`, `msp.c`, `it.c`, `main_app.h`, and `it.h`). We will use HSE as a System clock Not HSI.      
      
One of the member of CAN Handle structure `CAN_HandleTypeDef` is `Init` which is type `CAN_InitTypeDef` and contains following members.     

```c
typedef struct
{
  uint32_t Prescaler;                  /*!< Specifies the length of a time quantum.
                                            This parameter must be a number between Min_Data = 1 and Max_Data = 1024. */

  uint32_t Mode;                       /*!< Specifies the CAN operating mode.
                                            This parameter can be a value of @ref CAN_operating_mode */

  uint32_t SyncJumpWidth;              /*!< Specifies the maximum number of time quanta the CAN hardware
                                            is allowed to lengthen or shorten a bit to perform resynchronization.
                                            This parameter can be a value of @ref CAN_synchronisation_jump_width */

  uint32_t TimeSeg1;                   /*!< Specifies the number of time quanta in Bit Segment 1.
                                            This parameter can be a value of @ref CAN_time_quantum_in_bit_segment_1 */

  uint32_t TimeSeg2;                   /*!< Specifies the number of time quanta in Bit Segment 2.
                                            This parameter can be a value of @ref CAN_time_quantum_in_bit_segment_2 */

  FunctionalState TimeTriggeredMode;   /*!< Enable or disable the time triggered communication mode.
                                            This parameter can be set to ENABLE or DISABLE. */

  FunctionalState AutoBusOff;          /*!< Enable or disable the automatic bus-off management.
                                            This parameter can be set to ENABLE or DISABLE. */

  FunctionalState AutoWakeUp;          /*!< Enable or disable the automatic wake-up mode.
                                            This parameter can be set to ENABLE or DISABLE. */

  FunctionalState AutoRetransmission;  /*!< Enable or disable the non-automatic retransmission mode.
                                            This parameter can be set to ENABLE or DISABLE. */

  FunctionalState ReceiveFifoLocked;   /*!< Enable or disable the Receive FIFO Locked mode.
                                            This parameter can be set to ENABLE or DISABLE. */

  FunctionalState TransmitFifoPriority;/*!< Enable or disable the transmit FIFO priority.
                                            This parameter can be set to ENABLE or DISABLE. */

} CAN_InitTypeDef;
```      

_Prescaler_, _SyncJumpWidth_, _TimeSeg1_, and _TimeSeg2_ members determine CAN **bit timings** and **bit rate** whereas _TimeTriggeredMode_, _AutoBusOff_, _AutoWakeUp_, _AutoRetransmission_, _ReceiveFifoLocked_, and _TransmitFifoPriority_ settings are used to configure the Tx and Rx engine.     

To understand **SyncJumpWidth**, **TimeSeg1**, and **TimeSeg2** settings, we will go to the [link](http://www.bittiming.can-wiki.info/) which is to explore the CAN Bit Time Calculation. And if you just take a look into this figure which is actually 1-bit.     

<img src="../images/image228.png" alt="Picture from can-wiki">          
     
Similarly the diagram you have previously seen also 1-bit (can be a dominant bit or recessive bit)     
      
<img src="../images/image229.png" alt="CAN frame format">       

Now, what is the width of that bit? that depends upon the clock frequency we use. Now take a look into following diagram. That one bit duration of CAN protocol or CAN message is divided into four segments          
      
<img src="../images/image230.png" alt="CAN bit Timings Configuration">      
      
So, all these segments are required for transmission as well as reception purposes. And these segments are actually decided by the specification. Now, these are required for the correct operation of the CAN bus (correct transmission as well as for reception).      
      
To determine the width of one time quanta, you have to calculate the clock frequency that you supply to the CAN peripheral. As in our application, We are using HSE to generate a PLL output clock of 50 MHz (HCLK is also 50 MHz as AHB Presecaler is 1 i.e. SYSCLK/1). Ultimately we are supplying 25 MHz to the APB1 bus (as HCLK/APB1 Prescaler is 1). As our CAN1 is actually hanging on APB1 bus according to the the reference manual. You also saw the `CAN_InitTypeDef` which contains member `Prescaler` that refelects the CAN's prescaler (as CAN comes with its own prescaler). By using that you can further slow down the clock. Let's say if that struct member `Prescaler` is 1 then 25 MHz will be supplied to the CAN peripheral. Hence you can increase or slow down the clock.     
      
Similarly if the APB1 clock is 25 MHz and `Prescaler` is 1:     
      
**Duration of 1 time quanta (1 TQ)** = PCLK1 /CAN_prescaler = 0.04 micro seconds      

Hence these segments (Sync, Prop-seg, Phase 1, and Phase 2) has to be mentioned in terms of number of time quantas. _Synchronization_ segment is always one time quanta and _Propagation_ segment maybe 5, _Phase 1_ segment maybe 10, and _Phase 2_ segment maybe 8 etc. Hence a bit is actually collection of all the segments or number of Time quantas.        
      
<img src="../images/image231.png" alt="CAN bit segments/time quantas increases">      

<img src="../images/image232.png" alt="CAN bit segments/time quantas decreases">            
      
Therefore if a signal has to propagate for a long distance you may face issues, If you shrink the bit timing just like that. Hence you have to a properly calculate it. And for that purpose, this [calculator](http://www.bittiming.can-wiki.info/) is given.    
     
- Select your CAN controller from dropdown which is _ST Microelectronics bxCAN_      
- Type your Clock rate which _25_ MHz     
- And click _Request Table_ button       
    
It gives you a table as shown below      
<img src="../images/image233.png" alt="time quantas calculation table at 25MHz">          
     
According to the table you cannot achieve 1 Mbps using the above configuration. However if you increase it to 50 MHz. Then you can see the table that you can achieve 1 MB (1000 Bit Rate) using the prescaler as 5 and number of time quanta will be 10, out of that 8 will be given to segment 1 (propagation segment and phase segment 1), 1 is given to Phase segment 2 and synchronization segment will always 1 time quanta, so 10 in total. However this table recommends us to use the reading which is highlighted by the yellow line where one bit is 16 time quantas. Then, it will work properly. But the thing is you have to reduce the time quantas and you have to test Whether other reading works on your application or not.    
<img src="../images/image234.png" alt="time quantas calculation table at 25MHz">         
                
   