`ifndef __XORSHIFT_32_TESTBENCH_V__
`define __XORSHIFT_32_TESTBENCH_V__

`timescale 1ns/1ps

module XorShift32_TB;

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
    xorshift32_opt dut (
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

        // After reset, xorshift32_opt state resets to 0 by design (RST_VAL = 0)
        // So we always reseed before checking.
        repeat (5) begin : init_cycles
            @(posedge clk);
            $display("%t ns : rnd = 0x%08h | %0d", $time, rnd, rnd);
        end

        // Spot check a few iterations after reseed
        // Note: these expected values assume the RTL convention rnd = s_cur (pre-update),
        // and that reseed drives state to seed, then updates start on subsequent cycles.
        // If you choose rnd = s_next in RTL, these numbers will shift by 1 cycle.
        reseed_dut(32'hDEAD_BEEF);
        test_value(32'd3735928559, rnd);
        test_value(32'd1277311135, rnd);
        test_value(32'd2318515675, rnd);
        test_value(32'd1692061533, rnd);
        test_value(32'd2914831697, rnd);
        test_value(32'd2401654032, rnd);
        test_value(32'd3195421333, rnd);
        test_value(32'd239402786,  rnd);
        test_value(32'd1013918553, rnd);
        test_value(32'd2511123945, rnd);

        reseed_dut(32'hCAFEBABE);
        test_value(32'd3405691582, rnd);
        test_value(32'd3792176755, rnd);
        test_value(32'd976299370,  rnd);
        test_value(32'd1856029631, rnd);
        test_value(32'd2875390495, rnd);
        test_value(32'd4268168009, rnd);
        test_value(32'd3000433025, rnd);
        test_value(32'd3235076403, rnd);
        test_value(32'd881332418,  rnd);
        test_value(32'd1888437366, rnd);

        $display("%t: TEST PASSED", $time);
        $finish;
    end

endmodule

`endif // __XORSHIFT_32_TESTBENCH_V__
