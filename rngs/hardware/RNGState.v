// RNGState.v

module RNGState #(
    parameter integer NUM_BYTES  = 32,
    // total bits for state output and input
    parameter integer TOTAL_BITS = 8*NUM_BYTES
)(
    input  wire                   clk,
    input  wire                   rst_n,     // sync reset, active-low

    // per-byte write enable, one bit per byte
    // w_en_bytes[i] == 1 => update byte i this cycle
    input  wire [NUM_BYTES-1:0]   w_en_bytes,

    // write data bus, flattened bytes
    // w_data_bytes[8*i +: 8] goes to byte i if w_en_bytes[i] is 1
    input  wire [TOTAL_BITS-1:0]  w_data_bytes,

    // read out the whole state (registered by FFs)
    // q_bytes[8*i +: 8] == content of byte i
    output wire [TOTAL_BITS-1:0]  q_bytes
);

    // one FF per byte (yeah real flip-flops, not RAM)
    reg [7:0] byte_ff [0:NUM_BYTES-1];

    genvar k;

    generate
        for (k = 0; k < NUM_BYTES; k = k + 1) begin : gen_ffs
            always @(posedge clk) begin
                if (!rst_n) begin
                    byte_ff[k] <= 8'h00;  // zero on reset (no initial)
                end 
                else begin
                    if (w_en_bytes[k]) begin
                        // pick the 8-bit slice for this byte
                        byte_ff[k] <= w_data_bytes[(8*k)+7 : (8*k)];
                    end
                end
            end
        end
    endgenerate
    // when rst_n=0 clear all bytes, otherwise masked write to reg
    // always @(posedge clk) begin
    //     if (!rst_n) begin
    //         for (k = 0; k < NUM_BYTES; k = k + 1)
    //             byte_ff[k] <= 8'h00;  // zero on reset (no initial)
    //     end 
    //     else begin
    //         for (k = 0; k < NUM_BYTES; k = k + 1) begin
    //             if (w_en_bytes[k]) begin
    //                 // pick the 8-bit slice for this byte
    //                 byte_ff[k] <= w_data_bytes[(8*k)+7 : (8*k)];
    //             end
    //             // else hold its old value
    //         end
    //     end
    // end

    // pack out: output q_bytes as all the bytes concatenated
    // q_bytes[8*i +: 8] == byte i
    genvar gi;
    generate
        for (gi = 0; gi < NUM_BYTES; gi = gi + 1) begin : G_PACK
            assign q_bytes[(8*gi)+7 : (8*gi)] = byte_ff[gi];
        end
    endgenerate

endmodule
