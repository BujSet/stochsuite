// LCG: Linear Congruential Generator
// Hardware version matching C++ LCG::_read_random()
// State size: 32 bits stored in RNGState
// next = ((mul * state) + add) & mod

module lcg_core (
    input  wire        clk,
    input  wire        rst_n,     // active-low synchronous reset

    input  wire [31:0] seed,      // seed for reseeding
    input  wire        re_seed,   // high = overwrite state with seed

    // LCG parameters (can be constants or configurable)
    input  wire [31:0] mul,
    input  wire [31:0] add,
    input  wire [31:0] mod_mask,  // corresponds to _mod (C++ uses bitmask &)

    output wire [31:0] rnd,
    output wire        rnd_valid
);

    // Initial seed value (matches C++ constructor)
    localparam [31:0] S_INIT = 32'd1634404289;

    // ---- state wires ----
    wire [31:0] s_cur;
    wire [31:0] s_next;
    wire [31:0] s_wd;
    wire        s_we;

    // ---- 32-bit RNGState instance ----
    RNGState #(
        .NUM_BYTES  (4),
        .TOTAL_BITS (32)
    ) state_s (
        .clk        (clk),
        .rst_n      (rst_n),
        .w_en_bytes (s_we),
        .w_data_bytes(s_wd),
        .q_bytes    (s_cur)
    );

    // ---- 64-bit multiply + add (C++: uint64_t internal) ----
    wire [63:0] mult_result = s_cur * mul;
    wire [63:0] internal    = mult_result + add;

    // ---- apply modulus mask (C++: internal & mod_mask) ----
    assign s_next = internal[31:0] & mod_mask;

    // ---- write data mux (reset + reseed) ----
    assign s_wd = (~rst_n) ? S_INIT :
                  (re_seed ? seed : s_next);

    // Always write (because LCG updates every cycle)
    assign s_we = 1'b1;

    // ---- outputs ----
    assign rnd       = s_next;
    assign rnd_valid = rst_n;

endmodule
