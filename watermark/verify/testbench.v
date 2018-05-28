module test;

parameter length_i = 3, 
          length_o = 5, 
          max_ini = 1024;

reg clk, reset;
reg [length_i-1:0] in;
wire [length_o-1:0] out;
reg [length_o-1:0] exp_out;
reg [31:0]  env[1:0];
reg [max_ini-1:0] init_pattern[1:0], init_pattern_p;
reg [127:0] md5[1:0], md5_p;
reg compare;

integer i,j, k, init_clock, sf, sum_error;

testc U0(clk, reset, in, out); 

initial clk = 0;
always begin
    #10 clk = ~clk;
end

 initial
    begin
      $dumpfile("p1.vcd"); 
      $dumpvars(0, test); 
     /* $fsdbDumpfile("p1.fsdb");
      $fsdbDumpvars(0, test); */
    end  


  initial
    begin
      $readmemh("md5.env", env); 
       init_clock = env[0] ;
       $readmemb("md5.ini", init_pattern);
       $readmemh("md5.dat", md5);
       md5_p = md5[0];
   
    reset = 0 ;
    @(negedge clk) reset = 1;
    @(negedge clk) reset = 0;
        init_pattern_p = init_pattern[0];
        init_pattern_p = init_pattern_p << (max_ini -1 -init_clock*length_i);
         for (j  = init_clock; j >0; j = j-1) begin
           in =  init_pattern_p[max_ini-1:(max_ini-1-length_i)];
            init_pattern_p = init_pattern_p << length_i;
           @(negedge clk);
         end
        i = 128/(length_i+length_o);
        for (j = 0 ; j < i ; j = j+1) begin
          in = md5_p[127:(128-length_i)];
          exp_out = md5_p[(127-length_i):(128-length_i-length_o)];  
          md5_p = md5_p << (length_i + length_o);
          compare = (exp_out == out) ;
                if (exp_out === 'bx)
                   compare = 1'b0;
                if ( compare === 1'b0) begin
                  $write ("Time: %0t Error in WaterMark  ==> in: %h  expect result: %h  out =%h\n", 
                     $time, in, exp_out, out );
                end 
             @(negedge clk);
        end 

    #10  $finish;
    end
endmodule
