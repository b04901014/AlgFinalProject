Watermarking-based Intellectual Property Core Protection Scheme
�@�B�ؿ�����
   kiss2 :  4�մ��ո�ơA�ɮ׮榡�� linux/unix �榡�A���ODOS�榡�A t1c.kiss2 �O���㪺FSM�A�L�k�O�J�T���C
   verify:  ���ҬO�_�O�J�T����verilog ����
  
�G�B����B�J
   1.����z���{��:
     time your_program_name �Vi ifsm.kiss �Vo ofsm.kiss �Vm md5_1.dat �Vm md5_2.dat �Vm md5_3.dat
   2. �Nofsm.kiss �ഫ��verilog
      ����:  k2v.pl ofsm.kiss ofsm.v
     �`�N: 
        a.  �Ҧ������A�W�r���O�H"S"�}�Y�A�᭱�[�@��Ʀr�C
        b.  ofsm.kiss ���A���ۦP�� current-state �����۾F�A�_�hverilog �ɮ榡�|�����D�C
               �Ҧp���U���ƦC�覡�L�k���ͥ��T��verilog�ɡC    
                    100 S0 S1 11
                    101 S2 S1 11
                    110 S0 S1 11
    3. �N���ͪ�ofsm.v, md5_1.env, md5_2.env, md5_3.env, md5_1.ini, md5_2.ini, md5_3.ini�H��md5_1.dat, md5_2.dat, md5_3.dat�ƻs��verify�ؿ�
    4.  �C�����ҫe�A�����i�J��l���A�A�{�Ǧp�U:
     ���i�J��l���A�A ����latency input(�p�G��)�A�A�����ҲĤ@��watermark�A
     ���i�J��l���A�A ����latency input(�p�G��)�A�A�����ҲĤG��watermark�A
     ���i�J��l���A�A ����latency input(�p�G��)�A�A�����ҲĤT��watermark�C
          �N md5_1.env ��W�� md5.env
          �N md5_1.ini ��W�� md5.ini
          �N md5_1.dat ��W�� md5.dat
       ����: verilog ofsm.v testbench.v
        
         ���U�ӡA���ҲĤG��: 
          �N md5_2.env ��W�� md5.env
          �N md5_2.ini ��W�� md5.ini
          �N md5_2.dat ��W�� md5.dat
       ����: verilog ofsm.v testbench.v
   
          �ĤT�եH�������C
