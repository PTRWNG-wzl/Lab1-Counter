#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" // Insert Vbuddy code here.

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vcounter* top = new Vcounter;

    // init trace dump
    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");
    vbdSetMode(1);  // Set Vbuddy to ONE-SHOT mode

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;   // Initialize load signal
    top->v = 0;    // Initialize preload value

    // run simulation for many clock cycles
    for (i = 0; i < 300; i++) {
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // ++++ Send count value to Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, (int(top->count)) & 0xF);
        // vbdPlot(int(top->count), 0, 255); // Optionally plot the count
        vbdCycle(i);
        // ---- end of Vbuddy output section

        // Set load signal and load value based on Vbuddy flag
        top->ld = vbdFlag();          // Set load signal when flag is triggered
        top->v = vbdValue();          // Set load value from Vbuddy's input

        // change input stimuli
        top->rst = (i < 2) | (i == 15);  // Reset behavior, can be adjusted

        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose(); // housekeeping
    tfp->close();
    exit(0);
}