#include <iostream>
#include <fstream>
#include <stdlib.h>

#define BITS 12                   /* Setting the number of bits to 12, 13*/
#define HASHING_SHIFT BITS-8      /* or 14 affects several constants.    */
#define MAX_VALUE (1 << BITS) - 1 /* Note that MS-DOS machines need to   */
#define MAX_CODE MAX_VALUE - 1    /* compile their code in large model if*/
                                  /* 14 bits are selected.               */
#if BITS == 14
  #define TABLE_SIZE 18041        /* The string table size needs to be a */
#endif                            /* prime number that is somewhat larger*/
#if BITS == 13                    /* than 2**BITS.                       */
  #define TABLE_SIZE 9029
#endif
#if BITS <= 12
  #define TABLE_SIZE 5021
#endif


int *code_value;                  /* This is the code value array        */
unsigned int *prefix_code;        /* This array holds the prefix codes   */
unsigned char *append_character;  /* This array holds the appended chars */
unsigned char decode_stack[4000]; /* This array holds the decoded string */

void compress(ifstream &,ofstream &);
int find_match(int, unsigned int);
void expand(istream &, ostream &);
unsigned char *decode_string(unsigned char *, unsigned int);
int input_code(istream &);
void output_code(ostream &,unsigned int);

int main(int argc, char *argv[])
{
ifstream input_file;
ifstream lzw_file_i;
ofstream output_file;
ofstream lzw_file_o;
char input_file_name[81];

code_value=new int[TABLE_SIZE*sizeof(unsigned int)];
prefix_code=new unsigned int[TABLE_SIZE*sizeof(unsigned int)];
append_character=new unsigned char[TABLE_SIZE*sizeof(unsigned char)];


if (code_value==NULL || prefix_code==NULL || append_character==NULL)
  {
    cout << "Fatal error allocating table space!\n";
    exit(0);
  }

if (argc>1)
  {
    strcpy(input_file_name, argv[1]);
  }
else
  {
    cout << "Input file name: ";
    cin >> input_file_name;
  }
input_file.open(input_file_name, ios::binary);
/*input_file=fopen(input_file_name,"rb");*/
lzw_file_o.open("test.lzw", ios::binary);
/*lzw_file=fopen("test.lzw","wb");*/
if (!input_file || !lzw_file_o)
  {
    cout << "Fatal error opening files.\n";
    exit(0);
  };
/*
** Compress the file.
*/
compress(input_file,lzw_file_o);
input_file.close(); /*fclose(input_file);*/
lzw_file_o.close(); /*fclose(lzw_file);*/
delete code_value;

lzw_file_i.open("test.lzw", ios::binary);
/*lzw_file=fopen("test.lzw","rb");*/
output_file.open("test.out", ios::binary);
/*output_file=fopen("test.out","wb");*/
if (!lzw_file_i  || !output_file)
  {
    cout << "Fatal error opening files.\n";
    exit(0);
  };

expand(lzw_file_i, output_file);
lzw_file_i.close();
output_file.close();

delete prefix_code;
delete append_character;
}


void compress(ifstream &input,ofstream &output)
{
unsigned int next_code;
unsigned int character;
unsigned int string_code;
unsigned int index;
int i;
ofstream output_code_file;

	output_code_file.open("outputcodes", ios::out);

  next_code=256;              /* Next code is the next available string code*/
  for (i=0;i<TABLE_SIZE;i++)  /* Clear out the string table before starting */
    code_value[i]=-1;

  i=0;
  cout << "Compressing...\n";
  string_code=input.get();    /* Get the first code                         */

  while ((character=input.get()) != (unsigned)EOF)
  {
    if (++i==1000)                         /* Print a * every 1000    */
    {                                      /* input characters.  This */
      i=0;                                 /* is just a pacifier.     */
      cout << "*";
    }
    index=find_match(string_code,character);/* See if the string is in */
    if (code_value[index] != -1)            /* the table.  If it is,   */
      string_code=code_value[index];        /* get the code value.  If */
    else                                    /* the string is not in the*/
    {                                       /* table, try to add it.   */
      if (next_code <= MAX_CODE)
      {
        code_value[index]=next_code++;
        prefix_code[index]=string_code;
        append_character[index]=character;
      }

      output_code_file << string_code << "\n";

      output_code(output,string_code);  /* When a string is found  */
      string_code=character;            /* that is not in the table*/
    }                                   /* I output the last string*/
  }                                     /* after adding the new one*/

  output_code(output,string_code); /* Output the last code               */

      output_code_file << string_code << "\n";

  output_code(output,MAX_VALUE);   /* Output the end of buffer code      */
  output_code(output,0);           /* This code flushes the output buffer*/
  cout << "\n";
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


void expand(istream &input,ostream &output)
{
unsigned int next_code;
unsigned int new_code;
unsigned int old_code;
int character;
int counter;
unsigned char *string;
ofstream input_code_file;

  next_code=256;           /* This is the next available code to define */
  counter=0;               /* Counter is used as a pacifier.            */
  cout << "Expanding...\n";

  old_code=input_code(input);  /* Read in the first code, initialize the */
  character=old_code;          /* character variable, and send the first */
  /*cout << "Old code: " << old_code << "\n";
  cout << "Old code (char): " << ((char) old_code) << "\n"; */

  output.put((char) old_code);       /* code to the output file                */

  input_code_file.open("inputcodes", ios::out);

  input_code_file << old_code << "\n";

  while ((new_code=input_code(input)) != (MAX_VALUE))
  {
	 input_code_file << new_code << "\n";
    if (++counter==1000)   /* This section of code prints out     */
    {                      /* an asterisk every 1000 characters   */
      counter=0;           /* It is just a pacifier.              */
      cout << "*";
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
     /* cout << "String: " << *string << "\n"; */

      output.put(*string--);
      }
      /*
** Finally, if possible, add a new code to the string table.
*/
    if (next_code <= MAX_CODE)
    {
      prefix_code[next_code]=old_code;
      append_character[next_code]=character;
      next_code++;
    }
    old_code=new_code;
  }
  cout << "\n";
}


unsigned char *decode_string(unsigned char *buffer,unsigned int code)
{
int i;

  i=0;
  while (code > 255)
  {
    *buffer++ = append_character[code];
    code=prefix_code[code];
    if (i++>=4094)
    {
      cout << "Fatal error during code expansion.\n";
      exit(0);
    }
  }
  *buffer=code;
  return(buffer);
}


int input_code(istream &input)
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

void output_code(ostream &output,unsigned int code)
{
static int output_bit_count=0;
static unsigned long output_bit_buffer=0L;

  output_bit_buffer |= (unsigned long) code << (32-BITS-output_bit_count);
  output_bit_count += BITS;
  while (output_bit_count >= 8)
  {
	/* cout << "Output bit buffer: " << output_bit_buffer << "\n";
  	cout << "Output bit buffer >> 24: " << (output_bit_buffer >> 24) << "\n";
	cout << "Output bit buffer >> 24 (char): " << (output_bit_buffer >> 24) << "\n";
    */
  	output.put((char) (output_bit_buffer >> 24));
   output_bit_buffer <<= 8;
   output_bit_count -= 8;
  }
}
