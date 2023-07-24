#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0x04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

int main() {

	void *virtual_base;
	int fd;
	int loop_count;
	int led_direction;
	int led_mask;
	void *h2p_lw_led_addr;
	void *h2p_lw_coproc_addr_memory;

	printf("Calling fopen\n");

	if ((fd = open ("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
		printf("ERROR: could not open \"/dev/mem\"...\n");
		return(1);
	}

	printf("Creating mmap\n");

	virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

	if (virtual_base == MAP_FAILED) {
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return(1);
	}

	h2p_lw_led_addr = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + PIO_LED_BASE) & (unsigned long)(HW_REGS_MASK));

	// toggle the LEDs a bit

	loop_count = 0;
	led_mask = 0x01;
	led_direction = 0; // 0: left to right direction
	printf("Starting while loop, base address: %p, virtual_base = %p\n", h2p_lw_led_addr, virtual_base);
	while(loop_count < 1) {
    		printf("In Loop, loop count: %d, led_mask:%d, led_direction:%d\n", loop_count, led_mask, led_direction);
    		// control led
    		*(uint32_t * )h2p_lw_led_addr = ~led_mask;
		
		// wait 100ms
    		usleep(100*1000); 

    		// update led mask
    		if (led_direction == 0) {
        		led_mask <<= 1;
        	if (led_mask == (0x01 << (PIO_LED_DATA_WIDTH-1)))
            		led_direction = 1;
    		} else {
        		led_mask >>= 1;
       			if (led_mask == 0x01){
            			led_direction = 0;
            			loop_count++;
        		}	
    		}
	} // while

// ------------------------------------- //
	int mem_data, mm_reg, i;

	// Ask for ptext
	printf ("Enter ptext in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 0; i < 15; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}
	
	// Ask for key
	printf ("Enter key in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 16; i < 31; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Ask for IV
	printf ("Enter IV in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 32; i < 47; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Print out encrypted text for block 1
	printf ("Encrypted data 1: \n");
	for (i = 48; i < 61; i = i + 4) {
		mm_reg = i;
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		mem_data = *(uint32_t *)h2p_lw_coproc_addr_memory;
		printf("ADDRESS: %p\tDATA: %x\n", h2p_lw_coproc_addr_memory, mem_data);
	}
	printf ("\n");

	// Ask for ptext2
	printf ("Enter second ptext in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 64; i < 77; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Print out encrypted text for block 2
	printf ("Encrypted data 2: \n");
	for (i = 80; i < 93; i = i + 4) {
		mm_reg = i;
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		mem_data = *(uint32_t *)h2p_lw_coproc_addr_memory;
		printf("ADDRESS: %p\tDATA: %x\n", h2p_lw_coproc_addr_memory, mem_data);
	}
	printf ("\n");

	printf("-------------------------------------\n");
	for (i = 0; i < 93; i = i + 4) {
		mm_reg = i;
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_0_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		mem_data = *(uint32_t *)h2p_lw_coproc_addr_memory;
		printf("ADDRESS: %p\tDATA: %x\n", h2p_lw_coproc_addr_memory, mem_data);
	}

// ------------------------------------- //
/*
	int mem_data, mm_reg, i;

	// Ask for ptext
	printf ("Enter ptext in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 0; i < 16; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Ask for key
	printf ("Enter key in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 16; i < 32; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Ask for IV
	printf ("Enter IV in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 32; i < 48; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Print out encrypted text for block 1
	printf ("Encrypted data 1: \n");
	for (i = 48; i < 64; i = i + 4) {
		mm_reg = i;
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		mem_data = *(uint32_t *)h2p_lw_coproc_addr_memory;
		printf("ADDRESS: %p\tDATA: %x\n", h2p_lw_coproc_addr_memory, mem_data);
	}
	printf ("\n");

	// Ask for ptext block 2
	printf ("Enter ptext2 in hexadecimal. ONLY enter 32 bits at a time NO SPACES (4 lines of 8 hex characters).\n");
	for (i = 64; i < 80; i = i + 4) {
		mm_reg = i;
		scanf("%x", &mem_data);
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		*(uint32_t *)h2p_lw_coproc_addr_memory = mem_data;
	}

	// Print out encrypted text for block 2
	printf ("Encrypted data 2: \n");
	for (i = 80; i < 96; i = i + 4) {
		mm_reg = i;
		h2p_lw_coproc_addr_memory = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + MY_AES128_IP_BASE + mm_reg) & (unsigned long)(HW_REGS_MASK));
		mem_data = *(uint32_t *)h2p_lw_coproc_addr_memory;
		printf("ADDRESS: %p\tDATA: %x\n", h2p_lw_coproc_addr_memory, mem_data);
	}
	printf ("\n");		
*/
// ------------------------------------- //

	// clean up our memory mapping and exit
    	if( munmap(virtual_base, HW_REGS_SPAN) != 0) {
    		printf("ERROR: munmap() failed... \n");
    		close(fd);
    		return(1);
	}

	close(fd);
	//free(status_reg);
	return(0);
}