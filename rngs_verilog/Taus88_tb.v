`timescale 1ns/1ps

module tb_taus88_core;

    reg         clk;
    reg         rst_n;
    reg  [31:0] seed;
    reg         re_seed;

    wire [31:0] rnd;
    wire        rnd_valid;

    // dut
    taus88_core dut (
        .clk      (clk),
        .rst_n    (rst_n),
        .seed     (seed),
        .re_seed  (re_seed),
        .rnd      (rnd),
        .rnd_valid(rnd_valid)
    );

    // clock
    initial begin
        clk = 1'b0;
    end
    always #5 clk = ~clk;   

    integer i;

    initial begin
        rst_n   = 1'b0;
        re_seed = 1'b0;
        seed    = 32'h1234_5678;

        // $dumpfile("taus88_core_tb.vcd");
        // $dumpvars(0, tb_taus88_core);

        #20;           
        rst_n = 1'b1; 

        // defualt S1, S2, S3
        for (i = 0; i < 10; i = i + 1) begin
            @(posedge clk);
            if (rnd_valid)
                $display($time, " ns : rnd = 0x%08h", rnd);
        end

        // reseed    
        seed    = 32'hDEAD_BEEF;
        re_seed = 1'b1;
        @(posedge clk);      
        re_seed = 1'b0;

        for (i = 0; i < 10; i = i + 1) begin
            @(posedge clk);
            if (rnd_valid)
                $display($time, " ns : rnd = 0x%08h", rnd);
        end

        // reseed
        seed    = 32'hCAFEBABE;
        re_seed = 1'b1;
        @(posedge clk);
        re_seed = 1'b0;

        for (i = 0; i < 10; i = i + 1) begin
            @(posedge clk);
            if (rnd_valid)
                $display($time, " ns : rnd = 0x%08h", rnd);
        end

        $stop;
    end

endmodule
