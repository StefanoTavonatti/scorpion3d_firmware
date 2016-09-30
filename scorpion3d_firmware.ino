
const int TIMEOUT=50;
const int EXTRUDER_MOTOR=9;
const int X_MOTOR=22;
const int X_DIR=23;
const int Y_MOTOR=24;
const int Y_DIR=25;
const int Z_MOTOR=26;
const int Z_DIR=27;
const int TERMAL_SENSOR_PIN=A0;

const String VOID_COMMAND="VOID";

bool isPrinting;
String lastCommand[6];//Field are not reset every cycle
bool newCommand=false;


void parseCommand(String command);
void printCommandToSerial(); //For debugging

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
        printCommandToSerial();
        //TODO clear old
    }
  }
  else{
    
  }

}

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

