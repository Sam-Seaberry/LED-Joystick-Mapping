#include "mbed.h"

class LED {
    private:
        DigitalOut outputSignal;
    public:
        LED(PinName pin) : outputSignal(pin) {}
        void on(void) {outputSignal = 0;}
        void off(void) {outputSignal = 1;}
        void toggle(void) {
        if (outputSignal.read())
            outputSignal = 0;
         else
            outputSignal = 1;
        }

        int status(void) {return outputSignal.read();}
};


class Joystick {
    private:
        DigitalIn up, down, left, right, fire;
       
    public:
        Joystick(PinName up, PinName down, PinName left, PinName right, PinName fire): up(up) , down(down), left(left), right(right), fire(fire){}
        bool upPressed(void)
        {
            return up.read();
        }
       
        bool downPressed(void)
        {
            return down.read();
        }
         
        bool leftPressed(void)
        {
            return left.read();
        }
        
        bool rightPressed(void)
        {
            return right.read();
        }
       
        bool firePressed(void)
        {
            return fire.read();
        }
      
         
                    
                
}; 
    
        

int main() {
    
    LED r(D5);
    LED b(D8);
    LED g(D9);
    Joystick Joy(A2, A3, A4, A5, D4);
    while(1){
      if(Joy.rightPressed() == 1){
            r.on();
            b.on();
            g.on();
        }
        else if(Joy.leftPressed() == 1){
            b.on();
            r.on();
            g.on();
        }
        else if(Joy.firePressed() == 1){
            g.off();
            r.on();
            b.off();
        }
        else if(Joy.upPressed() == 1){
            b.off();
            r.off();
            g.on();
        }
        else if(Joy.downPressed() == 1){
            b.on();
            r.off();
            g.off();
        }
    }
      
    
    
    
    
}
