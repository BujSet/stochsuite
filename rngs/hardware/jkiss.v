`ifndef __JKISS_88_V__
`define __JKISS_88_V__

`include "common/register.v"

module jkiss (
    input wire        clk,         // clock
    input wire        rst_n,       // active-low synchronous reset

    input wire [31:0] seed,        // seed to reset state register S1
    input wire        re_seed,     // active high to reset S1 with seed (S2 & S3 unchanged)

    output wire [31:0] rnd         // random number output
);

    // JKISS constants
    localparam state_bitwidth = 4 * 4 * 8;

    wire [31:0] S1_cur, S2_cur, S3_cur, S4_cur;
    wire [31:0] S1_wd, S2_wd, S3_wd, S4_wd;

    // ---------- Full State Register --------------------
    Register
    #(
        .NUM_BITS (state_bitwidth), 
        .RST_VAL  ({(state_bitwidth){1'b0}})
    ) state_regs (
        .clk        (clk),
        .rst_n      (rst_n),
        .write_data ({S4_wd, S3_wd, S2_wd, S1_wd}),
        .read_data  ({S4_cur, S3_cur, S2_cur, S1_cur})
    );
    // ---------------------------------------------------

    // JKISS next random number algorithm
    wire [31:0] S1_next, S2_next, S3_next, S4_next;
    wire [31:0] S2_tmp1, S2_tmp2;
    wire [63:0] tmp;
    assign S1_next = 32'd314527869 * S1_cur + 32'd1234567;
    assign S2_tmp1 = (S2_cur << 'd5) ^ S2_cur;
    assign S2_tmp2 = (S2_tmp1 >> 'd7) ^ S2_tmp1;
    assign S2_next = (S2_tmp2 << 'd22) ^ S2_tmp2;
    assign tmp = 64'd4294584393 * S3_cur + S4_cur;
    assign S3_next = tmp[31:0];
    assign S4_next = tmp[63:32];

    // Write data mux
    assign S1_wd = re_seed ? seed : S1_next;
    assign S2_wd = re_seed ? 32'd987654321 :S2_next;
    assign S3_wd = re_seed ? 32'd43219876 :S3_next;
    assign S4_wd = re_seed ? 32'd6543217 :S4_next;

    // Outputs
    assign rnd       = S1_cur ^ S2_cur ^ S3_cur;

endmodule

`endif // __JKISS_88_V__