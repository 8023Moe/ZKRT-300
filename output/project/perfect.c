#include "../8023.h"
#include "HANS.h"
void main(){
    fun_start(//在此输入小车目前的状态
        0,//X坐标
        0,//Y坐标
        dir_up,//车头方向
        han_s,//手抓状态
        sjp_1,//升降位置
        tra_h,//平移位置
        dir_right);//回转方向
    fun_setxy(//x轴坐标,y轴坐标,工位号,到达此坐标时需要的朝向,工位在小车的哪一侧
        1,6,2,dir_up,dir_right,
        1,8,5,dir_up,dir_left,
        //2,7,3,dir_up,dir_right,
        4,9,3,dir_up,dir_left,
        2,9,1,dir_up,dir_left,
        4,dir_right);
    
    /*请从下一行开始写你的程序*/
	D(2000)
    fun_go(3);
/*
	  str_tfl.step[0]=tfl_line;
	  str_tfl.step[1]=2;
	  str_tfl.step[2]=tfl_cache;
      str_tfl.step[3]=75;

	  str_tfl.step[4]=tfl_turn;
	  str_tfl.step[5]=tur_l90;
2.84/5.3

	  str_tfl.step[6]=tfl_line;
	  str_tfl.step[7]=3;
      str_tfl.step[8]=tfl_cache;
      str_tfl.step[9]=75;
	  str_tfl.step[10]=tfl_turn;
	  str_tfl.step[11]=tur_l90;

	  str_tfl.step[12]=tfl_line;
	  str_tfl.step[13]=2;
      str_tfl.step[14]=tfl_cache;
      str_tfl.step[15]=75;

	  str_tfl.step[16]=tfl_turn;
	  str_tfl.step[17]=tur_r90;

	  str_tfl.step[18]=tfl_line;
	  str_tfl.step[19]=3;
      str_tfl.step[20]=tfl_cache;
      str_tfl.step[21]=75;

	  str_tfl.step[22]=tfl_turn;
	  str_tfl.step[23]=tur_r90;

	  str_tfl.step[24]=tfl_line;
	  str_tfl.step[25]=2;
      str_tfl.step[26]=tfl_cache;
      str_tfl.step[27]=75;

	  str_tfl.step[28]=tfl_turn;
	  str_tfl.step[29]=tur_r90;

	  str_tfl.step[30]=tfl_line;
	  str_tfl.step[31]=3;
      str_tfl.step[32]=tfl_cache;
      str_tfl.step[33]=75;

	  str_tfl.step[34]=tfl_turn;
	  str_tfl.step[35]=tur_r90;

	  str_tfl.step[36]=tfl_line;
	  str_tfl.step[37]=2;
      str_tfl.step[38]=tfl_cache;
      str_tfl.step[39]=75;

	  str_tfl.step[40]=tfl_turn;
	  str_tfl.step[41]=tur_l90;

	  str_tfl.step[42]=tfl_line;
	  str_tfl.step[43]=3;
      str_tfl.step[44]=tfl_cache;
      str_tfl.step[45]=75;

	  str_tfl.step[46]=tfl_turn;
	  str_tfl.step[47]=tur_l90;
	  
	  str_tfl.step[48]=def_end;
	 
	  str_tfl.online=tf_false;
      str_tfl.run=str_tfl.step;
	  str_tfl.gospeed=40;
	  str_tfl.turnspeed=40;
      str_tfl.cachespeed=40;
	  str_tfl.doing=tf_ture;
*/
    /*你的程序到此为止,请不要再继续改动*/
    fun_stop();
}
void Int0Interrupt() interrupt 0{
    ;
}
void Timer0Interrupt() interrupt 1 using 1{
    fun_timer0();
}
void Timer1Interrupt() interrupt 3 using 1{
    fun_timer1();
}