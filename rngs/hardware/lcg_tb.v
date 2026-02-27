`ifndef __LCG_TESTBENCH_V__
`define __LCG_TESTBENCH_V__

`timescale 1ns/1ps

module lcg_tb;

    // only for gatesim
    `ifdef GATESIM
        glbl glbl ();
    `endif

    reg         clk;
    reg         rst_n;
    reg  [31:0] seed;
    reg         re_seed;

    wire [31:0] rnd;

    // dut
    lcg dut (
        .clk     (clk),
        .rst_n   (rst_n),
        .seed    (seed),
        .re_seed (re_seed),
        .rnd     (rnd)
    );

    // clock
    initial begin
        clk = 1'b0;
    end
    always #5 clk = ~clk;

    task test_value;
        input integer expected;
        input [31:0] produced;
        begin
            if (produced != expected) begin
                $fatal(1, "Expected rnd %d after reseed, got %d", expected, produced);
            end
            @(posedge clk);
        end
    endtask

    task reseed_dut;
        input [31:0] new_seed;
        begin
            seed    <= new_seed;
            re_seed <= 1'b1;
            $display("%t Reseeding state register with 0x%08h", $time, new_seed);
            @(posedge clk);
            re_seed <= 1'b0;
            @(posedge clk);
            @(posedge clk);
        end
    endtask

    initial begin
        rst_n   = 1'b1;
        re_seed = 1'b0;
        seed    = 32'h1234_5678;

        repeat (5) @(posedge clk);
        rst_n <= 1'b0;
        @(posedge clk);
        rst_n <= 1'b1;
        $display("%t ns : rnd = 0x%08h | %0d", $time, rnd, rnd);
        reseed_dut(seed);

        // After reset, xorshift32_opt state resets to 0 by design (RST_VAL = 0)
        // So we always reseed before checking.
        repeat (5) begin : init_cycles
            @(posedge clk);
            $display("%t ns : rnd = 0x%08h | %0d", $time, rnd, rnd);
        end


        reseed_dut(32'hDEAD_BEEF);
        test_value(32'd3805667050, rnd);
        test_value(32'd1620195817, rnd);
        test_value(32'd4228188956, rnd);
        test_value(32'd482945011, rnd);
        test_value(32'd1814178590, rnd);
        test_value(32'd2126373773, rnd);
        test_value(32'd104675184, rnd);
        test_value(32'd1381559095,  rnd);
        test_value(32'd1617951890, rnd);
        test_value(32'd3861217649, rnd);

        reseed_dut(32'hCAFEBABE);
        test_value(32'd944244397, rnd);
        test_value(32'd3234068496, rnd);
        test_value(32'd1219054423,  rnd);
        test_value(32'd332305970, rnd);
        test_value(32'd4032013969, rnd);
        test_value(32'd1494586788, rnd);
        test_value(32'd77135579, rnd);
        test_value(32'd1919093478, rnd);
        test_value(32'd2882944693, rnd);
        test_value(32'd3129425528,  rnd);

        $display("%t: TEST PASSED", $time);
        $finish;
    end

endmodule
 
`endif // __LCG_TESTBENCH_V__
