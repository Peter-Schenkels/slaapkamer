#include <Arduino.h>


class message_receiver{

    const unsigned int clk = 0;
    const unsigned int data = 1;

    unsigned int buffer = 0;

public:

    message_receiver(){
        pinMode(clk, INPUT);
        pinMode(data, INPUT);

    }


    void listen_for_message(){
        unsigned long time = millis();
        while(!digitalRead(data)){
            if(millis() - time > 25){
                return;
            }
        }
        time = millis();
        while (digitalRead(data));
        if( millis() - time < 1){
            return;
        }
        while(!digitalRead(clk));
        for(unsigned int i = 0; i < 8; i++){
            buffer ^= (-digitalRead(data) ^ buffer) & (1UL << i);
            while(digitalRead(clk));
            while(!digitalRead(clk));
        }

        Serial.print(buffer);

    }




};