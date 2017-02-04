#include "easy.h"
#include "./lib/8023.h"
void main(){
    fun_start(//在此输入小车目前的状态
        4,//X坐标
        0,//Y坐标
        dir_up,//车头方向
        han_s,//手抓状态
        sjp_1,//升降位置
        tra_h,//平移位置
        dir_right);//回转方向
    fun_setxy(//x轴坐标,y轴坐标,工位号,到达此坐标时需要的朝向,工位在小车的哪一侧
        0, 7,  4, dir_down, dir_left,
        1, 10, 2, dir_down, dir_right,
        2, 6,  3, dir_up,   dir_left,
        3, 1,  1, dir_up,   dir_left,
               5, dir_left, dir_right);

    /*请从下一行开始写你的程序*/

//START
    GG//到拿件处
    ZJ(35862714,72143586)//抓件
    GS//回到起始区

//3: 37
    OG3//不同步走到3号工件台

    S12//上升半个格, 回转到前方
    PQ//前平移
    J3//升降至位置3
    SS//手抓松

    J1//升降至位置1
    HZ//回转至左侧
    J2//升降至位置2
    SJ//手抓紧
    S1//向上到位置1的同时回转至前方
    J2//升降至位置2
    SS//手抓松
    J1//升降至位置1

//4: 25
    TG4//同步走到4号工作台
    HZ//巡线的同时, 回转至左侧
    J3//巡线的同时, 升降到位置3
    SJ//巡线的同时, 手抓紧
    S2//巡线的同时, 升降到位置2并回转至前方
    WT//等待同步运动和巡线任务全部做完

    J3//升降到位置3
    SS//手抓松
    
    Y2//升降到位置2的同时回转至右方
    PH//平移到后方
    J3//升降到位置3
    SJ//手抓紧
    J1//升降至位置1
    SQ//平移至前方的同时回转至前方
    J2//升降至位置2
    SS//手抓松
    J1//升降至位置1

//1: 86
    TG1//同步走到1号工作台
    YH//巡线的同时, 回转至右侧, 平移至后侧
    J4//巡线的同时, 升降到位置4
    SJ//巡线的同时, 手抓紧
    S2//巡线的同时, 升降到位置2并回转至前方
    PQ//巡线的同时, 前平移
    WT//等待同步运动和巡线任务全部做完
    
    J3//升降到位置3
    SS//手抓松
    
    Y2//升降到位置2的同时回转至右方
    PH//平移到后方
    J5//升降到位置5
    SJ//手抓紧
    J1//升降至位置1
    SQ//平移至前方的同时回转至前方
    J2//升降至位置2
    SS//手抓松
    J1//升降至位置1

//5: 1
    TG5//同步走到5号工作台
    HZ//巡线的同时, 回转至左侧
    J4//巡线的同时, 升降到位置4
    SJ//巡线的同时, 手抓紧
    S2//巡线的同时, 升降到位置2并回转至前方
    PH//巡线的同时, 后平移
    WT//等待同步运动和巡线任务全部做完
    
    J3//升降到位置3
    SS//手抓松
    J2//升降到位置2
    
//2: 4
    TG2//同步走到2号工作台
    ZQ//巡线的同时, 平移至前方并回转至左侧
    J5//巡线的同时, 升降到位置5
    SJ//巡线的同时, 手抓紧
    S2//巡线的同时, 升降到位置2并回转至前方
    HX//巡线的同时, 回转之下方
    WT//等待同步运动和巡线任务全部做完

    SS//手抓松(因为之后不需要操作此台, 直接把工件扔下即可)
    
//3: 7 -> 5
    TG2//同步走到3号工作台
    SH1//巡线的同时, 升降到位置1并回转至前方并平移至后方
    WT//等待同步运动和巡线任务全部做完

    J2//巡线的同时, 升降到位置2
    SJ//巡线的同时, 手抓紧
    J1//巡线的同时, 升降到位置1

    TG5//同步走到5号工作台
    PH//巡线的同时, 平移到后方
    WT//等待同步运动和巡线任务全部做完

    J2//巡线的同时, 升降到位置2
    SS//巡线的同时, 手抓松
    J1//巡线的同时, 升降到位置1

//END
    GE

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