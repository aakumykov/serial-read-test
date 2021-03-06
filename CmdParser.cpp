#include <Arduino.h>

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
    unsigned int* _data = new unsigned int[this->_max_input_len];
    byte _cmd;
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
        Serial.print(F("CmdParser.processData('"));
        Serial.print(str);
        Serial.println(F("')"));
        
        char* work_string = strchr(str, this->_command_delimiter);
        
        while (NULL != work_string) {
          work_string += 1;
            //Serial.print(F("work_string: ")); Serial.println(work_string);
          
          char* piece = getPieceBefore(work_string, this->_data_delimiter);
            //Serial.print(F("piece: ")); Serial.println(piece);
            
          unsigned int num = (unsigned int)atol(piece);
            Serial.print(F("num: ")); Serial.println(num);
          
          this->_data[this->_counter] = num;
            
          delete piece;
          
          this->_counter++;
          
          work_string = strchr(work_string, this->_data_delimiter);
        }
    
        delete work_string;

        Serial.print(F("this->_counter: ")); Serial.println(this->_counter);

        Serial.print(F("this->_data: "));
        for(int i=0; i<this->_counter; i++) {
          Serial.print(this->_data[i]);
          Serial.print(F(","));
        } Serial.println(F(""));
    }
    
    // служебные методы
    char* getPieceBefore(char* str, char delimiter) {
//      Serial.print(F("CmdParser.getPieceBefore("));
//        Serial.print(str);
//        Serial.print(F(", "));
//        Serial.print(delimiter);
//      Serial.println(F(")"));
      
      char* arr_delimiter = this->char2ptr(delimiter);
        byte len = strcspn(str, arr_delimiter);
      delete arr_delimiter;
    
      char* piece = new char[len + 1]; // +1 для нулевого символа
      strncpy(piece, str, len);
      piece[len] = char(0);
        //Serial.print(F("piece: ")); Serial.println(piece);
      
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

