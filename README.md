## Q1 - Two interfaces ADC sending data over TCP

This video highlight the code implementation and at the end I compile the code
in two variante MCU and SPI and runs the code in splitted CLI.
I also shows the timing of each adc channel capture.

link to the code repo: [Q1 task code](https://github.com/pslavkin/beam_trail_test/tree/main/q1)

<p align="center">
  <img src="../q1/q1_demo.gif" title="Q1 task demo">
</p>


## Q2 - led connection options

#### vcc to led option

pros:
- in case the 5v rail that feed to the led comes from another independent regulator than the
  uC, it will not affect the uC and could be turned on and off without affecting
  the uC
- some uC has more current capabilities when switch to gnd than vcc

cons:
- the logic of the sowfware will be negated (not a big deal... but)
- when the uC is blank and before it's pin is configured, the led could draw some
  current depending on the reset state of this gpio (general HI-Z). This could
  affect some super low power applications and also could be seen in a dark
  environment
- it's more exposed to a spike in the 5v rail that could affect the uC
- when uC turn pin on it will not be exactly 5.0v, it will be less depending on
  the push-pull output impedance of the uC, so the led could draw some current
  even if the uC pin is in HI state


<p align="center">
  <img src="../q2/vcc_to_led.png" title="vcc to led option">
</p>

#### led to gnd option

pros:
- the logic of the sowfware will be respected
- when the uC is blank and before the pin is configured, it's less common that a current draw from HI-Z pin through gnd
- when uC turn pin to LOW, there is garantee a 0.0 A current draw to the led
- when uC turn pin on it will be exactly 5.0v, so the led will draw the
- gnd is more stable than vcc, so it's less exposed to a spike in the 5v rail
  that could affect the uC
- gnd plane is everywhere, so it's easier to connect the led to gnd than to vcc

cons:
- the led will draw current from the internal uC logic and generally you have a
  total current limit summing all the pins, so if you have other pins drawing
  current, this led will decrease the total current available for the other pins

<p align="center">
  <img src="../q2/led_to_gnd.png" title="led to gnd option">
</p>


## Q3 - Dynamic memory allocation in embedded systems

#### A 
    One of the main reasons to avoid dynamic memory allocation in embedded
    systems is that it's hard to predict the behavior of the system when the
    memory is fragmented and the system is running out of memory, or 'cause a
    memory leak. 

    In real time systems, it's important to know the maximum time that a task
    will take to execute, but with dynamic memory allocation is hard to predict
    the time that it will take to allocate memory and it's hard to predict the
    time that it will take to free memory.
    
    If you have a full static memory allocation system, you can predict the
    maximum time that a task will take to execute, avoiding real time problems,
    and never experience memory fragmentation.

    but some time the total memory of the system is less than the sum of all task
    memory requirements at compile time, so you need to share the resource using
    alloc/free memory at run-time.

    It's a good practice to use dynamic allocation only for those process that
    doesn't need real time and let static memory for the critical parts

    There is also other dynamic memory allocation strategies like grids that
    reduce some of the problems

#### B 
Is it a valid declaration on a machine which is not 16 bit?
Give an explanation. Propose proper declaration. 
```
    unsigned int null = 0; 
    unsigned int complement2zero_by_ones = 0xFFFF; // 1′ s complement to zero
```
