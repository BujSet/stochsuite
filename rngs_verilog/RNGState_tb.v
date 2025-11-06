module RNGState_tb;
    // 32 bit state
    RNGState #(.NUM_BYTES(32)) st();

    initial begin
        // write 0-2 bytes from a 32 bit int to offset=4
        st.set_state_bytes_from_int(32'h12345678, 3, 4);

        // read that
        $display("int @4 = 0x%08x", st.get_state_bytes_as_int(4));  // 0x00345678

        // write single byte
        st.set_state_byte(8'hAA, 0);
        $display("byte @0 = 0x%02x", st.get_state_byte(0)); // 0xaa

        // check size
        $display("size = %0d", st.size());

        #10 $stop;
    end
endmodule
