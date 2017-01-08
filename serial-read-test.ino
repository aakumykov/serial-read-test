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

char* str2ptr(char* str) {
  unsigned int len = strlen(str);
  char* res = new char[len+1];
  for (int i=0; i<len; i++) res[i] = str[i];
  res[len] = char(0);
  return res;
}

class CmdParser {
  public:
    CmdParser(int max_input_len, char command_delimiter, char data_delimiter, char mode_delimiter) {
      this->_max_input_len = max_input_len;
      this->_command_delimiter = command_delimiter;
      this->_data_delimiter = data_delimiter;
      this->_mode_delimiter = mode_delimiter;
    }
    
    byte cmd() {
      //Serial.println(F("CmdParser.cmd()"));
        //Serial.print(F("cmd: ")); Serial.println(this->_cmd);
      return this->_cmd;
    }
    byte count() {
      return this->_counter;
    }
    byte length() {
      return this->count();
    }
    unsigned int* data() {
      this->clear();
      return this->_data;
    }
    void clear() {
      this->_counter = 0;
    }
    byte parse(char* str) {
//      Serial.println(F("CmdParser.parse()"));
      this->processCmd(str);
      this->processData(str);
      return this->count();
    }
    void debug() {
      Serial.print(F("len: ")); Serial.println(this->_counter);
      Serial.print(F("cmd: ")); Serial.println(this->_cmd);
      Serial.print(F("data: "));
      for (int i=0; i < this->_counter; i++) {
        Serial.print(this->_data[i]);
        Serial.print(F(", "));
      } Serial.println(F(""));
    }
    
  private:
    // полезные данные
    byte _cmd;
    unsigned int* _data = new unsigned int[this->_max_input_len];
    byte _counter = 0;

    // служебные данные
    char _command_delimiter;
    char _data_delimiter;
    char _mode_delimiter;
    int _max_input_len;

    // полезные методы
    byte processCmd(char* str) {
      //Serial.println(F("CmdParser.processCmd()"));
      char* cmd_piece = getPieceBefore(str,this->_command_delimiter);
      this->_cmd = (byte)atoi(cmd_piece);
      delete cmd_piece;
    }

    unsigned int* processData(char* str) {
        //Serial.println(F("CmdParser.processData()"));
        
        char* work_string = strchr(str, this->_command_delimiter);
    
        while (NULL != work_string) {
          work_string += 1;
          
          char* piece = getPieceBefore(work_string, this->_data_delimiter);
            //showString(piece, "piece");
            
          this->_data[this->_counter] = (unsigned int)atol(piece);
          
          delete piece;
          
          this->_counter++;
          
          work_string = strchr(work_string, this->_data_delimiter);
            //showString(work_string, "work_string", true);
        }
    
        delete work_string;
    }
    
    // служебные методы
    char* getPieceBefore(char* str, char delimiter) {
      //Serial.println(F("CmdParser.getPieceBefore()"));
      
      char* arr_delimiter = this->char2ptr(delimiter);
        byte len = strcspn(str, arr_delimiter);
      delete arr_delimiter;
    
      char* piece = new char[len + 1]; // +1 для нулевого символа
      strncpy(piece, str, len);
      piece[len] = char(0);
      
      return piece;
    }

    char* char2ptr(char letter) {
      //Serial.println(F("CmdParser.char2ptr()"));
      char* res = new char[2];
            res[0] = letter;
            res[1] = char(0);
      return res;
    }
};


const int max_len = 255;
char recieved_data[max_len];
unsigned int* data = new unsigned int[max_len];
CmdParser parser(255, '|', '_', ':');

/*
123|1_2_3_4_5_6_7_8_9_0;
123|1_2_3;
*/

void setup() {
  Serial.begin(9600); while(!Serial);
  Serial.println(F("=native-serial-read-test="));
  showMem("on setup",true);
}

void loop() {
  
  if (Serial.available() > 0) {
  
     int recieved_len = Serial.readBytesUntil(';',recieved_data,max_len);
     Serial.print("recieved_len: "); Serial.println(recieved_len);
     Serial.print("recieved data: "); Serial.println(recieved_data);
     showMem("after recieve");

     parser.parse(recieved_data);
     showMem("after parse");

     int data_count = parser.count();
     data = parser.data();
     showMem("after assigning");

     Serial.print("data["); Serial.print(data_count); Serial.print("]: ");
     for (int i=0; i<data_count; i++) {
        Serial.print(data[i]);
        Serial.print(",");
     } Serial.println("");
     showMem("after printing",true);

//     delete data; // и с этим, и без этого УТЕКАЕТ ПАМЯТЬ!
//     showMem("after deleting *data",true);
  }
}
