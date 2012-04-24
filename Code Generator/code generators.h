//	Copyright (C) 2008, 2012 Vaptistis Anogeianakis <el05208@mail.ntua.gr>
/*
 *	This file is part of Code Generator.
 *
 *	Code Generator is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Code Generator is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Code Generator.  If not, see <http://www.gnu.org/licenses/>.
 */

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