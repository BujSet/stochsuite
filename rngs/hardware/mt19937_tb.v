`ifndef __MERSENNE_TWISTER_19937_TESTBENCH_V__
`define __MERSENNE_TWISTER_19937_TESTBENCH_V__

`timescale 1ns/1ps

module mt19937_tb;

`ifdef GATESIM
    glbl glbl ();
`endif
    reg         clk;
    reg         rst_n;
    reg  [31:0] seed;
    reg         re_seed;

    wire [31:0] rnd;

    // dut
    mt19937 dut (
        .clk      (clk),
        .rst_n    (rst_n),
        .seed     (seed),
        .re_seed  (re_seed),
        .rnd      (rnd)
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
            @(posedge clk); // Wait for a positive clock edge
        end
    endtask

    task reseed_dut;
        input [31:0] new_seed;
        begin
            seed    <= new_seed;
            re_seed <= 1'b1;
            $display("%t Reseeding S1 register with 0x%08h", $time, new_seed);
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
        $display("%t ns : rnd = 0x%08h | %d", $time, rnd, rnd);

        repeat (10) begin : init_cycles
            @(posedge clk);
            $display("%t ns : rnd = 0x%08h | %d", $time, rnd, rnd);
        end


        // Spot check a few iterations after reseed
        reseed_dut(32'hDEAD_BEEF);
        test_value(32'd3759560541, rnd);
        test_value(32'd4273012039, rnd);
        test_value(32'd1162407812, rnd);
        test_value(32'd1121386505, rnd);
        test_value(32'd3932735964, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);
        test_value(32'd0, rnd);

        // reseed_dut(32'hCAFEBABE);
        // test_value(32'd3951813429, rnd);
        // test_value(32'd3191570171, rnd);
        // test_value(32'd4247730860, rnd);
        // test_value(32'd4225878095, rnd);
        // test_value(32'd2349118836, rnd);
        // test_value(32'd3582280224, rnd);
        // test_value(32'd3567654117, rnd);
        // test_value(32'd1909425086, rnd);
        // test_value(32'd3510461680, rnd);
        // test_value(32'd4127355812, rnd);

        $display("%t: TEST PASSED", $time);
        $finish;
    end

endmodule
`endif // __MERSENNE_TWISTER_19937_TESTBENCH_V__
