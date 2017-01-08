#include "CmdParser.cpp"
#include <MemoryFree.h>

void showMem(char* comment=NULL, bool newLine=false){
  //Serial.println(F(""));
  Serial.print(F("Free memory"));
  if (NULL != comment)  {
    Serial.print(F(" "));
    Serial.print(comment);
  }
  Serial.print(F(": "));
  Serial.println(freeMemory());
  if (newLine) Serial.println(F(""));
}


const int max_len = 255;

char recieved_data[max_len];

unsigned int* data = new unsigned int[max_len];

CmdParser parser(255, '|', '_', ':');

/*
123|1_2_3_4_5_6_7_8_9_0;
456|1_2_3;
*/

void setup() {
  Serial.begin(9600); while(!Serial);
  Serial.println(F("=native-serial-read-test="));
  Serial.println(F("123|11_22_33;"));
  Serial.println(F("456|1_2_3;"));
  showMem("on setup",true);
}

void loop() {

/*
Вешающая последовательность:
123|11_22_33;
456|1_2_3;
*/
  
  if (Serial.available() > 0) {
  
     int recieved_len = Serial.readBytesUntil(';', recieved_data, max_len);
      recieved_data[recieved_len] = char(0);
     Serial.print("recieved: "); Serial.println(recieved_data);
     Serial.print("len: "); Serial.println(recieved_len);
     //showMem("after recieve");

     parser.parse(recieved_data);
     //showMem("after parse");

     int data_count = parser.count();
     data = parser.data();
     //showMem("after assigning");

     Serial.print("data["); Serial.print(data_count); Serial.print("]: ");
     for (int i=0; i<data_count; i++) {
        Serial.print(data[i]);
        Serial.print(",");
     } Serial.println("");
     showMem("",true);

//     delete data; // и с этим, и без этого УТЕКАЕТ ПАМЯТЬ!
//     showMem("after deleting *data",true);
  }
}
