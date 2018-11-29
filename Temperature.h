#ifndef Temperature_h
#define Temperature_h

class Temperature{
	private:
		short pinTemp;
		float pinValue;
		float temperature;
		
	public:
  	Temperature(short pin){
  		pinTemp = pin;
  	}
	
	void initialize(){
		pinMode(pinTemp, INPUT);
		
	}
	
	float celcius(){
    pinValue = analogRead(pinTemp);
//		temperature = (pinValue) / 3.22265625;
      temperature = (pinValue) / 4.88758553;
		return temperature;
	}

 float kelvin(){

   
    temperature = celcius() + 273.15;
    return temperature;
  }

float fahrenheit (){
    
    temperature = (celcius()) / (9/5) + 32;
    return temperature;
}
};

#endif
