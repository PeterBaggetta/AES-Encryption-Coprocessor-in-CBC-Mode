# AES-Encryption-Coprocessor-in-CBC-Mode
- Implemented an AWS coprocessor on a DE1-SoC development board which uses an Altera SoC with dual-core Cortex A-9 embedded cores
- The project was developed using Verilog and C code within the Quartus Prime II FPGA Software IDE.

In this lab we are tasked to create an Advanced Encryption Standard coprocessor in CBC mode using the DE1-SoC board. To implement the AES coprocessor onto our FPGA development board, we had to implement a simple APB IP module to interface between our software and hardware. After completion of the APB IP module, our hardware from the DE1-SoC board will be able to communicate to the software on the board and read in values that are entered through the terminal. The understanding of how the hardware and software on an FPGA board communicate are important towards our understanding of how systems work with one another. Understanding this complex project implemented on the SoC board allows us to build and use design techniques learned in our courses and previous labs and working with a memory-mapping coprocessor module enables us to implement virtually any implementation we choose.

<h2 align="center">To view more information about the lab and results please view the lab report within the repository </h2>

This lab was completed on April 6, 2023 by Peter Baggetta and Owen Walsh from the University of Guelph. Any copying of our work is considered copywriting. 
<p align="center">&copy; University of Guelph, April 6, 2023</p>
