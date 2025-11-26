#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "sleep.h"
#include "xparameters.h"

// --- Address Definitions ---
#define R_BASE XPAR_AXI_TIMER_0_BASEADDR
#define G_BASE XPAR_AXI_TIMER_1_BASEADDR
#define B_BASE XPAR_AXI_TIMER_2_BASEADDR
#define SW_BASE XPAR_AXI_GPIO_0_BASEADDR

// --- AXI Timer Register Offsets ---
#define TCSR_MDT   0x00000001   // bit 0
#define TCSR_UDT   0x00000002   // bit 1
#define TCSR_GENT  0x00000004   // bit 2
#define TCSR_CAPT  0x00000008   // bit 3
#define TCSR_ARHT  0x00000010   // bit 4
#define TCSR_LOAD  0x00000020   // bit 5
#define TCSR_ENIT  0x00000040   // bit 6
#define TCSR_ENT   0x00000080   // bit 7
#define TCSR_TINT  0x00000100   // bit 8
#define TCSR_PWMA  0x00000200   // bit 9
#define TCSR_ENALL 0x00000400   // bit 10

// Offsets
#define TCSR0 0x00
#define TLR0  0x04
#define TCR0  0x08

#define TCSR1 0x10
#define TLR1  0x14
#define TCR1  0x18

// --- Configuration Constants ---
#define PWM_ENABLE_MASK 0x00000296 
#define TIMER_PERIOD     50000
#define halfTIMER_PERIOD rr

// --- Function Prototypes ---
void setColor(u32 redDuty, u32 greenDuty, u32 blueDuty);
void configurePWM(u32 baseAddr, char* name); // Added name for debug
u32 readSwitches();

void breathe_red(u32 current_switches) {
    xil_printf("[BREATHE] Red Sequence Started...\r\n");

    // Fade In
    for(int i = 0; i < TIMER_PERIOD; i+=1000) {
        // Break immediately if user changes switches
        if(readSwitches() != current_switches) return;

        setColor(i, 0, 0); // Modulate RED channel
        usleep(1000);
    }

    // Fade Out
    for(int i = TIMER_PERIOD; i > 0; i-=1000) {
        if(readSwitches() != current_switches) return;

        setColor(i, 0, 0); // Modulate RED channel
        usleep(1000);
    }
}

// --------------------------------------------------------------------------
// MODULE: Breathe Green
// --------------------------------------------------------------------------
void breathe_green(u32 current_switches) {
    xil_printf("[BREATHE] Green Sequence Started...\r\n");

    // Fade In
    for(int i = 0; i < TIMER_PERIOD; i+=1000) {
        if(readSwitches() != current_switches) return;

        setColor(0, i, 0); // Modulate GREEN channel
        usleep(1000);
    }

    // Fade Out
    for(int i = TIMER_PERIOD; i > 0; i-=1000) {
        if(readSwitches() != current_switches) return;

        setColor(0, i, 0); // Modulate GREEN channel
        usleep(1000);
    }
}

// --------------------------------------------------------------------------
// MODULE: Breathe Blue
// --------------------------------------------------------------------------
void breathe_blue(u32 current_switches) {
    xil_printf("[BREATHE] Blue Sequence Started...\r\n");

    // Fade In
    for(int i = 0; i < TIMER_PERIOD; i+=1000) {
        if(readSwitches() != current_switches) return;

        setColor(0, 0, i); // Modulate BLUE channel
        usleep(1000);
    }

    // Fade Out
    for(int i = TIMER_PERIOD; i > 0; i-=1000) {
        if(readSwitches() != current_switches) return;

        setColor(0, 0, i); // Modulate BLUE channel
        usleep(1000);
    }
}

// --------------------------------------------------------------------------
// MAIN APPLICATION
// --------------------------------------------------------------------------
int main()
{
    init_platform();
    
    xil_printf("\r\n=========================================\r\n");
    xil_printf("   RGB Breathing Modules Application\r\n");
    xil_printf("=========================================\r\n");

    // Initialize Timers
    configurePWM(R_BASE, "RED");
    configurePWM(G_BASE, "GREEN");
    configurePWM(B_BASE, "BLUE");

    u32 switches = 0;
    u32 last_switches = 0xFFFFFFFF; 

    while(1) {
        switches = readSwitches();

        // Optional: Print Debug info for TCR if needed
        // u32 t0 = Xil_In32(R_BASE + 0x08);
        // xil_printf("   [DBG] TCR0 = %u\r\n", t0);

        // State Machine
        switch(switches) {
            case 0x01: // Switch 0 -> Breathe Red
                breathe_red(switches);
                break;

            case 0x02: // Switch 1 -> Breathe Green
                breathe_green(switches);
                break;

            case 0x04: // Switch 2 -> Breathe Blue
                breathe_blue(switches);
                break;

            default: // All other states -> Turn Off
                setColor(0, 0, 0);
                break;
        }

        // Print state change only once per change
        if(switches != last_switches) {
            xil_printf("\r\n[INPUT CHANGE] Switch State: 0x%02x\r\n", switches);
            last_switches = switches;
        }
    }

    cleanup_platform();
    return 0;
}
// --- Helper Functions ---

void configurePWM(u32 baseAddr, char* name) {
    xil_printf("   [CFG] Configuring %s...\r\n", name);

    // 1. STOP everything
    Xil_Out32(baseAddr + TCSR0, 0);
    Xil_Out32(baseAddr + TCSR1, 0); // Stop Timer 1 too

    // 2. Set Frequency (TLR0) and Duty Cycle (TLR1)
    Xil_Out32(baseAddr + TLR0, TIMER_PERIOD); 
    Xil_Out32(baseAddr + TLR1, TIMER_PERIOD / 2); // 50% Brightness
    Xil_Out32(baseAddr + TCSR1, 0x20);
    Xil_Out32(baseAddr + TCSR0, 0x20);

    Xil_Out32(baseAddr + TCSR1, 0x216);
    Xil_Out32(baseAddr + TCSR0, 0x216);
    Xil_Out32(baseAddr + TCSR1, 0x296);
    Xil_Out32(baseAddr + TCSR0, 0x296);

    xil_printf("         %s Configured (Both Counters Loaded).\r\n", name);
}

void setColor(u32 redDuty, u32 greenDuty, u32 blueDuty) {

    Xil_Out32(R_BASE + TLR1, redDuty);
    Xil_Out32(G_BASE + TLR1, greenDuty);
    Xil_Out32(B_BASE + TLR1, blueDuty);
}

u32 readSwitches() {
    u32 val = Xil_In32(SW_BASE);
    // Debug raw vs masked value (helps diagnose if upper bits are floating)
    // printf("[RAW SW] Read: 0x%08x | Masked: 0x%01x\r\n", val, val & 0x0F);
    return val & 0x0F; 
}