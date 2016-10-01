// Pin that the thermistor is connected to
#define PINOTERMISTOR A0
// Nominal temperature value for the thermistor
#define TERMISTORNOMINAL 100000
// Nominl temperature depicted on the datasheet
#define TEMPERATURENOMINAL 25
// Number of samples 
#define NUMAMOSTRAS 5
// Beta value for our thermistor
#define BCOEFFICIENT 3950
// Value of the series resistor
#define SERIESRESISTOR 100000

const int TIMEOUT=50;
const int EXTRUDER_MOTOR=9;
const int X_MOTOR=22;
const int X_DIR=23;
const int Y_MOTOR=24;
const int Y_DIR=25;
const int Z_MOTOR=26;
const int Z_DIR=27;

const String VOID_COMMAND="VOID";

bool isPrinting;
String lastCommand[6];//Field are not reset every cycle
bool newCommand=false;


void parseCommand(String command);
void printCommandToSerial(); //For debugging

float readTemperatureC();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(TIMEOUT);

  isPrinting=false;

  //TODO termal sensor calibration

  lastCommand[0]=VOID_COMMAND;
  Serial.println("Ready");

}

void loop() {
  // put your main code here, to run repeatedly:

  float temperature=readTemperatureC();

  if(!isPrinting){
    if(Serial.available()>0){
        String cmd=Serial.readString();
        newCommand=true;
        parseCommand(cmd);
        printCommandToSerial();
        //TODO clear old

        if(lastCommand[0]=="TEMP"){
          Serial.print("TEMP ");
          Serial.println(temperature);
        }
    }
  }
  else{
    
  }

}

/**
 * Split command by spaces
 */
void parseCommand(String command){
 
  int lastCommandIndex = 0;
  int lastSubstringIndex = 0;
  
  for(int i=0;i<command.length();i++){
    if(command.charAt(i)==' '){
      lastCommand[lastCommandIndex]=command.substring(lastSubstringIndex,i);
      //Serial.println("partial:"+command.substring(lastSubstringIndex,i));
      lastSubstringIndex = i+1;
      lastCommandIndex++;
    }
  }

  lastCommand[lastCommandIndex]=command.substring(lastSubstringIndex,command.length());
}

void printCommandToSerial(){
  Serial.print(lastCommand[0]+" ");
  Serial.print(lastCommand[1]+" ");
  Serial.print(lastCommand[2]+" ");
  Serial.print(lastCommand[3]+" ");
  Serial.print(lastCommand[4]+" ");
  Serial.println(lastCommand[5]+" ");
  
}

/**
 * read the temperature from thermistor in the printing head
 */
float readTemperatureC(){
  //read sensor 5 times then compute the average
  float avg=0;
  for (int i=0; i< NUMAMOSTRAS; i++) {
    avg+= analogRead(PINOTERMISTOR);
    delay(10);
  }

  avg/=NUMAMOSTRAS;

  //Calculating thermistore resistance with Voltage partitor formula
  float vin=5;
  float vout=vin*avg/1023;
  
  
  float alfa=vout/vin;
  
  float r1=(SERIESRESISTOR/alfa)-SERIESRESISTOR;
  
  //Calculate temperature using the Beta Factor equation
  float temperatura;
  temperatura = r1 / TERMISTORNOMINAL;     // (R/Ro)
  temperatura = log(temperatura); // ln(R/Ro)
  temperatura /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  temperatura += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  temperatura = 1.0 / temperatura;                 // Invert the value
  temperatura -= 273.15;                         // Convert it to Celsius

  return temperatura;
}

