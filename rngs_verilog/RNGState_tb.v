`timescale 1ns/1ps

// simple testbench for RNGState
module tb_RNGState;

    localparam integer NUM_BYTES  = 32;
    localparam integer TOTAL_BITS = 8*NUM_BYTES;

    reg                    clk;
    reg                    rst_n;
    reg  [NUM_BYTES-1:0]   w_en_bytes;
    reg  [TOTAL_BITS-1:0]  w_data_bytes;
    wire [TOTAL_BITS-1:0]  q_bytes;

    // DUT
    RNGState #(
        .NUM_BYTES(NUM_BYTES),
        .TOTAL_BITS(TOTAL_BITS)
    ) dut (
        .clk(clk),
        .rst_n(rst_n),
        .w_en_bytes(w_en_bytes),
        .w_data_bytes(w_data_bytes),
        .q_bytes(q_bytes)
    );

    // clock, 10ns period
    initial begin
        clk = 0;
    end

    always #5 clk = ~clk;

    // small helper: write one byte at index idx
    // note: this is just for tb, not synthesizable logic
    task write_one_byte;
        input integer idx;
        input [7:0]   data;
        integer i;
    begin
        // clear enables and data first
        w_en_bytes   = {NUM_BYTES{1'b0}};
        w_data_bytes = {TOTAL_BITS{1'b0}};

        // set only one byte enable
        if (idx >= 0 && idx < NUM_BYTES) begin
            w_en_bytes[idx] = 1'b1;
            // put data on that 8-bit slice
            w_data_bytes[(8*idx)+7 -: 8] = data;
        end else begin
            $display("** tb warning: idx %0d out of range", idx);
        end

        // wait one clock edge to actually write
        @(posedge clk);
        #1; // little delay for q_bytes to settle

        // drop enables
        w_en_bytes = {NUM_BYTES{1'b0}};
    end
    endtask

    // monitor a few interesting bytes
    // check byte0, byte1, byte2
    initial begin
        $display("time  rst_n  byte0  byte1  byte2");
        $monitor("%4t   %b    %02x    %02x    %02x",
                 $time, rst_n,
                 q_bytes[7:0],
                 q_bytes[15:8],
                 q_bytes[23:16]);
    end

    // main stimulus
    integer k;
    initial begin
        // init signals
        rst_n       = 1'b0;
        w_en_bytes  = {NUM_BYTES{1'b0}};
        w_data_bytes = {TOTAL_BITS{1'b0}};

        // hold reset a few cycles
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);

        // release reset
        rst_n = 1'b1;
        @(posedge clk);
        #1;

        // check reset result (suppose to be all 0)
        if (q_bytes !== {TOTAL_BITS{1'b0}})
            $display("** tb ERROR: state not zero after reset");
        else
            $display("tb info: state is all zero after reset, good");

        // write byte0 = 8'hAA
        $display("tb info: write byte 0 = 0xAA");
        write_one_byte(0, 8'hAA);

        // write byte1 = 8'h55
        $display("tb info: write byte 1 = 0x55");
        write_one_byte(1, 8'h55);

        // write byte2 = 8'hFF
        $display("tb info: write byte 2 = 0xFF");
        write_one_byte(2, 8'hFF);

        // write byte0 again，8'h0F
        $display("tb info: overwrite byte 0 = 0x0F");
        write_one_byte(0, 8'h0F);


        repeat (5) @(posedge clk);

        // self check
        if (q_bytes[7:0]   !== 8'h0F) $display("** tb ERROR: byte0 mismatch, got %02x", q_bytes[7:0]);
        if (q_bytes[15:8]  !== 8'h55) $display("** tb ERROR: byte1 mismatch, got %02x", q_bytes[15:8]);
        if (q_bytes[23:16] !== 8'hFF) $display("** tb ERROR: byte2 mismatch, got %02x", q_bytes[23:16]);

        // reset
        $display("tb info: assert reset again");
        rst_n = 1'b0;
        @(posedge clk);
        @(posedge clk);
        rst_n = 1'b1;
        @(posedge clk);
        #1;

        if (q_bytes !== {TOTAL_BITS{1'b0}})
            $display("** tb ERROR: state not zero after second reset");
        else
            $display("tb info: second reset ok, state cleared");


        $display("tb done.");
        $stop;
    end

endmodule
