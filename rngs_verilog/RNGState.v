// RNGState.v
// just a simple version of RNGState in verilog
// stores bytes and allow user to read and write

module RNGState #(
    parameter integer NUM_BYTES = 32
) (
    input wire clk, rst

);

    reg [7:0] state_mem [0:NUM_BYTES-1];
    integer i;

    // always ff smth
    
    initial begin
        // init everything to 0 (not really necessary)
        for (i = 0; i < NUM_BYTES; i = i + 1)
            state_mem[i] = 8'h00;
    end

    // uint8_t *getState()
    // return bytes instead of pointers
    function [7:0] get_state_byte;
        input integer state_offset;
        begin
            if (state_offset < NUM_BYTES)
                get_state_byte = state_mem[state_offset];
            else begin
                get_state_byte = 8'h00;   // should assert?
                $display("get_state_byte: Out of bound (offset=%0d)", state_offset);
            end
        end
    endfunction

    // void set_state_byte(uint8_t new_state, size_t state_offset)
    // just set one byte
    task set_state_byte;
        input [7:0] new_state;
        input integer state_offset;
        begin
            if (state_offset < NUM_BYTES)
                state_mem[state_offset] = new_state;
            else
                $display("set_state_byte: offset %0d out of range (max %0d)", state_offset, NUM_BYTES-1);
        end
    endtask

    // set bytes from a 32-bit int
    // similar to set_state_bytes_from_int() in cpp
    // write little endian order (low byte first)
    task set_state_bytes_from_int;
        input [31:0] new_state;
        input integer valid_bytes;     // <= 4
        input integer state_offset;
        integer k;
        begin
            if (valid_bytes == 0)
                ; // nothing
            else if (valid_bytes > 4)
                $display("too many bytes (%0d), should be <=4", valid_bytes);
            else if (state_offset + valid_bytes > NUM_BYTES)
                $display("write out of range! offset=%0d, bytes=%0d", state_offset, valid_bytes);
            else begin
                for (k = 0; k < valid_bytes; k = k + 1)
                    state_mem[state_offset + k] = (new_state >> (8*k)) & 8'hFF;
            end
        end
    endtask

    // set bytes from a 64-bit long
    // same idea but for 8 bytes
    task set_state_bytes_from_long;
        input [63:0] new_state;
        input integer valid_bytes;     // <= 8
        input integer state_offset;
        integer k;
        begin
            if (valid_bytes == 0)
                ; // do nothing
            else if (valid_bytes > 8)
                $display("too many bytes (%0d), should be <=8", valid_bytes);
            else if (state_offset + valid_bytes > NUM_BYTES)
                $display("write out of range! offset=%0d, bytes=%0d", state_offset, valid_bytes);
            else begin
                for (k = 0; k < valid_bytes; k = k + 1)
                    state_mem[state_offset + k] = (new_state >> (8*k)) & 8'hFF;
            end
        end
    endtask

    // read 4 bytes as 32-bit int
    // little endian
    function [31:0] get_state_bytes_as_int;
        input integer state_offset;
        reg [31:0] result;
        integer k;
        begin
            result = 0;
            if (state_offset + 4 > NUM_BYTES)
                $display("get_state_bytes_as_int: read out of range (offset=%0d)", state_offset);
            else begin
                for (k = 0; k < 4; k = k + 1)
                    result = result | ( {24'h0, state_mem[state_offset + k]} << (8*k) );
            end
            get_state_bytes_as_int = result;
        end
    endfunction

    // read 8 bytes as 64-bit long
    // also little endian
    function [63:0] get_state_bytes_as_long;
        input integer state_offset;
        reg [63:0] result;
        integer k;
        begin
            result = 0;
            if (state_offset + 8 > NUM_BYTES)
                $display("get_state_bytes_as_long: read out of range (offset=%0d)", state_offset);
            else begin
                for (k = 0; k < 8; k = k + 1)
                    result = result | ( {56'h0, state_mem[state_offset + k]} << (8*k) );
            end
            get_state_bytes_as_long = result;
        end
    endfunction

    // size()
    // just return numBytes (constant actually)
    function integer size;
        begin
            size = NUM_BYTES;
        end
    endfunction

endmodule
