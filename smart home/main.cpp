#include "hwlib.hpp"
#include "kamer_unit.hpp"

int main( void ){	
   
      // wait for the PC console to start
   hwlib::wait_ms( 500 );   
   
   auto kamer = kamer_unit();
   kamer.main();

   namespace target = hwlib::target;
  
   
   
}

