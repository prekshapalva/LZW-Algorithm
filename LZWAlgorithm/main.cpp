#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <string.h>

#define BITS 12                   
#define HASHING_SHIFT BITS-8      
#define MAX_VALUE (1 << BITS) - 1 
#define MAX_CODE MAX_VALUE - 1    
                                  
#if BITS == 14
  #define TABLE_SIZE 18041        
#endif                           
#if BITS == 13                    
  #define TABLE_SIZE 9029
#endif
#if BITS <= 12
  #define TABLE_SIZE 5021
#endif
int *code_value;                  
unsigned int *prefix_code;        
unsigned char *append_character;  
unsigned char decode_stack[4000]; 
void compress(std::ifstream &, std::ofstream &);
int find_match(int, unsigned int);
void expand(std::istream &, std::ostream &);
unsigned char *decode_string(unsigned char *, unsigned int);
int input_code(std::istream &);
void output_code(std::ostream &,unsigned int);

int main(int argc, char *argv[])
{
  std::ifstream input_file;
  std::ifstream lzw_file_i;
  std::ofstream output_file;
  std::ofstream lzw_file_o;
  char input_file_name[81];
  code_value=new int[TABLE_SIZE*sizeof(unsigned int)];
  prefix_code=new unsigned int[TABLE_SIZE*sizeof(unsigned int)];
  append_character=new unsigned char[TABLE_SIZE*sizeof(unsigned char)];
  if (code_value==NULL || prefix_code==NULL || append_character==NULL)
  {
    std::cout << "Fatal error allocating table space!\n";
    exit(0);
  }
  if (argc>1)
  {
    strcpy(input_file_name, argv[1]);
  }
  else
  {
    std::cout << "Input file name: ";
    std::cin >> input_file_name;
  }
  input_file.open(input_file_name, std::ios::binary);
  lzw_file_o.open("test.lzw", std::ios::binary);
  if (!input_file || !lzw_file_o)
  {
    std::cout << "Fatal error opening files.\n";
    exit(0);
  };
  compress(input_file,lzw_file_o);
  input_file.close();
  lzw_file_o.close(); 
  delete code_value;
  lzw_file_i.open("test.lzw", std::ios::binary);
  output_file.open("test.out", std::ios::binary);
  if (!lzw_file_i  || !output_file)
  {
    std::cout << "Fatal error opening files.\n";
    exit(0);
  };
  expand(lzw_file_i, output_file);
  lzw_file_i.close();
  output_file.close();
  delete prefix_code;
  delete append_character;
}

void compress(std::ifstream &input,std::ofstream &output)
{
  unsigned int next_code;
  unsigned int character;
  unsigned int string_code;
  unsigned int index;
  int i;
  std::ofstream output_code_file;
  output_code_file.open("outputcodes", std::ios::out);
  next_code=256;              
  for (i=0;i<TABLE_SIZE;i++)  
    code_value[i]=-1;
  i=0;
  std::cout << "Compressing...\n";
  string_code=input.get();   
  while ((character=input.get()) != (unsigned)EOF)
  {
    if (++i==1000)                         
    {                                     
      i=0;                                
      std::cout << "*";
    }
    index=find_match(string_code,character);
    if (code_value[index] != -1)            
      string_code=code_value[index];      
    else                                   
    {                                       
      if (next_code <= MAX_CODE)
      {
        code_value[index]=next_code++;
        prefix_code[index]=string_code;
        append_character[index]=character;
      }
      output_code_file << string_code << "\n";
      output_code(output,string_code);  
      string_code=character;           
    }                                 
  } 
  output_code(output,string_code);              
  output_code_file << string_code << "\n";
  output_code(output,MAX_VALUE);   
  output_code(output,0);          
  std::cout << "\n";
}

int find_match(int hash_prefix,unsigned int hash_character)
{
  int index;
  int offset;
  index = (hash_character << (HASHING_SHIFT)) ^ hash_prefix;
  if (index == 0)
    offset = 1;
  else
    offset = TABLE_SIZE - index;
  while (1)
  {
    if (code_value[index] == -1)
      return(index);
    if (prefix_code[index] == hash_prefix &&
        append_character[index] == hash_character)
      return(index);
    index -= offset;
    if (index < 0)
      index += TABLE_SIZE;
  }
}

void expand(std::istream &input,std::ostream &output)
{
  unsigned int next_code;
  unsigned int new_code;
  unsigned int old_code;
  int character;
  int counter;
  unsigned char *string;
  std::ofstream input_code_file;
  next_code=256;           
  counter=0;              
  std::cout << "Expanding...\n";
  old_code=input_code(input);  
  character=old_code;  
  output.put((char) old_code);      
  input_code_file.open("inputcodes", std::ios::out);
  input_code_file << old_code << "\n";
  while ((new_code=input_code(input)) != (MAX_VALUE))
  {
	 input_code_file << new_code << "\n";
    if (++counter==1000)   
    {                    
      counter=0;           
      std::cout << "*";
    }
    if (new_code>=next_code)
    {
      *decode_stack=character;
      string=decode_string(decode_stack+1,old_code);
    }
    else
      string=decode_string(decode_stack,new_code);
    character=*string;
    while (string >= decode_stack)
    {
      output.put(*string--);
    }
    if (next_code <= MAX_CODE)
    {
      prefix_code[next_code]=old_code;
      append_character[next_code]=character;
      next_code++;
    }
    old_code=new_code;
  }
  std::cout << "\n";
}

unsigned char *decode_string(unsigned char *buffer,unsigned int code)
{
  int i=0;
  while (code > 255)
  {
    *buffer++ = append_character[code];
    code=prefix_code[code];
    if (i++>=4094)
    {
      std::cout << "Fatal error during code expansion.\n";
      exit(0);
    }
  }
  *buffer=code;
  return(buffer);
}

int input_code(std::istream &input)
{
  unsigned int return_value;
  static int input_bit_count=0;
  static unsigned long input_bit_buffer=0L;
  while (input_bit_count <= 24)
  {
    input_bit_buffer |= 
        (unsigned long) input.get() << (24-input_bit_count);
    input_bit_count += 8;
  }
  return_value=input_bit_buffer >> (32-BITS);
  input_bit_buffer <<= BITS;
  input_bit_count -= BITS;
  return(return_value);
}

void output_code(std::ostream &output,unsigned int code)
{
  static int output_bit_count=0;
  static unsigned long output_bit_buffer=0L;
  output_bit_buffer |= (unsigned long) code << (32-BITS-output_bit_count);
  output_bit_count += BITS;
  while (output_bit_count >= 8)
  {
    output.put((char) (output_bit_buffer >> 24));
    output_bit_buffer <<= 8;
    output_bit_count -= 8;
  }
}
