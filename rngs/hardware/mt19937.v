`ifndef __MERSENNE_TWISTER_19937_V__
`define __MERSENNE_TWISTER_19937_V__

`include "common/register.v"

module mt19937
#(  parameter         num_state_words = 32'd624,
    parameter         period          = 32'd397 
)
(
    input wire        clk,         // clock
    input wire        rst_n,       // active-low synchronous reset

    input wire [31:0] seed,        // seed to reset state register S1
    input wire        re_seed,     // active high to reset S1 with seed (S2 & S3 unchanged)

    output wire [31:0] rnd         // random number output
);

    // mt19937 constants
    localparam state_bitwidth = num_state_words* 32; // 624 32-bit words
    localparam C1 = 'h9d2c5680;
    localparam C2 = 'hefc60000;

   

    reg [state_bitwidth-1:0] state;
    wire [state_bitwidth-1:0] next_state;

    reg [$clog2(num_state_words)-1:0] rnd_count;
    wire [$clog2(num_state_words)-1:0] rnd_count_next;

    assign rnd_count_next = (rnd_count == num_state_words-1) ? 0 : rnd_count + 1;

    Register
    #(
        .NUM_BITS ($clog2(num_state_words)),
        .RST_VAL  ({$clog2(num_state_words){1'b0}})
    ) indexReg (
        .clk        (clk),
        .rst_n      (rst_n),
        .write_data (rnd_count_next),
        .read_data  (rnd_count)
    );

    // ---------- Full State Register --------------------
    Register
    #(
        .NUM_BITS (state_bitwidth),
        .RST_VAL  ({(state_bitwidth){1'b0}})
    ) state_regs (
        .clk        (clk),
        .rst_n      (rst_n),
        .write_data (next_state),
        .read_data  (state)
    );
    // ---------------------------------------------------

    // Next state logic
    // TODO need to do all the complicated reloading and twisting here
    assign next_state = (rnd_count == num_state_words-1) ? state : state;

    // Output logic
    wire [31:0] cur_state_word, S1, S2, S3;
    assign cur_state_word = state[rnd_count*32 +: 32];
    assign S1 = cur_state_word ^ (cur_state_word >> 11);
    assign S2 = S1 ^ (S1 <<  7) & C1;
    assign S3 = S2 ^ (S2 << 15) & C2;
    assign rnd  = S3 ^ (S3 >> 18);

endmodule

`endif // __MERSENNE_TWISTER_19937_V__