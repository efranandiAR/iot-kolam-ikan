#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

#define turbidityPin A1
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.18            //deviation compensate
#define samplingInterval 20
#define printInterval 300000
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

#define feedingInterval 43200000
Servo myservo;
int pos = 0;

#define actuatorInterval 6000
#define buttonModeIkan 47
int modeIkan = 0; //mode 0: lele; mode 1: nila; mode 2: gurame

int pinPhUp = 48;
int pinPhDown = 49;
int pinTambahAir = 50;
int pinKurangiAir = 51;

//Variabel sensor suhu
const int oneWireBus = 52;
float temperatureC;    

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//Variabel sensor ketinggian air
#define sensorKetinggianAir 53
int readKetinggianAir;
int ketinggianAir;

//Variabel transfer data
String kirim = "";

void setup(void)
{
  Serial.begin(9600);
  Serial3.begin(115200);
  
  pinMode(52, INPUT);
  pinMode(53, INPUT_PULLUP);
  pinMode(buttonModeIkan, INPUT_PULLUP);

  myservo.attach(2);
}
void loop(void)
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static unsigned long feedingTime = millis();
  static unsigned long actuatorTime = millis();
  static float pHValue,voltagePh;
  static float ntu, voltageTurbidity;
  
  if(millis()-samplingTime > samplingInterval)
  {
      //pH
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltagePh = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = constrain((3.5*voltagePh+Offset), 2, 11);

      //Suhu
      sensors.requestTemperatures(); 
      temperatureC = constrain((sensors.getTempCByIndex(0)), 18, 38);

      //Kekeruhan
      voltageTurbidity = analogRead(turbidityPin) * (5.0/1024.0);
      ntu = constrain((-1120.4*square(voltageTurbidity) + 5742.3*voltageTurbidity - 4352.9), 0, 1000);

      //Ketinggian air
      readKetinggianAir = digitalRead(sensorKetinggianAir);
      
      samplingTime=millis();
  }
  
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
//    Serial.print("Voltage:");
//    Serial.print(voltagePh,2);
//    Serial.print("    pH value: ");
//    Serial.println(pHValue,2);
//    Serial.print(temperatureC);
//    Serial.println("ºC");
//    Serial.print("Turbidity voltage: ");
//    Serial.println(voltageTurbidity);
//    Serial.print("Turbidity: ");
//    Serial.println(ntu);
    if(readKetinggianAir == LOW){
        ketinggianAir = 0;
//        Serial.println("Ketinggian air <40");
//        Serial.println("-------");
      }else {
        ketinggianAir = 1;
//        Serial.println("Ketinggian air >40");
//        Serial.println("-------");
      }

    kirim = "";
    kirim += pHValue;
    kirim += ";";
    kirim += temperatureC;
    kirim += ";";
    kirim += ntu;
    kirim += ";";
    kirim += ketinggianAir;

    Serial3.println(kirim);

    if(Serial3.available()) {
      String msg = "";
      while(Serial3.available()) {
        msg += char(Serial3.read());
        delay(50);
      }
      Serial.println(msg);
    }
    printTime=millis();
  }

  if(millis() - feedingTime > feedingInterval) {
    for(pos = 0; pos >= -45; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }
    delay(2000);
    for(pos = -45; pos <= 0; pos += 1) {
      myservo.write(pos);
      delay(15);
    }
    feedingTime = millis();
  }

  if(buttonModeIkan == LOW) {
    modeIkan++;
    if(modeIkan == 3) {
      modeIkan = 0;
    }
    delay(1000);
  }
  
  if(millis() - actuatorTime > actuatorInterval) {
    switch (modeIkan) {
      case 0:
        if(pHValue < 6.5) {
          digitalWrite(pinPhUp, LOW);
          delay(5000);
        } else if(pHValue > 8.0) {
          digitalWrite(pinPhDown, LOW);
          delay(5000);
        }
        
        if(temperatureC > 30) {
          digitalWrite(pinTambahAir, LOW);
          delay(10000);
        }

        if(ntu > 750) {
          digitalWrite(pinTambahAir, LOW);
          delay(10000);
        }

        if(ketinggianAir == 1) {
          digitalWrite(pinKurangiAir, LOW);
          delay(5000);
        } else if(ketinggianAir ==0) {
          digitalWrite(pinTambahAir, LOW);
          delay(5000);
        }
        break;
        
      case 1:
        if(pHValue < 6.5) {
          digitalWrite(pinPhUp, LOW);
          delay(5000);
        } else if(pHValue > 7.5) {
          digitalWrite(pinPhDown, LOW);
          delay(5000);
        }
        
        if(temperatureC > 32) {
          digitalWrite(pinTambahAir, LOW);
          delay(10000);
        }

        if(ntu > 1500) {
          digitalWrite(pinTambahAir, LOW);
          delay(10000);
        }

        if(ketinggianAir == 1) {
          digitalWrite(pinKurangiAir, LOW);
          delay(5000);
        } else if(ketinggianAir ==0) {
          digitalWrite(pinTambahAir, LOW);
          delay(5000);
        }
        break;

      case 2:
        if(pHValue < 6.5) {
          digitalWrite(pinPhUp, LOW);
          delay(5000);
        } else if(pHValue > 8) {
          digitalWrite(pinPhDown, LOW);
          delay(5000);
        }
        
        if(temperatureC > 30) {
          digitalWrite(pinTambahAir, LOW);
          delay(10000);
        }

        if(ntu > 1500) {
          digitalWrite(pinTambahAir, LOW);
          delay(10000);
        }

        if(ketinggianAir == 1) {
          digitalWrite(pinKurangiAir, LOW);
          delay(5000);
        } else if(ketinggianAir ==0) {
          digitalWrite(pinTambahAir, LOW);
          delay(5000);
        }
        break;
    }
  }
}

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
