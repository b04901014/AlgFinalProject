Watermarking-based Intellectual Property Core Protection Scheme
一、目錄說明
   kiss2 :  4組測試資料，檔案格式為 linux/unix 格式，不是DOS格式， t1c.kiss2 是完整的FSM，無法嵌入訊息。
   verify:  驗證是否嵌入訊息的verilog 環境
  
二、執行步驟
   1.執行您的程式:
     time your_program_name –i ifsm.kiss –o ofsm.kiss –m md5_1.dat –m md5_2.dat –m md5_3.dat
   2. 將ofsm.kiss 轉換成verilog
      執行:  k2v.pl ofsm.kiss ofsm.v
     注意: 
        a.  所有的狀態名字都是以"S"開頭，後面加一串數字。
        b.  ofsm.kiss 狀態表的相同的 current-state 必須相鄰，否則verilog 檔格式會有問題。
               例如底下的排列方式無法產生正確的verilog檔。    
                    100 S0 S1 11
                    101 S2 S1 11
                    110 S0 S1 11
    3. 將產生的ofsm.v, md5_1.env, md5_2.env, md5_3.env, md5_1.ini, md5_2.ini, md5_3.ini以及md5_1.dat, md5_2.dat, md5_3.dat複製到verify目錄
    4.  每組驗證前，都先進入初始狀態，程序如下:
     先進入初始狀態， 執行latency input(如果有)，再來驗證第一組watermark，
     先進入初始狀態， 執行latency input(如果有)，再來驗證第二組watermark，
     先進入初始狀態， 執行latency input(如果有)，再來驗證第三組watermark。
          將 md5_1.env 改名成 md5.env
          將 md5_1.ini 改名成 md5.ini
          將 md5_1.dat 改名成 md5.dat
       執行: verilog ofsm.v testbench.v
        
         接下來，驗證第二組: 
          將 md5_2.env 改名成 md5.env
          將 md5_2.ini 改名成 md5.ini
          將 md5_2.dat 改名成 md5.dat
       執行: verilog ofsm.v testbench.v
   
          第三組以此類推。
