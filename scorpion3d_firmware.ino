
const int TIMEOUT=50;
const int EXTRUDER_MOTOR=9;
const int X_MOTOR=22;
const int Y_MOTOR=24;
const int Z_MOTOR=26;
const int TERMAL_SENSOR_PIN=A0;

const String VOID_COMMAND="VOID";

bool isPrinting;
String lastCommand[4];
bool newCommand=false;


void parseCommand(String command);

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

  

  if(!isPrinting){
    if(Serial.available()>0){
        String cmd=Serial.readString();
        newCommand=true;
        parseCommand(cmd);
    }
  }
  else{
    
  }

}

void parseCommand(String command){
  //String result[]= new String[3];
}

