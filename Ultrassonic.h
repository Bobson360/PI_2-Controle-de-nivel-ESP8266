#ifndef Ultrassonic_h
#define Ultrassonic_h
#include "Temperature.h"

class Ultrassonic{
  private:
    float _speed;
    float _distance;
    short _trig;
    short _echo;
    long  _duration;

  public: Ultrassonic(short pinTrig, short pinEcho){
    _trig = pinTrig;
    _echo = pinEcho;
  }

  long pulse(){
      digitalWrite(_trig, LOW);
      delayMicroseconds(2);
      digitalWrite(_trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trig, LOW);

      _duration = pulseIn(_echo, HIGH);

      return _duration / 2;
  }

  void initialize(){
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
  }

  float speedOfSoundWithTemp(short Temp){// recebe a temperatura em fahrenheit
    _speed = 331.45 * (sqrt(Temp/273.15));

    return _speed;
    
  }

 float distance (){
    return pulse() * 0.034;
  }

 int distance( short Temp ){
    return pulse() * speedOfSoundWithTemp( Temp );
 }

 float distance( String unit, short Temp ){// retorn distance in meters or inches, receive M or I as parameter
    if( unit == "M" ){
        return pulse() * speedOfSoundWithTemp( Temp );
    } else if( unit == "I" ){
        return pulse() * speedOfSoundWithTemp( Temp ) * 0.393701;
    }else{
        return pulse() * speedOfSoundWithTemp( Temp );
    }
 }

 float distance( String unit ){
    if( unit == "M" ){
        return pulse() *  0.034;
    } else if( unit == "I" ){
        return pulse() *  0.034 * 0.393701;
    }else{
        return pulse() *  0.034;
    }
 }

};
#endif
