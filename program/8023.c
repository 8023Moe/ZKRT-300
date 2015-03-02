#include "./output/8023.h"
struct str_state str_begin,str_now,str_next;//分别为:起始状态/当前状态/目标状态
struct str_parameter str_cod={
    /*ui str_cod.mlinerqd*/500,//默认主函数巡线软起动时间为500毫秒
    /*ui str_cod.mlineqc*/300,//默认主函数巡线前冲时间为500毫秒

    /*ui str_cod.sj1xx*/600,
    /*ui str_cod.sj1xs*/1000,

    /*ui str_cod.py1qkq*/666,
    /*ui str_cod.py1kqz*/684,
    /*ui str_cod.py1zkh*/684,
    /*ui str_cod.py1khh*/666,
    /*ui str_cod.py1qz*/1310,
    /*ui str_cod.py1zh*/1310,
    /*ui str_cod.py1kqkh*/1480,
    /*ui str_cod.py1qkh*/2122,
    /*ui str_cod.py1kqh*/2122,
};
void fun_delay(ui par_value,enum varENU_del par_model){
    ui loc_con=par_value;
    switch(par_model){
        case del_us://微秒级延时
            while(loc_con-->0){
                _nop_();
                _nop_();
            }
            return;
        case del_ms://毫秒级延时
            while(loc_con-->0){
                uc loc_i, loc_j;
                _nop_();
                _nop_();
                loc_i=12;
                loc_j=168;
                do{
                    while(--loc_j);
                }while(--loc_i);
            }
            return;
        case del_s://秒级延时
            while(loc_con-->0){
                uc loc_i, loc_j, loc_k;
                loc_i=46;
                loc_j=153;
                loc_k=245;
                do{
                    do{
                        while(--loc_k);
                    }while(--loc_j);
                }while(--loc_i);
            }
            return;
        default:return;
    }
}//延时
void fun_timer0init(){
    AUXR&=0x7F;
    TMOD&=0xF0;
    TMOD|=0x01;
    TL0=0xB0;
    TH0=0x3C;
    TF0=0;
}//50毫秒定时器0初始化
void fun_timer1init(){
    AUXR&=0xBF;
    TMOD&=0x0F;
    TMOD|=0x10;
    TL1=0xE0;
    TH1=0xB1;
    TF1=0;
}//20毫秒定时器1初始化
void fun_timer0(){
    TL0=0xB0;
    TH0=0x3C;
    _nop_();
}//50毫秒定时器0处理函数
void fun_timer1(){
    TL1=0xE0;
    TH1=0xB1;
    _nop_();
}//20毫秒定时器1处理函数
void fun_wait(){
    while(in_start==1);
    fun_delay(20,del_ms);
    while(in_start==0);
    fun_delay(256,del_ms);
}//等待按键
void fun_select(enum varENU_sel par_model){
    if(par_model==sel_58)
        out_switchselect=0;
    else if(par_model==sel_912)
        out_switchselect=1;
}//传感器片选
void fun_initialization(){
    CLK_DIV=0x00;//不分频

    P0M1=0xff;//P0用于输入
    P0M0=0x00;//P0不能输出
    
    P1M1=0x00;//P1口0-1双向，2中断输入，3-6输入，7输出
    P1M0=0xfc;//P1口2-7输出

    P2M1=0xf0;//P2口4-7输入
    P2M0=0x0f;//P2口0-3输出

    //PS_2=1;//手抓松传感器置1 $?$
    //PS_11=1;//升降位置3传感器置1 $?$

    out_en1=1;//电机1/3使能 $?$
    out_en2=1;//电机2/4使能 $?$
    //out_motorselect=1;//电机片选为1 $?$
    //fun_delay(del_ms,1);//延时1毫秒 $?$
    out_motorselect=0;//电机片选为0 $?$
    //PS_5=PS_1;// $?$

    fun_pwminit();//PWM的初始化设置
    fun_timer0init();//初始化定时器0
    fun_timer1init();//初始化定时器1
    in_start=1;//按键置1

    fun_wait();
}//初始化
void fun_pwminit(){
    CCON=0x00;
    CH=0;
    CL=0;
    CMOD=0x00;
}//PWM初始化
void fun_pwmr(uc par_value){
    CCAP0H=CCAP0L=par_value*2.5;//控制输出的占空比
    CCAPM0=0X42;//8位PWM输出，无中断
    PCA_PWM0=0x00;
}//右路PWM输出
void fun_pwml(uc par_value){
    CCAP1H=CCAP1L=par_value*2.5;//控制输出的占空比
    CCAPM1=0X42;//8位PWM输出，无中断
    PCA_PWM1=0x00;
}//左路PWM输出
void fun_startdj(enum varENU_mot par_model,char par_speed){
    if(par_speed==0)
        return;
    else if(par_speed>100)
        par_speed=100;
    else if(par_speed<-100)
        par_speed=-100;
    switch(par_model){
        case mot_l://左轮电机
            CR=1;
            if(par_speed>0){
                fun_pwml(par_speed);out_pwml=0;
            }
            else if(par_speed<0){
                fun_pwml(cabs(par_speed));out_pwml=1;
            }
            break;
        case mot_r://右轮电机
            CR=1;
            if(par_speed>0){
                fun_pwmr(par_speed);out_pwmr=0;
            }
            else if(par_speed<0){
                fun_pwmr(cabs(par_speed));out_pwmr=1;
            }
            break;
        case mot_rl://左右轮同步
            CR=1;
            if(par_speed>0){
                fun_pwml(par_speed);out_pwml=0;
                fun_pwmr(par_speed);out_pwmr=0;
            }
            else if(par_speed<0){
                fun_pwml(cabs(par_speed));out_pwml=1;
                fun_pwmr(cabs(par_speed));out_pwmr=1;
            }
            break;
        case mot_dj1://正转为抓紧，反转为松开
            out_motorselect=1;
            if(par_speed>0)
                out_dir1=1;
            else if(par_speed<0)
                out_dir1=0;
            out_en1=0;
            break;
        case mot_dj2://正转是向无电机一方转,反转为向有电机一方转
            out_motorselect=1;
            if(par_speed>0)
                out_dir2=1;
            else if(par_speed<0)
                out_dir2=0;
            out_en2=0;
            break;
        case mot_dj3://向上为正转,向下为反转
            out_motorselect=0;
            if(par_speed>0)
                out_dir1=0;
            else if(par_speed<0)
                out_dir1=1;
            out_en1=0;
            break;
        case mot_dj4://顺时针为正转,逆时针为反转
            out_motorselect=0;
            if(par_speed>0)
                out_dir2=0;
            else if(par_speed<0)
                out_dir2=1;
            out_en2=0;
            break;
        default:
            break;
    }
}//启动电机
void fun_stop(enum varENU_mot par_model){
    switch(par_model){
        case mot_l:
            fun_pwml(0);out_pwml=0;break;
        case mot_r:
            fun_pwmr(0);out_pwmr=0;break;
        case mot_rl:
            CR=0;
            fun_pwmr(0);out_pwmr=0;
            fun_pwml(0);out_pwml=0;
            break;
        case mot_dj1:
            out_motorselect=1;out_dir1=0;out_en1=1;break;
        case mot_dj2:
            out_motorselect=1;out_dir2=0;out_en2=1;break;
        case mot_dj3:
            out_motorselect=0;out_dir1=0;out_en1=1;break;
        case mot_dj4:
            out_motorselect=0;out_dir2=0;out_en2=1;break;
        default:
            break;
    }
}//停止电机
void fun_sz1(enum varENU_han par_model){
    if(par_model==han_s){//手抓松
        while(1){
            fun_startdj(mot_dj1,-100);
            while(in_s==1);
            fun_delay(20,del_ms);
            if(in_s==0)
                break;
        }
    }
    else{//手抓紧
        while(1){
            fun_startdj(mot_dj1,100);
            while(in_j==1);
            fun_delay(20,del_ms);
            if(in_j==0)
                break;
        }
    }
    fun_stop(mot_dj1);
    str_begin.szzt=par_model;//存储运行结果
}//手抓单步运动
void fun_sj1(enum varENU_sjp par_model){
    if(par_model==str_begin.sjwz)
       return;
    switch(par_model){
        case sjp_wz1://升降位置1(最上位)
            fun_startdj(mot_dj3,100);
            fun_select(sel_58);
            fun_delay(50,del_ms);
            while(1){//只有向上
                while(in_wz1==1);
                fun_delay(20,del_ms);
                if(in_wz1==0)
                    break;
            }
            break;
        case sjp_wz12:
            if(par_model>str_begin.sjwz){
                fun_startdj(mot_dj3,-100);
                fun_delay(str_cod.sj1xx,del_ms);
            }
            else{//要去的地方在上面，向上走
                fun_sj1(sjp_wz2);
                fun_startdj(mot_dj3,100);
                fun_delay(str_cod.sj1xs,del_ms);
            }
            break;
        case sjp_wz2://升降位置2
            if(par_model>str_begin.sjwz)
                fun_startdj(mot_dj3,-100);
            else
                fun_startdj(mot_dj3,100);
            fun_select(sel_58);
            fun_delay(50,del_ms);
            while(1){//要去的地方比较靠下,向下走                    
                while(in_wz2==1);
                fun_delay(20,del_ms);
                if(in_wz2==0)
                    break;
            }
            break;
        case sjp_wz23:
            if(par_model>str_begin.sjwz){
                fun_sj1(sjp_wz2);
                fun_startdj(mot_dj3,-100);
                fun_delay(str_cod.sj1xx,del_ms);
            }
            else{//要去的地方在上面，向上走
                fun_sj1(sjp_wz3);
                fun_startdj(mot_dj3,100);
                fun_delay(str_cod.sj1xs,del_ms);
            }
            break;
        case sjp_wz3://升降位置3
            if(par_model>str_begin.sjwz)
                fun_startdj(mot_dj3,-100);
            else
                fun_startdj(mot_dj3,100);
            fun_select(sel_58);
            fun_delay(50,del_ms);
            while(1){
                while(in_wz3==1);
                fun_delay(20,del_ms);
                if(in_wz3==0)
                    break;
            }
            break;
        case sjp_wz34:
            if(par_model>str_begin.sjwz){
                fun_sj1(sjp_wz3);
                fun_startdj(mot_dj3,-100);
                fun_delay(str_cod.sj1xx,del_ms);
            }
            else{//要去的地方在上面，向上走
                fun_sj1(sjp_wz4);
                fun_startdj(mot_dj3,100);
                fun_delay(str_cod.sj1xs,del_ms);
            }
            break;
        case sjp_wz4://升降位置4
            if(par_model>str_begin.sjwz)
                fun_startdj(mot_dj3,-100);
            else
                fun_startdj(mot_dj3,100);
            fun_select(sel_58);
            fun_delay(50,del_ms);
            while(1){
                while(in_wz4==1);
                fun_delay(20,del_ms);
                if(in_wz4==0)
                    break;
            }
            break;
        case sjp_wz45:
            if(par_model>str_begin.sjwz){
                fun_sj1(sjp_wz4);
                fun_startdj(mot_dj3,-100);
                fun_delay(str_cod.sj1xx,del_ms);
            }
            else{//要去的地方在上面，向上走
                fun_startdj(mot_dj3,100);
                //fun_delay(str_cod.sj1xs,del_ms);
            }
            break;
        case sjp_wz5://升降位置5
            fun_startdj(mot_dj3,-100);
            fun_select(sel_912);
            fun_delay(50,del_ms);
            while(1){//要去的地方比较靠下,向下走                
                while(in_wz5==1);
                fun_delay(20,del_ms);
                if(in_wz5==0)
                    break;
            }
            break;
        default:
            break;
    }
    fun_stop(mot_dj3);
    str_begin.sjwz=par_model;//存储运行结果
}//升降单步运动
void fun_py1(enum varENU_tra par_model){
    switch(par_model){
        case tra_q://前平移(没有电机的呢个方向)
            while(1){
                fun_startdj(mot_dj2,100);
                while(in_qpy==1);
                fun_delay(20,del_ms);
                if(in_qpy==0)
                    break;
            }
            break;
        // case tra_kq:
        //     switch(str_begin.pywz){
        //         case tra_q:
        //             //fun_startdj(mot_dj2,-100);
        //             //D(666)

        //             break;
        //         case tra_z:
        //             break;
        //         case tra_kh:
        //             break;
        //         case tra_h:
        //             break;
        //     }
        //     break;
        // case tra_z:
        //     switch(str_begin.pywz){
        //         case tra_q:
        //             break;
        //         case tra_kq:
        //             break;
        //         case tra_kh:
        //             break;
        //         case tra_h:
        //             break;
        //     }
        //     break;
        // case tra_kh:
        //     switch(str_begin.pywz){
        //         case tra_q:
        //             break;
        //         case tra_kq:
        //             break;
        //         case tra_z:
        //             break;
        //         case tra_h:
        //             break;
        //     }
        //     break;
        case tra_h://后平移(有电机的呢个方向)
            while(1){
                fun_startdj(mot_dj2,-100);
                while(in_hpy==1);
                fun_delay(20,del_ms);
                if(in_hpy==0)
                    break;
            }
            break;
        default:
            break;
    }
    fun_stop(mot_dj2);
    str_begin.pywz=par_model;//存储运行结果
}//平移单步运动
void fun_hz1(enum varENU_dir par_model){
    switch(par_model){
        case dir_up://回转至前方
            switch(str_begin.hzfx){
                case dir_down://现在在下方
                    fun_startdj(mot_dj4,-100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    while(1){
                        while(in_hz==0);
                        fun_delay(25,del_ms);
                        if(in_hz==1)
                            break;
                    }
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_left://现在在左边
                    fun_startdj(mot_dj4,100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_right://现在在右边
                    fun_startdj(mot_dj4,-100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        case dir_down://要去下面
            switch(str_begin.hzfx){
                case dir_up://现在在上面
                    fun_startdj(mot_dj4,100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    while(1){
                        while(in_hz==0);
                        fun_delay(25,del_ms);
                        if(in_hz==1)
                            break;
                    }
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_left://现在在左面
                    fun_startdj(mot_dj4,-100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_right://现在在右面
                    fun_startdj(mot_dj4,100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        case dir_left://要去左边
            switch(str_begin.hzfx){
                case dir_up://现在在上面
                    fun_startdj(mot_dj4,-100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_down://现在在下面
                    fun_startdj(mot_dj4,100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_right://现在在右面
                    fun_startdj(mot_dj4,-100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    while(1){
                        while(in_hz==0);
                        fun_delay(25,del_ms);
                        if(in_hz==1)
                            break;
                    }
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0){
                            break;
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case dir_right://要去右面
            switch(str_begin.hzfx){
                case dir_up://现在在前面
                    fun_startdj(mot_dj4,100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_down://现在在下面
                    fun_startdj(mot_dj4,-100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    break;
                case dir_left://现在在左面
                    fun_startdj(mot_dj4,100);
                    fun_select(sel_912);
                    fun_delay(1,del_s);
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0)
                            break;
                    }
                    while(1){
                        while(in_hz==0);
                        fun_delay(25,del_ms);
                        if(in_hz==1)
                            break;
                    }
                    while(1){
                        while(in_hz==1);
                        fun_delay(25,del_ms);
                        if(in_hz==0){
                            break;
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    fun_stop(mot_dj4);
    str_begin.hzfx=par_model;//存储运行结果
}//回转单步运动
void fun_mptline(uc par_num,uc par_sd,enum varENU_dir par_model){
    bit loc_flag=0;
    ui loc_i;
    uc loc_con=0;
    uc loc_l=par_sd,loc_r=par_sd;
    for(loc_i=2;loc_i<par_sd;fun_startdj(mot_rl,loc_i++))//软起动
        fun_delay(str_cod.mlinerqd/par_sd,del_ms);
    while(1){
        loc_l=par_sd;//恢复默认参数
        loc_r=par_sd;
        if((in_ls1&&in_ls7)||(in_ls2&&in_ls8)){//巡线计数
            loc_flag=1;
            if(loc_con>=par_num){
                if(par_model==dir_left){//左转
                    fun_startdj(mot_l,-20);
                    fun_delay(500,del_ms);
                    while(1){
                        while(!in_ls4||!in_ls5);
                        fun_delay(100,del_us);
                        if(in_ls4&&in_ls5){
                            fun_stop(mot_rl);
                            return;
                        }
                    }
                }
                else if(par_model==dir_right){
                    fun_startdj(mot_r,-20);
                    fun_delay(500,del_ms);
                    while(1){
                        while(!in_ls4||!in_ls5);
                        fun_delay(100,del_us);
                        if(in_ls4&&in_ls5){
                            fun_stop(mot_rl);
                            return;
                        }
                    }
                }
            }
        }
        else if(loc_flag==1){
            if(++loc_con>=par_num){
                if(par_model==dir_up){
                    for(loc_i=0;loc_i<str_cod.mlineqc;loc_i++){
                        loc_l=par_sd*0.7;//恢复默认参数
                        loc_r=par_sd*0.7;
                        if(in_ls3){//纠偏
                            loc_l*=0.9;
                            loc_r*=1.1;
                        }
                        if(in_ls6){
                            loc_l*=1.1;
                            loc_r*=0.9;
                        }
                        if(in_ls2){
                            loc_l*=0.8;
                            loc_r*=1.2;
                        }
                        if(in_ls7){
                            loc_l*=1.2;
                            loc_r*=0.8;
                        }
                        if(in_ls1){
                            loc_l*=0.7;
                            loc_r*=1.3;
                        }
                        if(in_ls8){
                            loc_l*=1.3;
                            loc_r*=0.7;
                        }
                        fun_startdj(mot_l,loc_l);//更新电机参数
                        fun_startdj(mot_r,loc_r);
                        fun_delay(1,del_ms);
                    }
                    fun_stop(mot_rl);
                    return;
                }
            }
            loc_flag=0;
        }
        if(in_ls3){//纠偏
            loc_l*=0.9;
            loc_r*=1.1;
        }
        if(in_ls6){
            loc_l*=1.1;
            loc_r*=0.9;
        }  
        if(in_ls2){
            loc_l*=0.8;
            loc_r*=1.2;
        }
        if(in_ls7){
            loc_l*=1.2;
            loc_r*=0.8;
        }
        if(in_ls1){
            loc_l*=0.7;
            loc_r*=1.3;
        }
        if(in_ls8){
            loc_l*=1.3;
            loc_r*=0.7;
        }
        fun_startdj(mot_l,loc_l);//更新电机参数
        fun_startdj(mot_r,loc_r);
    }
}//主函数普通巡线
void fun_stope2prom(){
    IAP_CONTR = 0;                  //关闭IAP功能
    IAP_CMD = 0;                    //清除命令
    IAP_TRIG = 0;                   //清除触发寄存器
    IAP_ADDRH = 0x80;               //数据指针指向非EEPROM区
    IAP_ADDRL = 0;                  //清除IAP地址
}//关闭EEPROM功能(IapIdle)
uc fun_reade2prom(ui par_add){
    uc loc_dat;                     //数据缓冲区
    IAP_CONTR = 0x83;               //打开EEPROM功能,设置等待时间
    IAP_CMD = 1;                    //设置EEPROM读命令
    IAP_ADDRL = par_add;            //设置EEPROM地址低八位
    IAP_ADDRH = par_add >> 8;       //设置EEPROM地址高八位
    IAP_TRIG = 0x5a;                //触发
    IAP_TRIG = 0xa5;                //再次触发
    fun_delay(10,del_us);           //稍等一会儿
    loc_dat = IAP_DATA;             //读出EEPROM中的数据
    fun_stope2prom();               //关闭EEPROM功能
    return loc_dat;                 //返回读取结果
}//读取EEPROM数据
void fun_writee2prom(ui par_add,uc par_dat){
    IAP_CONTR = 0x83;               //打开EEPROM功能,设置等待时间
    IAP_CMD = 2;                    //设置EEPROM写入命令
    IAP_ADDRL = par_add;            //设置EEPROM地址低八位
    IAP_ADDRH = par_add >> 8;       //设置EEPROM地址高八位
    IAP_DATA = par_dat;             //写入数据
    IAP_TRIG = 0x5a;                //触发
    IAP_TRIG = 0xa5;                //再次触发
    fun_delay(10,del_us);           //稍等一会儿
    fun_stope2prom();               //关闭EEPROM功能
}//写EEPROM数据
void fun_cleane2prom(ui par_add){
    IAP_CONTR = 0x83;               //打开EEPROM功能,设置等待时间
    IAP_CMD = 3;                    //设置EEPROM擦除命令
    IAP_ADDRL = par_add;            //设置EEPROM地址低八位
    IAP_ADDRH = par_add >> 8;       //设置EEPROM地址高八位
    IAP_TRIG = 0x5a;                //触发
    IAP_TRIG = 0xa5;                //再次触发
    fun_delay(10,del_us);           //稍等一会儿
    fun_stope2prom();               //关闭EEPROM功能
}//清除EEPROM数据

