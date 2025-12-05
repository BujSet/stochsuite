// Taus113 pseudo-random number generator (hardware version)
// Equivalent to C++ Taus113::_read_random()
// Four 32-bit state registers updated each cycle.
// Output = XOR of updated S1,S2,S3,S4.

module taus113_core (
    input  wire        clk,
    input  wire        rst_n,      // active-low synchronous reset

    input  wire [31:0] seed,       // seed for S1 only (same as C++ _seed_random)
    input  wire        re_seed,    // high = overwrite S1 with seed

    output wire [31:0] rnd,
    output wire        rnd_valid
);

    // ===== Taus113 constants (YOU MUST FILL THESE!) =====
    localparam [31:0] C1 = 32'd4294967294;
    localparam [31:0] C2 = 32'd4294967288;
    localparam [31:0] C3 = 32'd4294967280;
    localparam [31:0] C4 = 32'd4294967168;

    // ===== Default seeds (match C++ constructor) =====
    localparam [31:0] S1_INIT = 32'd16289;
    localparam [31:0] S2_INIT = 32'd8;
    localparam [31:0] S3_INIT = 32'd16;
    localparam [31:0] S4_INIT = 32'd128;

    // ===== State register wires =====
    wire [31:0] S1_cur, S2_cur, S3_cur, S4_cur;
    wire [31:0] S1_next, S2_next, S3_next, S4_next;
    wire [31:0] S1_wd, S2_wd, S3_wd, S4_wd;
    wire        S1_we, S2_we, S3_we, S4_we;

    // ===== RNGState array instantiation (4 × 32-bit) =====
    RNGState #(
        .NUM_BYTES  (4),
        .TOTAL_BITS (32)
    ) state_array [3:0] (
        .clk        (clk),
        .rst_n      (rst_n),
        .w_en_bytes ({S1_we, S2_we, S3_we, S4_we}),
        .w_data_bytes({S1_wd, S2_wd, S3_wd, S4_wd}),
        .q_bytes    ({S1_cur, S2_cur, S3_cur, S4_cur})
    );

    // ===== Taus113 update logic =====
    assign S1_next = ((S1_cur & C1) << 18) ^ (((S1_cur <<  6) ^ S1_cur) >> 13);
    assign S2_next = ((S2_cur & C2) <<  2) ^ (((S2_cur <<  2) ^ S2_cur) >> 27);
    assign S3_next = ((S3_cur & C3) <<  7) ^ (((S3_cur << 13) ^ S3_cur) >> 21);
    assign S4_next = ((S4_cur & C4) << 13) ^ (((S4_cur <<  3) ^ S4_cur) >> 12);

    // ===== Write data mux (sync reset + reseed) =====
    assign S1_wd = (~rst_n) ? S1_INIT :
                   (re_seed ? seed : S1_next);

    assign S2_wd = (~rst_n) ? S2_INIT : S2_next;
    assign S3_wd = (~rst_n) ? S3_INIT : S3_next;
    assign S4_wd = (~rst_n) ? S4_INIT : S4_next;

    // ===== All writes enabled (matches C++ "always update") =====
    assign S1_we = 1'b1;
    assign S2_we = 1'b1;
    assign S3_we = 1'b1;
    assign S4_we = 1'b1;

    // ===== Output = XOR of updated states =====
    assign rnd = S1_next ^ S2_next ^ S3_next ^ S4_next;

    // Output valid once reset is released
    assign rnd_valid = rst_n;

endmodule
