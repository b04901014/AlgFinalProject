#! /usr/bin/perl
# writen by Wei-Chang Tsai, CIC, NARL
# 2018/04/20

if ( !$ARGV[0]) { die " Please enter kiss file name  and output file name\n"}

my $filename=$ARGV[0];
my $ofilename =$ARGV[1];
my @power2= (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096);

$input_l = 0;
$output_l = 0 ;
$number_p = 0;
$number_state = 0 ;

$step = 0;
my $i=0;
my $j=0;
my $state_step =0 ;

open(my $fh, $filename)  or die "Could not open file: '$filename' $!";
open(my $ofh, ">$ofilename")  or die "Could not open file: '$ofilename' $!";

while (my $row = <$fh>) {

  if ($step < 6 ) {
  @lines =    split(' ', $row);
  foreach my $line (@lines) {
    if ($line eq ".i") {$step = 1;}
    elsif ($line eq ".o") {$step = 2;}
    elsif ($line eq ".p") {$step = 3;}
    elsif ($line eq ".s") {$step = 4;}
    elsif ($line eq ".r") {$step = 5;}
    else {
      if ($step == 1 ) {$input_l = $line -1;}
      elsif($step == 2) { $output_l = $line -1;}
      elsif($step ==3) { $number_p = $line; }
      elsif($step ==4) { $number_state = $line; }
      elsif($step ==5) {
        $init_state_r = $line; 
        $step = 6;
        print $ofh "module testc(clk, rst, in, out);\n";
        print $ofh "input clk, rst;\n";
        print $ofh "input [$input_l:0] in;\n";
        print $ofh "output [$output_l:0] out;\n";
        print $ofh "reg [$output_l:0] out;\n";
        $i = &state_len($number_state);
        $j = $i + 1 ;
        print $ofh "reg [$i:0] pstate, nstate;\n";
        print $ofh "parameter ";
        for ($i= 0 ; $i < $number_state ; $i ++)  {
          print $ofh "S$i=$j\'d$i" ;
          if ($i == $number_state -1 ) {print $ofh ";\n\n";}
          elsif ($i % 5 == 4 ) {print $ofh ",\n";}
          else {print $ofh ", ";}
        } 
         
        print $ofh "always @(posedge clk or posedge rst) begin\n";
        print $ofh " if (rst) pstate <= $init_state_r; \n";
        print $ofh " else pstate <= nstate;\n";
        print $ofh "end\n\n";
#        print $ofh "always @(*) begin\n";
        print $ofh "always @(in or pstate) begin\n";
        print $ofh "case (pstate)\n";
        print $ofh "S0: casex(in)\n";
         $input_l = $input_l +1 ;
         $output_l = $output_l +1 ;
      }
   }
  } 
  } elsif ($step ==6) {
  @lines =  split(' ', $row);
   if ($lines[0] ne ".e")  {
       my $current_state = substr($lines[1], 1) ;
     if ($current_state  != $state_step) {
         $state_step = $current_state ;
         print $ofh "  endcase\nS$current_state: casex(in)\n";
     } 
#       print ("$lines[0] $lines[1] $lines[2] $lines[3] $state_step $current_state\n");
       my $input_value  = $lines[0];
       $input_value =~s/-/x/g;
       my $next_state =  substr($lines[2], 1);
       print $ofh "  $input_l\'b$input_value: begin nstate=S$next_state; out =$output_l\'b$lines[3]; end\n"
     } else {
        $step = 7; 
    }
  }
}  

print $ofh "  endcase\nendcase\nend\nendmodule\n";


close($fh);
close($ofh);

sub state_len{
my $n = shift;
my $i, $j;
   for ($i = 0 ; $i <= 12; $i++) {
#     print("TTT: $n  @power2[$i]  $i \n");
     if ($n <= @power2[$i])  {
       $j = $i;  $i = 12;
     }
   }
 return ($j);
}
