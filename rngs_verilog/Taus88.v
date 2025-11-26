// Taus88 pseudo-random number generator (hardware version)
// Equivalent to the C++ Taus88::_read_random() logic.
// Three 32-bit state registers updated each cycle when `next` is high.
// Output = XOR of the three updated states.

module taus88_core (
    input wire        clk,         // clock
    input wire        rst_n,       // active-low synchronous reset

    input wire [31:0] seed,        // seed to reset state register S1
    input wire        re_seed;     // active high to reset S1 with seed (S2 & S3 unchanged)

    output reg [31:0] rnd,         // random number output
    output reg        rnd_valid    // output valid flag
);

    // Taus88 constants
    localparam [31:0] C1 = 32'hFFFFFFFE;
    localparam [31:0] C2 = 32'hFFFFFFF8;
    localparam [31:0] C3 = 32'hFFFFFFF0;

    // Default seeds
    localparam [31:0] S1_INIT = 32'd1634404289;
    localparam [31:0] S2_INIT = 32'd8;
    localparam [31:0] S3_INIT = 32'd16;

    wire [31:0] S1_cur, S2_cur, S3_cur;             // current S1, S2, S3 value from RNGState
    wire [31:0] S1_wd, S2_wd, S3_wd;                // S1, S2, S3 value to write into RNGState
    wire        S1_we, S2_we, S3_we;                // per-byte write enable for S1, S2, S3

    // ---------- RNGState Module Instantiation ----------
    RNGState #(
        .NUM_BYTES  (4),
        .TOTAL_BITS (32)
    ) state_s2 [2:0] (
        .clk          (clk),
        .rst_n        (rst_n),
        .w_en_bytes   ({S1_we, S2_we, S3_we}),
        .w_data_bytes ({S1_wd, S2_wd, S3_wd}),
        .q_bytes      ({S1_cur, S2_cur, S3_cur})
    );
    // ---------------------------------------------------

    // Taus88 next random number algorithm
    wire [31:0] S1_next = ((S1_cur & C1) << 12) ^ (((S1_cur << 13) ^ S1_cur) >> 19);
    wire [31:0] S2_next = ((S2_cur & C2) <<  4) ^ (((S2_cur <<  2) ^ S2_cur) >> 25);
    wire [31:0] S3_next = ((S3_cur & C3) << 17) ^ (((S3_cur <<  3) ^ S3_cur) >> 11);

    // Write data mux
    assign S1_wd = (~rst_n) ? S1_INIT : 
                    re_seed ? seed    : S1_next;
    assign S2_wd = (~rst_n) ? S2_INIT : S2_next;
    assign S3_wd = (~rst_n) ? S3_INIT : S3_next;

    // Write enable signals
    assign S1_we = 1'b1;
    assign S2_we = 1'b1;
    assign S3_we = 1'b1;


    // Outputs
    assign rnd       = S1_cur ^ S2_cur ^ S3_cur;
    assign rnd_valid = rst_n;

endmodule