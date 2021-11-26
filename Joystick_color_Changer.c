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
class Color{
    private:
        typedef enum {None, Red, Green, Blue} LEDColourType;
        LEDColourType state;
        int statusled;
        LED red, green, blue;
        DigitalIn fire;
        LED* lightr;
        LED* lightg;
        LED* lightb;
        
    public:
        Color(PinName p1, PinName p2, PinName p3,PinName fire, LED* r, LED* g,LED* b) 
        : red(p1), green(p2), blue(p3), fire(fire), lightr(r), lightg(g),lightb(b) {
        state = None;
        statusled= 0;
        
        }  
       void toggle(void){
           switch(state){
                case(Red):
                    lightr->toggle();
                    break;
                case(Green):
                    lightg->toggle();
                    break;
                case(Blue):
                    lightb->toggle();
                    break;
            }
                    
        }
       
       void Start(void){
        switch (state) {
            case (Red) : // if red is on
                red.off(); // turn it off
                green.on(); // turn green on
                state = Green; // state is now green (i.e. green light on)
                break;
            case (Green) :
                green.off();
                blue.on();
                state = Blue;
                break;
            case (Blue) :
                blue.off();
                red.on();
                state = Red;
                break;
            default :
                green.off();
                blue.off();
                red.on();
                state = Red;
                }
           } 
        
};
class Joystick {
    private:
        
        InterruptIn fire;
        Color* Col;
        int state;
       
    public:
        Joystick(PinName pin, Color* C): fire(pin), Col(C){
            fire.rise(callback(this, &Joystick::rise));
            fire.fall(callback(this, &Joystick::fall));
            state =0;
            }
        
        bool firePressed(void)
        {
            fire.rise(callback(this, &Joystick::rise));
            fire.fall(callback(this, &Joystick::fall));
            return fire.read();
        }
        void rise(void){
            Col->Start();
            state = 0;
            }
        void fall(void){
            Col->Start();
            state=1;
            }
      
         
                    
                
}; 
class Potentiometer {
    private:
        AnalogIn inputSignal;
        float VDD, currentSamplePC, currentSampleVolts;
    public:
        Potentiometer(PinName pin, float v) : inputSignal(pin), VDD(v) {}
        float amplitudeVolts(void) { return (inputSignal.read()*VDD); }
        float amplitudePC(void) { return inputSignal.read(); }
        
        void sample(void) {
            currentSamplePC = inputSignal.read();
            currentSampleVolts = currentSamplePC * VDD;
        }
        
        float getCurrentSampleVolts(void) { 
            return currentSampleVolts; 
        }
        
        float getCurrentSamplePC(void) {
             return currentSamplePC; 
        }

};

class FlashRate {
    private:
        Potentiometer* pot;
        Color* Col;
        Joystick* Joy;
        
// left/right sample and frequency values for linear relationship
        const float smin, smax, fmin, fmax;
        float gdt, incpt, period;
        Timeout to;
// calculate gradient and intercept of linear relationship
        float calc_gdt(void){
            return (fmax-fmin)/(smax-smin);
            }
        float calc_incpt(void){
             return (fmax - gdt*smax);
             }
    public:
        FlashRate(Potentiometer* p,Color* c, Joystick* j, float xmn,float xmx, float ymn, float ymx): pot(p), Col(c), Joy(j), smin(xmn),smax(xmx), fmin(ymn), fmax(ymx) {
            gdt = calc_gdt();
            incpt = calc_incpt();
            to.attach(callback(this, &FlashRate::updateRate), 0.01);

           
            }
        void updateRate(void){
            Joy->firePressed();         
            Col->toggle();    
            
            
            period = 1.0f/(gdt*pot->getCurrentSampleVolts() + incpt); // set next to
            to.attach(callback(this, &FlashRate::updateRate), period/3.0f); 
            }
}; 
int main() {
    
    Potentiometer* PLeft = new Potentiometer(A0, 3.3);
    LED* lightr= new LED(D5);
    LED* lightg= new LED(D8);
    LED* lightb= new LED(D9);
    Color* RGB = new Color(D5, D8, D9, D4,lightr,lightg,lightb);
    
    Joystick* Push = new Joystick(D4, RGB);
    FlashRate t(PLeft,RGB, Push,0.0, 3.3, 3.0, 7.0);
    
    
    while(1){
        Push->firePressed();
        PLeft->sample();
        wait(.1);
    }
        
        
    
    
    

   
}

