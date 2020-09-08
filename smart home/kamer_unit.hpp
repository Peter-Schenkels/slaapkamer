




#include "../hwlib/library/hwlib.hpp"


namespace target = hwlib::target;

class ultra_sonar{

private:
    target::pin_out trigger = target::pin_out(target::pins::d5);
    target::pin_in echo = target::pin_in(target::pins::d6);
    int distance = 0; //in cm

public:

    ultra_sonar(){}

    void trigger_signal(){
        trigger.write(1);
        trigger.flush();
        hwlib::wait_ms(1);
        trigger.write(0);
        trigger.flush();
    }

    void read(){

        while(!echo.read()){
            echo.refresh();
        }
        int time = hwlib::now_us();
        while(echo.read()){
            echo.refresh();
        }
        distance = (hwlib::now_us() - time) * 0.0343 / 2;

    }

    int get_distance(){
        return distance;
    }

};



class message_sender{

    target::pin_out clk = target::pin_out( target::pins::d7 );
    target::pin_out data = target::pin_out( target::pins::d8 );

public:

    message_sender(){
        data.write(0);
        data.flush();

        clk.write(0);
        clk.flush();
    }
    void send_message(uint8_t message){
        clk.write(0);
        data.write(1);
        clk.flush();
        data.flush();
        hwlib::wait_ms(5);
        data.write(0);
        data.flush();
        hwlib::wait_ms(1);
        for(unsigned int i = 0; i < 8; i++){
            data.write((message >> i) & 1);
            clk.write(1);
            data.flush();
            clk.flush();
            hwlib::wait_ms(1);
            clk.write(0);
            clk.flush();
            hwlib::wait_ms(1);
        }
        data.write(0);
        data.flush();
    }
};


class kamer_unit{

private:
 
    target::pin_oc scl = target::pin_oc( target::pins::d4 );
    target::pin_oc sda = target::pin_oc( target::pins::d3 );

    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );

    const uint8_t MPU_REGISTER = 0x68;
    const uint8_t WHO_AM_I_MPU = 0x75;
    const uint8_t TEMP_REGISTER = 0x41;
    const uint8_t WAKE_MPU = 0x6B;

    message_sender sender = message_sender();

    ultra_sonar sonar = ultra_sonar();

    uint16_t current_tempurature = 0;

    hwlib::glcd_oled oled = hwlib::glcd_oled( i2c_bus, 0x3c );  

    hwlib::window_part_t top = hwlib::part( 
        oled, 
        hwlib::xy( 0, 0 ),
        hwlib::xy( 128, 16));
    hwlib::window_part_t bottom = hwlib::part( 
        oled, 
        hwlib::xy( 0, 16 ),
        hwlib::xy( 128, 48));
    hwlib::font_default_16x16 f1 = hwlib::font_default_16x16();
    hwlib::font_default_8x8 f2 = hwlib::font_default_8x8();
    hwlib::terminal_from d1 = hwlib::terminal_from( top, f1 );
    hwlib::terminal_from d2 = hwlib::terminal_from( bottom, f2 );

    bool behind_desk = false;
public:

    kamer_unit(){

        d1 << "\f" << "Kamer API";
        d2 << "";
        oled.flush();
        start_gyro();

    }

    void main(){
        for(;;){
            hwlib::wait_ms(100);
            sender.send_message('a');
            // read_temprature();
            // sonar.trigger_signal();
            // sonar.read();
            // check_desk();
            // d1 << "\f" << "Kamer:\n";
            // d2 << "\f===========================\n" << "Temp:" 
            // << current_tempurature << "C\n" << "Behind desk:" 
            // << behind_desk << "\nDistance:" 
            // << sonar.get_distance() << "\n\n=====================================";
            // oled.flush();

      
        }

    }

    void start_gyro(){
        const uint8_t target[2] = {WAKE_MPU, 0x00};
        i2c_bus.i2c_bus::write(MPU_REGISTER).write( target, 2);
    }

    void read_temprature(){
        uint8_t TEMP_HI, TEMP_LO = 0x00;
        i2c_bus.i2c_bus::write(MPU_REGISTER).write(TEMP_REGISTER);
        i2c_bus.i2c_bus::read(MPU_REGISTER).read(TEMP_HI);
        i2c_bus.i2c_bus::write(MPU_REGISTER).write(TEMP_REGISTER+1);
        i2c_bus.i2c_bus::read(MPU_REGISTER).read(TEMP_LO);
        uint16_t data = TEMP_HI << 8 | TEMP_LO;
        current_tempurature = uint16_t(data / 340 - 165 );
    }

    void check_desk(){
        if(sonar.get_distance() < 70){

            behind_desk = true;
        }else{
            behind_desk = false;
        }

    }

    void display_text(const char * text){
        d1 << "\f" << text;
    }

};



