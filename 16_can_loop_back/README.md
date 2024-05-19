Create a CubeMx project and go to **Connectivity** > **CAN1** and click checkbox _Activated_. You will see CAN1_TX and CAN1_RX pin PA12 and PA11. These are a single ended digital signals and these pins should go to the transceiver. Below that we have _Configuration_ section where we do _Parameter Settings_, _NVIC Settings etc_. We need not to do anything here as we will do in code. Neither you need to do anything in the _Clock configuration_ Tab. Now _Generate the code_.   
      
Similar to other projects we will use our own files in Src and Inc folder (i.e. `main_app.c`, `msp.c`, `it.c`, `main_app.h`, and `it.h`). We will use HSE as a System clock Not HSI.      
      

   