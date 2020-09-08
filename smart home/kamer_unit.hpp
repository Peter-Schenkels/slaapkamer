




#include "../hwlib/library/hwlib.hpp"


namespace target = hwlib::target;

class kamer_unit{

private:
 
    target::pin_oc scl = target::pin_oc( target::pins::d4 );
    target::pin_oc sda = target::pin_oc( target::pins::d3 );

    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );

    const uint8_t MPU_REGISTER = 0x68;
    const uint8_t WHO_AM_I_MPU = 0x75;
    const uint8_t TEMP_REGISTER = 0x41;
    const uint8_t WAKE_MPU = 0x6B;

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

public:

    kamer_unit(){

        display_text("Soest \nAPI");
        oled.flush();
        start_gyro();

    }

    void main(){
        for(;;){
            hwlib::wait_ms(2000);
            read_temprature();
            d1 << "\f" << "Kamer:\n";
            d2 << "\f===========================\n" << "Temp:" << current_tempurature << "C\n\n\n=====================================";
            oled.flush();
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


    void display_text(const char * text){
        d1 << "\f" << text;
    }

};


class IRReceiver {

    uint32_t message;
    





}

