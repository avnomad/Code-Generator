#ifndef CODE_GENERATORS_H
#define CODE_GENERATORS_H

#include <stdio.h>

#pragma warning(disable : 4996)



void generateUnaryFunctions(FILE *fileToWrite, const char *outerFunctionName, const char *innerFunctionName);
void generateUnaryOperators(FILE *fileToWrite, const char *outerOperatorName, const char *innerOperatorName);
void generateBinaryOperators(FILE *fileToWrite, const char *outerOperatorName, const char *innerOperatorName, char **alternativeCode);
void generateBinaryFunctions(FILE *fileToWrite, const char *outerFunctionName, const char *innerFunctionName, char **alternativeCode);
void generateReduceBasedFunctions(FILE *fileToWrite, const char *outerFunctionName, const char *innerFunctionName, char **alternativeCode, char *mode , unsigned int countStart);

bool function(const char *);

#endif