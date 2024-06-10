/*************************************
* Tanner Wagner
* Professor Haugh
* CS 241
* Group 3RW
*
* This file is used to store all necessary
* code for convert.c. To use it, you can
* interact with it from the command line
* using ./convert ARGUMENT ARGUMENT ARGUMENT ARGUMENT
* Output will be directly displayed to the 
* terminal.
*
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************
* Function: printUsage
* Description: Prints the usage instructions for the convert program.
* Algorithm: Directly prints a predefined message to standard output.
*************************************/

void printUsage()
{
    printf("usage:\n");
    printf("convert IN OUT SIZE NUMBER\n");
    printf("  IN:\n");
    printf("    -inB  NUMBER is binary.\n");
    printf("    -inD  NUMBER is decimal.\n");
    printf("    -inH  NUMBER is hexadecimal.\n\n");
    printf("  OUT:\n");
    printf("    -outB Output will be in binary.\n");
    printf("    -outD Output will be in decimal.\n");
    printf("    -outH Output will be in hexadecimal.\n\n");
    printf("  SIZE:\n");
    printf("    -8    input is an unsigned 8-bit integer.\n");
    printf("    -16   input is an unsigned 16-bit integer.\n");
    printf("    -32   input is an unsigned 32-bit integer.\n");
    printf("    -64   input is an unsigned 64-bit integer.\n\n");
    printf("  NUMBER:\n");
    printf("    number to be converted.\n\n");
}

/*************************************
* Parameters: const char* format, int isInput
* - format: The format string to be validated, input.
* - isInput: Flag indicating if format is for input or output, input.
* Return value: int (1 for valid format, 0 for invalid).
* Description: Validates the input or output format argument.
* Algorithm: Compares format string against known valid formats.
*************************************/

int validateFormat(const char* format, int isInput)
{
  return !strcmp(format, "-inB") || !strcmp(format, "-inD") || !strcmp(format, "-inH") ||
         !strcmp(format, "-outB") || !strcmp(format, "-outD") || !strcmp(format, "-outH");
}

/*************************************
* Parameters: const char* bitStr, input.
* Description: Converts a size argument to its numeric bit size.
* Return value: int (bit size or 0 for error).
* Algorithm: String comparison to map size argument to bit size.
*************************************/

int getNumBits(const char* bitStr)
{
  if (!strcmp(bitStr, "-8")) return 8;
  if (!strcmp(bitStr, "-16")) return 16;
  if (!strcmp(bitStr, "-32")) return 32;
  if (!strcmp(bitStr, "-64")) return 64;
  
  return 0;
}

/*************************************
* Parameters:
* - const char* numStr: String containing the number to parse, input.
* - const char* format: Format of the input number (binary, decimal, hex), input.
* - int* isValid: Pointer to int to store the result of validation, output.
* Return value: unsigned long (parsed number).
* Description: Parses the input number according to the specified format.
* Algorithm: Uses strtoul with base determined by format, sets isValid based on parsing success.
*************************************/

unsigned long parseNumber(const char* numStr, const char* format, int* isValid)
{
  char *end;
  unsigned long number = strtoul(numStr, &end, (format[3] == 'B') ? 2 : (format[3] == 'D') ? 10 : 16);
  *isValid = (*end == '\0');

  return number;
}

/*************************************
* Parameters:
* - unsigned long number: The number to print in binary, input.
* - int numBits: The number of bits to print, input.
* Description: Prints the given number in binary, padded to numBits.
* Algorithm: Iterates over each bit, printing '0' or '1', with spacing every 4 bits.
*************************************/

void printBinary(unsigned long number, int numBits)
{
  int i;

  for (i = numBits - 1; i >= 0; i--)
  {
    putchar((number & (1UL << i)) ? '1' : '0');
    if (i % 4 == 0 && i != 0)
    {
      putchar(' ');
    }
  }
  putchar('\n');
}

/*************************************
* Parameters:
* - unsigned long number: The number to print in decimal, input.
* - int numBits: The bit size of the input number, input.
* Description: Prints the given number in decimal, right-justified.
* Algorithm: Determines field width based on numBits, prints with padding.
*************************************/

void printDecimal(unsigned long number, int numBits)
{
  char formatted[35] = {0};
  int numDigits = 1;
  unsigned long temp = number;
  int expectedLength;
  int fieldWidth;
  int formattedIndex;
  int digitIndex;

  while (temp >= 10)
  {
    temp /= 10;
    numDigits++;
  }

  expectedLength = numDigits + (numDigits - 1) / 3;

  switch (numBits)
  {
    case 8: fieldWidth = 4; break;
    case 16: fieldWidth = 6; break;
    case 32: fieldWidth = 14; break;
    case 64: fieldWidth = 27; break;
    default: fieldWidth = 14; break;
  }

  formattedIndex = sizeof(formatted) - 2;
  digitIndex = 0;

  do
  {
    if (digitIndex > 0 && digitIndex % 3 == 0)
    {
      formatted[formattedIndex--] = ',';
    }
    formatted[formattedIndex--] = '0' + (number % 10);
    number /= 10;
    digitIndex++;
  } while (number > 0 || formattedIndex >= sizeof(formatted) - expectedLength - 1);

  printf("%*s\n", fieldWidth, &formatted[formattedIndex + 1]);
}

/*************************************
* Parameters:
* - unsigned long number: The number to print in hexadecimal, input.
* - int numBits: The number of bits the output should consider, input.
* Description: Prints the given number in hexadecimal format, adjusted for leading zeros and spacing.
* Algorithm: Calculates the required padding and formats the output in lowercase with spaces.
*************************************/

void printHexadecimal(unsigned long number, int numBits)
{
  int numNibbles = numBits / 4;
  int i;

  for (i = numNibbles - 1; i >= 0; i--)
  {
    printf("%c", "0123456789abcdef"[(number >> (i * 4)) & 0xF]);
    if (i % 2 == 0 && i != 0)
    {
      printf(" ");
    }
  }
  printf("\n");
}

int main (int argc, char* argv[])
{
  char *inFormat, *outFormat;
  int numBits, isValidNumber;
  unsigned long number;

  if (argc != 5)
  {
    printf("ERROR: incorrect number of arguments\n");
    printUsage();
    return 1;
  }

  inFormat = argv[1];
  outFormat = argv[2];

  if (!validateFormat(inFormat, 1))
  {
    printf("ERROR: argument 1 must be -inB | -inD | -inH\n");
    printUsage();
    return 1;
  }

  if (!validateFormat(outFormat, 0))
  {
    printf("ERROR: argument 2 must be -outB | -outD | -outH\n");
    printUsage();
    return 1;
  }

  numBits = getNumBits(argv[3]);

  if (numBits == 0)
  {
    printf("ERROR: argument 3 must be: -8 | -16 | -32 | -64\n");
    printUsage();
    return 1;
  }

  number = parseNumber(argv[4], inFormat, &isValidNumber);

  if (!isValidNumber)
  {
    if (inFormat[3] == 'B') printf("ERROR: argument 4 is not a binary integer\n");
    else if (inFormat[3] == 'D') printf("ERROR: argument 4 is not a decimal integer\n");
    else if (inFormat[3] == 'H') printf("ERROR: argument 4 is not a hexadecimal integer\n");
    printUsage();
    return 1;
  }

  number &= ((1UL << numBits) - 1);

  if (strcmp(outFormat, "-outB") == 0) printBinary(number, numBits);
  else if (strcmp(outFormat, "-outD") == 0) printDecimal(number, numBits);
  else if (strcmp(outFormat, "-outH") == 0) printHexadecimal(number, numBits);
  else
  {
    printf("ERROR: Unknown output format.\n");
    printUsage();
  }

  return 0;
}
