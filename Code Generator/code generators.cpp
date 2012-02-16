#include "code generators.h"

#include <ctype.h>

bool function(const char *s)	// todo: alter the code to handle more complex cases.
{
	if(isalpha(*s) || *s == '_' || *s == ':')
		return true;
	else
		return false;
}
	
void generateUnaryFunctions(FILE *fileToWrite, const char *outerFunctionName, const char *innerFunctionName)
{
	FILE *headerFile;

	// write in headers.
	if(headerFile = fopen("generated code/header parts/TempVector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector %s(Vector);\t\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(TempVector);\t\t\t\t\t\t\t\t//\t>>\n",outerFunctionName);		
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	if(headerFile = fopen("generated code/header parts/Vector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector %s(Vector);\t\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	if(headerFile = fopen("generated code/header parts/TempMatrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix);\t\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(TempMatrix);\t\t\t\t\t\t\t\t//\t>>\n",outerFunctionName);		
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	if(headerFile = fopen("generated code/header parts/Matrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix);\t\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	// start the file.
	fprintf(fileToWrite,"#include \"../Real Matrices and Vectors.h\"\nusing namespace ASL;\n\n\n");

	// pragma
	fprintf(fileToWrite,"#pragma region Vector\n\n");

	// TempVector argument.
	fprintf(fileToWrite,"TempVector ASL::%s(TempVector v)\n{\n",outerFunctionName);
	fprintf(fileToWrite,"\tunsigned int c;\n\tanticast p;\n\n\tp.v = v.data.v;\n\tc = *p.ui++;\n");
	fprintf(fileToWrite,"\twhile(c--)\n\t\t*p.d = %s(*p.d),++p.d;\n\n\treturn v;\n",innerFunctionName);
	fprintf(fileToWrite,"} //end of fuction %s\n\n\n",outerFunctionName);

	// Vector argument.
	fprintf(fileToWrite,"TempVector ASL::%s(Vector v)\n{\n",outerFunctionName);
	fprintf(fileToWrite,"\tunsigned int c;\n\tanticast p;\n\tTempVector u;\n\n\tc = *v.data.ui++;\n");
	fprintf(fileToWrite,"\tp.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;\n");
	fprintf(fileToWrite,"\twhile(c--)\n\t\t*p.d++ = %s(*v.data.d++);\n\n\tv.data.v = 0;\n",innerFunctionName);
	fprintf(fileToWrite,"\treturn u;\n} //end of fuction %s\n\n",outerFunctionName);

	// pragma
	fprintf(fileToWrite,"#pragma endregion\n#pragma region Matrix\n\n");

	// TempMatrix argument.
	fprintf(fileToWrite,"TempMatrix ASL::%s(TempMatrix A)\n{\t/* we will recycle the memory used by A. */\n",outerFunctionName);
	fprintf(fileToWrite,"\tregister unsigned int c;\n\tregister anticast p;\n\n\tp.v = A.data.v;\n\tc = *p.ui++;\n");
	fprintf(fileToWrite,"\t++p.ui;\t// skip the # of rows.\n\twhile(c--)\n\t\t*p.d = %s(*p.d),++p.d;\n\n",innerFunctionName);
	fprintf(fileToWrite,"\treturn A;\n} //end of fuction %s\n\n\n",outerFunctionName);

	// Matrix argument.
	fprintf(fileToWrite,"TempMatrix ASL::%s(Matrix A)\n{\n\tregister unsigned int c;\n\tregister anticast p;\n",outerFunctionName);
	fprintf(fileToWrite,"\tTempMatrix B;\n\n\tc = *A.data.ui++;\t// set c to the # of elements in A.\n");
	fprintf(fileToWrite,"\tp.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;");
	fprintf(fileToWrite,"\t// store the # of elements of A in B.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in B.\n");
	fprintf(fileToWrite,"\twhile(c--)\n\t\t*p.d++ = %s(*A.data.d++);\n\n\tA.data.v = 0;\n",innerFunctionName);
	fprintf(fileToWrite,"\treturn B;\n} //end of fuction %s\n\n",outerFunctionName);
	
	// pragma
	fprintf(fileToWrite,"#pragma endregion\n");

} // end function generateUnaryFunctions



void generateUnaryOperators(FILE *fileToWrite, const char *outerOperatorName, const char *innerOperatorName)
{
	FILE *headerFile;

	// write in headers.
	if(headerFile = fopen("generated code/header parts/TempVector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector operator %s (Vector);\t\t\t\t\t\t\t// operator %s",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"%s\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");
		fprintf(headerFile,"\tfriend TempVector operator %s (TempVector);\t\t\t\t\t\t//\t>>\n",outerOperatorName);		
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	if(headerFile = fopen("generated code/header parts/Vector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector operator %s (Vector);\t\t\t\t\t\t\t// operator %s",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"%s\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	if(headerFile = fopen("generated code/header parts/TempMatrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix);\t\t\t\t\t\t\t// operator %s",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"%s\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");
		fprintf(headerFile,"\tfriend TempMatrix operator %s (TempMatrix);\t\t\t\t\t\t//\t>>\n",outerOperatorName);		
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	if(headerFile = fopen("generated code/header parts/Matrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix);\t\t\t\t\t\t\t// operator %s",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"%s\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");

	//start the file.
	fprintf(fileToWrite,"#include \"../Real Matrices and Vectors.h\"\nusing namespace ASL;\n\n\n");

	// pragma
	fprintf(fileToWrite,"#pragma region Vector\n\n");

	//TempVector argument.
	if(*innerOperatorName)
	{
		fprintf(fileToWrite,"TempVector ASL::operator %s (TempVector v)\n{\n",outerOperatorName);
		fprintf(fileToWrite,"\tunsigned int c;\n\tanticast p;\n\n\tp.v = v.data.v;\n\tc = *p.ui++;\n");
		fprintf(fileToWrite,"\twhile(c--)\n\t\t*p.d = %s*p.d,++p.d;\n\n\treturn v;\n",innerOperatorName);
		fprintf(fileToWrite,"} //end of fuction operator %s",outerOperatorName);
	}
	else
	{
		fprintf(fileToWrite,"TempVector ASL::operator %s (TempVector v)\n{\n\treturn v;\n} //end of fuction operator %s",outerOperatorName,outerOperatorName);
	}
	fprintf(fileToWrite,"%s\n\n\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");

	//Vector argument.
	fprintf(fileToWrite,"TempVector ASL::operator %s (Vector v)\n{\n",outerOperatorName);
	fprintf(fileToWrite,"\tunsigned int c;\n\tanticast p;\n\tTempVector u;\n\n\tc = *v.data.ui++;\n");
	fprintf(fileToWrite,"\tp.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;\n");
	fprintf(fileToWrite,"\twhile(c--)\n\t\t*p.d++ = %s*v.data.d++;\n\n\tv.data.v = 0;\n",innerOperatorName);
	fprintf(fileToWrite,"\treturn u;\n} //end of fuction operator %s",outerOperatorName);
	fprintf(fileToWrite,"%s\n\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");

	// pragma
	fprintf(fileToWrite,"#pragma endregion\n#pragma region Matrix\n\n");

	// TempMatrix argument.
	if(*innerOperatorName)
	{
		fprintf(fileToWrite,"TempMatrix ASL::operator %s (TempMatrix A)\n{\t/* we will recycle the memory used by A. */\n",outerOperatorName);
		fprintf(fileToWrite,"\tregister unsigned int c;\n\tregister anticast p;\n\n\tp.v = A.data.v;\n\tc = *p.ui++;\n");
		fprintf(fileToWrite,"\t++p.ui;\t// skip the # of rows.\n\twhile(c--)\n\t\t*p.d = %s*p.d,++p.d;\n\n",innerOperatorName);
		fprintf(fileToWrite,"\treturn A;\n} //end of fuction operator %s",outerOperatorName);
	}
	else
	{
		fprintf(fileToWrite,"TempMatrix ASL::operator %s (TempMatrix A)\n{\n\treturn A;\n} //end of fuction operator %s",outerOperatorName,outerOperatorName);
	}
	fprintf(fileToWrite,"%s\n\n\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");

	// Matrix argument.
	fprintf(fileToWrite,"TempMatrix ASL::operator %s (Matrix A)\n{\n\tregister unsigned int c;\n\tregister anticast p;\n",outerOperatorName);
	fprintf(fileToWrite,"\tTempMatrix B;\n\n\tc = *A.data.ui++;\t// set c to the # of elements in A.\n");
	fprintf(fileToWrite,"\tp.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;");
	fprintf(fileToWrite,"\t// store the # of elements of A in B.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in B.\n");
	fprintf(fileToWrite,"\twhile(c--)\n\t\t*p.d++ = %s*A.data.d++;\n\n\tA.data.v = 0;\n",innerOperatorName);
	fprintf(fileToWrite,"\treturn B;\n} //end of fuction operator %s",outerOperatorName);
	fprintf(fileToWrite,"%s\n\n",*outerOperatorName == '-'||*outerOperatorName == '+'||*outerOperatorName == '*'||*outerOperatorName == '&'?" (unary)":"");

	// pragma
	fprintf(fileToWrite,"#pragma endregion\n");

} // end function generateUnaryOperators



void generateBinaryOperators(FILE *fileToWrite, const char *outerOperatorName, const char *innerOperatorName, char **alternativeCode)
{
	FILE *headerFile;

	unsigned int c;
	char temp[10];
	char *definitions[] = {
		"#pragma region Vector & Vector\n\n// vectors v and u are assumed to be of the same size.\nTempVector ASL::operator %s (Vector v, Vector u)\t// overloaded version %s\n{\n\tTempVector w;\n\tunsigned int c;\n\tanticast p;\n\n\tc = *v.data.ui++;\n\t++u.data.ui;\n\tp.v = w.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;\n\twhile(c--)\n\t\t*p.d++ = *v.data.d++ %s *u.data.d , ++u.data.d;\n\n\tv.data.v = u.data.v = 0;\n\treturn w;\n} // end function operator %s overloaded version %s\n\n\n",
		"// vectors v and u are assumed to be of the same size.\nTempVector ASL::operator %s (TempVector v, Vector u)\t// overloaded version %s\n{\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\n\tc = *p.ui++;\n\t++u.data.ui;\n\n\twhile(c--)\n\t\t*p.d = *p.d %s *u.data.d , ++p.d , ++u.data.d;\n\n\tu.data.v = 0;\n\treturn v;\n} // end function operator %s overloaded version %s\n\n\n",
		"// vectors v and u are assumed to be of the same size.\nTempVector ASL::operator %s (Vector u, TempVector v)\t// overloaded version %s\n{\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\n\tc = *p.ui++;\n\t++u.data.ui;\n\n\twhile(c--)\n\t\t*p.d = *u.data.d++ %s *p.d , ++p.d;\n\n\tu.data.v = 0;\n\treturn v;\n} // end function operator %s overloaded version %s\n\n\n",
		"// vectors v and u are assumed to be of the same size.\nTempVector ASL::operator %s (TempVector v, TempVector u)\t// overloaded version %s\n{\n\tunsigned int c;\n\tanticast p,q;\n\n\tp = v.data;\n\tq = u.data;\n\tc = *p.ui++;\n\t++q.ui;\n\n\twhile(c--)\n\t\t*p.d = *p.d %s *q.d , ++p.d , ++q.d;\n\n\tfree(u.data.v);\n\treturn v;\n} // end function operator %s overloaded version %s\n\n#pragma endregion\n#pragma region Vector & Scalar\n\n",
		"TempVector ASL::operator %s (double x, Vector v)\t// overloaded version %s\n{\n\tunsigned int c;\n\tTempVector u;\n\tanticast p;\n\n\tc = *v.data.ui++;\t// get the size of v.\n\tp.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempVector of that size.\n\t*p.ui++ = c;\t\t// save the size of the new TempVector.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d++ = x %s *v.data.d , ++v.data.d;\t\n\t\n\tv.data.v = 0;\t// prevents the destractor from deallocating v\'s memory.\n\treturn u;\n} // end function operator %s overloaded version %s\n\n\n",
		"TempVector ASL::operator %s (Vector v, double x)\t// overloaded version %s\n{\n\tunsigned int c;\n\tTempVector u;\n\tanticast p;\n\n\tc = *v.data.ui++;\t// get the size of v.\n\tp.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempVector of that size.\n\t*p.ui++ = c;\t\t// save the size of the new TempVector.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d++ = *v.data.d++ %s x;\t\n\t\n\tv.data.v = 0;\t// prevents the destractor from deallocating v\'s memory.\n\treturn u;\n} // end function operator %s overloaded version %s\n\n\n",
		"TempVector ASL::operator %s (double x, TempVector v)\t// overloaded version %s\n{\t/* we will recycle the memory used by v. */\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\t\t// set p to point at the begining of v.\n\tc = *p.ui++;\t// get the size of v.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d = x %s *p.d , ++p.d;\n\n\treturn v;\n} // end function operator %s overloaded version %s\n\n\n",
		"TempVector ASL::operator %s (TempVector v, double x)\t// overloaded version %s\n{\t/* we will recycle the memory used by v. */\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\t\t// set p to point at the begining of v.\n\tc = *p.ui++;\t// get the size of v.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d = *p.d %s x , ++p.d;\n\n\treturn v;\n} // end function operator %s overloaded version %s\n\n#pragma endregion\n#pragma region Matrix & Matrix\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::operator %s (Matrix A, Matrix B)\t// overloaded version %s\n{\n\tregister unsigned int c;\n\tregister anticast p;\n\tTempMatrix C;\n\n\tc = *A.data.ui++;\t// set c to the # of elements of A.\n\t++B.data.ui;\t// skip the # of elements of B.\n\tp.v = C.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;\t// store the # of elements of A in C.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in C.\n\t++B.data.ui;\t// skip the # of rows of B.\n\n\twhile(c--)\n\t\t*p.d++ = *A.data.d++ %s *B.data.d , ++B.data.d;\n\n\tA.data.v = B.data.v = 0;\t// prevent the destractor from deallocating A\'s and B\'s memory.\n\treturn C;\n} // end function operator %s overloaded version %s\n\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::operator %s (TempMatrix A, Matrix B)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++B.data.ui;\t// skip the # of elements of B.\n\t++p.ui , ++B.data.ui;\t// skip the # of rows of A and B.\n\n\twhile(c--)\n\t\t*p.d = *p.d %s *B.data.d , ++p.d , ++B.data.d;\n\n\tB.data.v = 0;\t// prevent the destractor from deallocating B\'s memory.\n\treturn A;\n} // end function operator %s overloaded version %s\n\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::operator %s (Matrix B, TempMatrix A)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++B.data.ui;\t// skip the # of elements of B.\n\t++p.ui , ++B.data.ui;\t// skip the # of rows of A and B.\n\n\twhile(c--)\n\t\t*p.d = *B.data.d++ %s *p.d , ++p.d;\n\n\tB.data.v = 0;\n\treturn A;\n} // end function operator %s overloaded version %s\n\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::operator %s (TempMatrix A, TempMatrix B)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p,q;\n\n\tp = A.data;\t\t// set p at the begining of A.\n\tq = B.data;\t\t// set q at the begining of B.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++q.ui;\t// skip the # of elements of B.\n\t++p.ui , ++q.ui;\t// skip the # of rows of A and B.\n\n\twhile(c--)\n\t\t*p.d = *p.d %s *q.d , ++p.d , ++q.d;\n\n\tfree(B.data.v);\t// free the memory occupied by the temporary matric B.\n\treturn A;\n} // end function operator %s overloaded version %s\n\n#pragma endregion\n#pragma region Matrix & Scalar\n\n",
		"TempMatrix ASL::operator %s (double x, Matrix A)\t// overloaded version %s\n{\n\tregister unsigned int c;\n\tregister anticast p;\n\tTempMatrix B;\n\n\tc = *A.data.ui++;\t// set c to the # of elements of A.\n\tp.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempMatrix of that many elements.\n\t*p.ui++ = c;\t// store the # of elements of A in B.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in B.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d++ = x %s *A.data.d , ++A.data.d;\t\n\t\n\tA.data.v = 0;\t// prevents the destractor from deallocating A\'s memory.\n\treturn B;\n} // end function operator %s overloaded version %s\n\n\n",
		"TempMatrix ASL::operator %s (Matrix A, double x)\t// overloaded version %s\n{\n\tregister unsigned int c;\n\tTempMatrix B;\n\tregister anticast p;\n\n\tc = *A.data.ui++;\t// set c to the # of elements of A.\n\tp.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempMatrix of that many elements.\n\t*p.ui++ = c;\t// store the # of elements of A in B.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in B.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d++ = *A.data.d++ %s x;\t\n\t\n\tA.data.v = 0;\t// prevents the destractor from deallocating A\'s memory.\n\treturn B;\n} // end function operator %s overloaded version %s\n\n\n",
		"TempMatrix ASL::operator %s (double x, TempMatrix A)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p to point at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++p.ui;\t// skip the # of rows of A.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d = x %s *p.d , ++p.d;\n\n\treturn A;\n} // end function operator %s overloaded version %s\n\n\n",
		"TempMatrix ASL::operator %s (TempMatrix A, double x)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p to point at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++p.ui;\t// skip the # of rows of A.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d = *p.d %s x , ++p.d;\n\n\treturn A;\n} // end function operator %s overloaded version %s\n\n#pragma endregion\n"
	,0};
	char **currentDefinition;


	//write the prototypes for the functions - TempVector.
	if(headerFile = fopen("generated code/header parts/TempVector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector operator %s (Vector,Vector);\t\t\t\t\t// operator %s\n",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (TempVector,Vector);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (Vector,TempVector);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (TempVector,TempVector);\t\t\t//\t>>\n",outerOperatorName);

		fprintf(headerFile,"\tfriend TempVector operator %s (Vector,double);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (double,Vector);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (TempVector,double);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (double,TempVector);\t\t\t\t//\t>>\n",outerOperatorName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	//write the prototypes for the functions - Vector.
	if(headerFile = fopen("generated code/header parts/Vector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector operator %s (Vector,Vector);\t\t\t\t\t// operator %s\n",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (TempVector,Vector);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (Vector,TempVector);\t\t\t\t//\t>>\n",outerOperatorName);

		fprintf(headerFile,"\tfriend TempVector operator %s (Vector,double);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempVector operator %s (double,Vector);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	//write the prototypes for the functions - TempMatrix.
	if(headerFile = fopen("generated code/header parts/TempMatrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix,Matrix);\t\t\t\t\t// operator %s\n",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (TempMatrix,Matrix);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix,TempMatrix);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (TempMatrix,TempMatrix);\t\t\t//\t>>\n",outerOperatorName);

		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix,double);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (double,Matrix);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (TempMatrix,double);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (double,TempMatrix);\t\t\t\t//\t>>\n",outerOperatorName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	//write the prototypes for the functions - Matrix.
	if(headerFile = fopen("generated code/header parts/Matrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix,Matrix);\t\t\t\t\t// operator %s\n",outerOperatorName,outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (TempMatrix,Matrix);\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix,TempMatrix);\t\t\t\t//\t>>\n",outerOperatorName);

		fprintf(headerFile,"\tfriend TempMatrix operator %s (Matrix,double);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fprintf(headerFile,"\tfriend TempMatrix operator %s (double,Matrix);\t\t\t\t\t//\t>>\n",outerOperatorName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	//write some initial code in the file.
	fprintf(fileToWrite,"#include \"../Real Matrices and Vectors.h\"\nusing namespace ASL;\n\n\n");

	//enter loop to generate the rest of the code.
	c = 1;	
	currentDefinition = definitions;
	while(*currentDefinition)
	{
		if(*alternativeCode)
			fprintf(fileToWrite,*alternativeCode);
		else
		{
			sprintf(temp,"%u",c);
			fprintf(fileToWrite,*currentDefinition,outerOperatorName,temp,innerOperatorName,outerOperatorName,temp);
		}
		++c , ++currentDefinition , ++alternativeCode;
	} // end while

} // end function generateBinaryOperators



void generateBinaryFunctions(FILE *fileToWrite, const char *outerFunctionName, const char *innerFunctionName, char **alternativeCode)
{
	FILE *headerFile;

	unsigned int c;
	char temp[10];
	char *definitions[] = {
		"#pragma region Vector & Vector\n\n// vectors v and u are assumed to be of the same size.\nTempVector ASL::%s(Vector v, Vector u)\t// overloaded version %s\n{\n\tTempVector w;\n\tunsigned int c;\n\tanticast p;\n\n\tc = *v.data.ui++;\n\t++u.data.ui;\n\tp.v = w.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;\n\twhile(c--)\n\t\t*p.d++ = %s(*v.data.d++ , *u.data.d) , ++u.data.d;\n\n\tv.data.v = u.data.v = 0;\n\treturn w;\n} // end function %s overloaded version %s\n\n\n",
		"// vectors v and u are assumed to be of the same size.\nTempVector ASL::%s(TempVector v, Vector u)\t// overloaded version %s\n{\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\n\tc = *p.ui++;\n\t++u.data.ui;\n\n\twhile(c--)\n\t\t*p.d = %s(*p.d , *u.data.d) , ++p.d , ++u.data.d;\n\n\tu.data.v = 0;\n\treturn v;\n} // end function %s overloaded version %s\n\n\n",
		"// vectors v and u are assumed to be of the same size.\nTempVector ASL::%s(Vector u, TempVector v)\t// overloaded version %s\n{\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\n\tc = *p.ui++;\n\t++u.data.ui;\n\n\twhile(c--)\n\t\t*p.d = %s(*u.data.d++ , *p.d) , ++p.d;\n\n\tu.data.v = 0;\n\treturn v;\n} // end function %s overloaded version %s\n\n\n",
		"// vectors v and u are assumed to be of the same size.\nTempVector ASL::%s(TempVector v, TempVector u)\t// overloaded version %s\n{\n\tunsigned int c;\n\tanticast p,q;\n\n\tp = v.data;\n\tq = u.data;\n\tc = *p.ui++;\n\t++q.ui;\n\n\twhile(c--)\n\t\t*p.d = %s(*p.d , *q.d) , ++p.d , ++q.d;\n\n\tfree(u.data.v);\n\treturn v;\n} // end function %s overloaded version %s\n\n#pragma endregion\n#pragma region Vector & Scalar\n\n",
		"TempVector ASL::%s(double x, Vector v)\t// overloaded version %s\n{\n\tunsigned int c;\n\tTempVector u;\n\tanticast p;\n\n\tc = *v.data.ui++;\t// get the size of v.\n\tp.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempVector of that size.\n\t*p.ui++ = c;\t\t// save the size of the new TempVector.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d++ = %s(x , *v.data.d) , ++v.data.d;\t\n\t\n\tv.data.v = 0;\t// prevents the destractor from deallocating v\'s memory.\n\treturn u;\n} // end function %s overloaded version %s\n\n\n",
		"TempVector ASL::%s(Vector v, double x)\t// overloaded version %s\n{\n\tunsigned int c;\n\tTempVector u;\n\tanticast p;\n\n\tc = *v.data.ui++;\t// get the size of v.\n\tp.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempVector of that size.\n\t*p.ui++ = c;\t\t// save the size of the new TempVector.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d++ = %s(*v.data.d++ , x);\t\n\t\n\tv.data.v = 0;\t// prevents the destractor from deallocating v\'s memory.\n\treturn u;\n} // end function %s overloaded version %s\n\n\n",
		"TempVector ASL::%s(double x, TempVector v)\t// overloaded version %s\n{\t/* we will recycle the memory used by v. */\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\t\t// set p to point at the begining of v.\n\tc = *p.ui++;\t// get the size of v.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d = %s(x , *p.d) , ++p.d;\n\n\treturn v;\n} // end function %s overloaded version %s\n\n\n",
		"TempVector ASL::%s(TempVector v, double x)\t// overloaded version %s\n{\t/* we will recycle the memory used by v. */\n\tunsigned int c;\n\tanticast p;\n\n\tp = v.data;\t\t// set p to point at the begining of v.\n\tc = *p.ui++;\t// get the size of v.\n\n\twhile(c--)\t\t// for each element of v.\n\t\t*p.d = %s(*p.d , x) , ++p.d;\n\n\treturn v;\n} // end function %s overloaded version %s\n\n#pragma endregion\n#pragma region Matrix & Matrix\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::%s(Matrix A, Matrix B)\t// overloaded version %s\n{\n\tregister unsigned int c;\n\tregister anticast p;\n\tTempMatrix C;\n\n\tc = *A.data.ui++;\t// set c to the # of elements of A.\n\t++B.data.ui;\t// skip the # of elements of B.\n\tp.v = C.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\n\t*p.ui++ = c;\t// store the # of elements of A in C.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in C.\n\t++B.data.ui;\t// skip the # of rows of B.\n\n\twhile(c--)\n\t\t*p.d++ = %s(*A.data.d++ , *B.data.d) , ++B.data.d;\n\n\tA.data.v = B.data.v = 0;\t// prevent the destractor from deallocating A\'s and B\'s memory.\n\treturn C;\n} // end function %s overloaded version %s\n\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::%s(TempMatrix A, Matrix B)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++B.data.ui;\t// skip the # of elements of B.\n\t++p.ui , ++B.data.ui;\t// skip the # of rows of A and B.\n\n\twhile(c--)\n\t\t*p.d = %s(*p.d , *B.data.d) , ++p.d , ++B.data.d;\n\n\tB.data.v = 0;\t// prevent the destractor from deallocating B\'s memory.\n\treturn A;\n} // end function %s overloaded version %s\n\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::%s(Matrix B, TempMatrix A)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++B.data.ui;\t// skip the # of elements of B.\n\t++p.ui , ++B.data.ui;\t// skip the # of rows of A and B.\n\n\twhile(c--)\n\t\t*p.d = %s(*B.data.d++ , *p.d) , ++p.d;\n\n\tB.data.v = 0;\n\treturn A;\n} // end function %s overloaded version %s\n\n\n",
		"// matrices A and B are assumed to be of the same size.\nTempMatrix ASL::%s(TempMatrix A, TempMatrix B)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p,q;\n\n\tp = A.data;\t\t// set p at the begining of A.\n\tq = B.data;\t\t// set q at the begining of B.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++q.ui;\t// skip the # of elements of B.\n\t++p.ui , ++q.ui;\t// skip the # of rows of A and B.\n\n\twhile(c--)\n\t\t*p.d = %s(*p.d , *q.d) , ++p.d , ++q.d;\n\n\tfree(B.data.v);\t// free the memory occupied by the temporary matric B.\n\treturn A;\n} // end function %s overloaded version %s\n\n#pragma endregion\n#pragma region Matrix & Scalar\n\n",
		"TempMatrix ASL::%s(double x, Matrix A)\t// overloaded version %s\n{\n\tregister unsigned int c;\n\tregister anticast p;\n\tTempMatrix B;\n\n\tc = *A.data.ui++;\t// set c to the # of elements of A.\n\tp.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempMatrix of that many elements.\n\t*p.ui++ = c;\t// store the # of elements of A in B.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in B.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d++ = %s(x , *A.data.d) , ++A.data.d;\t\n\t\n\tA.data.v = 0;\t// prevents the destractor from deallocating A\'s memory.\n\treturn B;\n} // end function %s overloaded version %s\n\n\n",
		"TempMatrix ASL::%s(Matrix A, double x)\t// overloaded version %s\n{\n\tregister unsigned int c;\n\tTempMatrix B;\n\tregister anticast p;\n\n\tc = *A.data.ui++;\t// set c to the # of elements of A.\n\tp.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));\t// allocate memory for a new TempMatrix of that many elements.\n\t*p.ui++ = c;\t// store the # of elements of A in B.\n\t*p.ui++ = *A.data.ui++;\t// store the # of rows of A in B.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d++ = %s(*A.data.d++ , x);\t\n\t\n\tA.data.v = 0;\t// prevents the destractor from deallocating A\'s memory.\n\treturn B;\n} // end function %s overloaded version %s\n\n\n",
		"TempMatrix ASL::%s(double x, TempMatrix A)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p to point at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++p.ui;\t// skip the # of rows of A.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d = %s(x , *p.d) , ++p.d;\n\n\treturn A;\n} // end function %s overloaded version %s\n\n\n",
		"TempMatrix ASL::%s(TempMatrix A, double x)\t// overloaded version %s\n{\t/* we will recycle the memory used by A. */\n\tregister unsigned int c;\n\tregister anticast p;\n\n\tp = A.data;\t\t// set p to point at the begining of A.\n\tc = *p.ui++;\t// set c to the # of elements of A.\n\t++p.ui;\t// skip the # of rows of A.\n\n\twhile(c--)\t\t// for each element of A.\n\t\t*p.d = %s(*p.d , x) , ++p.d;\n\n\treturn A;\n} // end function %s overloaded version %s\n\n#pragma endregion\n"
	,0};
	char **currentDefinition;


	// write the prototypes for the functions - TempVector.
	if(headerFile = fopen("generated code/header parts/TempVector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector %s(Vector,Vector);\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(TempVector,Vector);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(Vector,TempVector);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(TempVector,TempVector);\t\t\t//\t>>\n",outerFunctionName);

		fprintf(headerFile,"\tfriend TempVector %s(Vector,double);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(double,Vector);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(TempVector,double);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(double,TempVector);\t\t\t\t//\t>>\n",outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	// write the prototypes for the functions - Vector.
	if(headerFile = fopen("generated code/header parts/Vector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector %s(Vector,Vector);\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(TempVector,Vector);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(Vector,TempVector);\t\t\t\t//\t>>\n",outerFunctionName);

		fprintf(headerFile,"\tfriend TempVector %s(Vector,double);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(double,Vector);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	// write the prototypes for the functions - TempMatrix.
	if(headerFile = fopen("generated code/header parts/TempMatrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix,Matrix);\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(TempMatrix,Matrix);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix,TempMatrix);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(TempMatrix,TempMatrix);\t\t\t//\t>>\n",outerFunctionName);

		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix,double);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(double,Matrix);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(TempMatrix,double);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(double,TempMatrix);\t\t\t\t//\t>>\n",outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	// write the prototypes for the functions - Matrix.
	if(headerFile = fopen("generated code/header parts/Matrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix,Matrix);\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(TempMatrix,Matrix);\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix,TempMatrix);\t\t\t\t//\t>>\n",outerFunctionName);

		fprintf(headerFile,"\tfriend TempMatrix %s(Matrix,double);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempMatrix %s(double,Matrix);\t\t\t\t\t//\t>>\n",outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


	// write some initial code in the file.
	fprintf(fileToWrite,"#include \"../Real Matrices and Vectors.h\"\nusing namespace ASL;\n\n\n");

	// enter loop to generate the rest of the code.
	c = 1;	
	currentDefinition = definitions;
	while(*currentDefinition)
	{
		if(*alternativeCode)
			fprintf(fileToWrite,*alternativeCode);
		else
		{
			sprintf(temp,"%u",c);
			fprintf(fileToWrite,*currentDefinition,outerFunctionName,temp,innerFunctionName,outerFunctionName,temp);
		}
		++c , ++currentDefinition , ++alternativeCode;
	} // end while

} // end function generateBinaryFunctions


void generateReduceBasedFunctions(FILE *fileToWrite, const char *outerFunctionName, const char *innerFunctionName, char **alternativeCode, char *mode , unsigned int countStart)
{
	FILE *headerFile;

	unsigned int c;
	char temp[10];
	char *definitions[] = {
		"#pragma region Vector\n\n// doesn\'t work on vectors with 0 elements.\ndouble ASL::%s(Vector v) // overloaded version %s\n{\n\tregister unsigned int c;\n\tregister anticast p;\n\tregister double accumulator;\n\n\tp = v.data;\t// set p at the beginning of v.\n\tc = *p.ui++;\t// set c to the # of elements of v.\n\n\taccumulator = *p.d;\n\twhile(--c)\n\t\taccumulator = %s%saccumulator %s *++p.d%s;\n\n\tv.data.v = 0;\t// prevent the destractor from deallocating v\'s memory.\n\treturn accumulator;\n} // end function %s overloaded version %s\n\n\n",
		"// doesn\'t work on vectors with 0 elements.\ndouble ASL::%s(TempVector v) // overloaded version %s\n{\n\tregister unsigned int c;\n\tregister anticast p;\n\tregister double accumulator;\n\n\tp = v.data;\t// set p at the beginning of v.\n\tc = *p.ui++;\t// set c to the # of elements of v.\n\n\taccumulator = *p.d;\n\twhile(--c)\n\t\taccumulator = %s%saccumulator %s *++p.d%s;\n\n\tfree(v.data.v);\t// free v\'s memory.\n\treturn accumulator;\n} // end function %s overloaded version %s\n\n#pragma endregion\n#pragma region Matrix\n\n",
		0,
		"// doesn\'t work on matrices with 0 elements.\nTempVector ASL::%s(Matrix A , unsigned int dim) // overloaded version %s\n{\n\tregister unsigned int reduceDimention,c;\n\tregister anticast source,destination;\n\tregister double accumulator;\n\tunsigned int otherDimention;\n\tTempVector v;\n\n\tsource = A.data;\t// set source at the first element of the first column of A.\n\treduceDimention = *source.ui++;\t// set reduceDimention to the # of elements of A.\n\n\tif(dim == 1)\n\t{\n\t\t\t// calculate dimentions and prepare A.\n\t\totherDimention = reduceDimention / *source.ui;\t// set otherDimention to the # of columns of A.\n\t\treduceDimention = *source.ui++;\t// set reduceDimantion to the # of rows of A.\n\t\t\t// prepare v.\n\t\tdestination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));\t// allocate memory for v.\n\t\t*destination.ui++ = otherDimention;\t// store the # of elements of v in v.\n\t\t\t// perform the actual computation.\n\t\twhile(otherDimention--)\t// for each column of A.\n\t\t{\n\t\t\tc = reduceDimention;\t// set c to the # of rows of A.\n\t\t\taccumulator = *source.d;\t// initialize accumulator with the first element of the current column.\n\t\t\twhile(--c)\t// reduce the current column\n\t\t\t\taccumulator = %s%saccumulator %s *++source.d%s;\n\t\t\t*destination.d++ = accumulator;\t// store the result from reducing this column in v.\n\t\t\t++source.d;\t// move the the next column.\n\t\t} // end outer while\n\t}\n\t",
		"else\n\t{\n\t\t\t// calculate dimentions and prepare A.\n\t\treduceDimention = reduceDimention / *source.ui;\t// set reduceDimention to the # of columns of A.\n\t\totherDimention = *source.ui++;\t// set otherDimantion to the # of rows of A.\n\t\t\t// prepare v.\n\t\tdestination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));\t// allocate memory for v.\n\t\t*destination.ui++ = otherDimention;\t// store the # of elements of v in v.\n\t\t\t// perform the actual computation.\n\t\tunsigned int rows = otherDimention;\t// keep the # of rows of A in rows.\n\t\tdouble *row = source.d;\t// set row to the beginning of the first row.\n\n\t\twhile(otherDimention--)\t// for each row of A.\n\t\t{\n\t\t\tc = reduceDimention;\t// set c to the # of columns of A.\n\t\t\taccumulator = *source.d;\t// initialize accumulator with the first element of the current row.\n\t\t\twhile(--c)\t// reduce the current row\n\t\t\t\taccumulator = %s%saccumulator %s *(source.d += rows)%s;\n\t\t\t*destination.d++ = accumulator;\t// store the result from reducing this row in v.\n\t\t\tsource.d = ++row;\t// move the the next row.\n\t\t} // end outer while\n\t}\n\n\tA.data.v = 0;\t// prevent the destractor from deallocating A\'s memory.\n\treturn v;\n} // end function %s overloaded version %s\n\n\n",
		"// doesn\'t work on matrices with 0 elements.\nTempVector ASL::%s(TempMatrix A , unsigned int dim) // overloaded version %s\n{\n\tregister unsigned int reduceDimention,c;\n\tregister anticast source,destination;\n\tregister double accumulator;\n\tunsigned int otherDimention;\n\tTempVector v;\n\n\tsource = A.data;\t// set source at the first element of the first column of A.\n\treduceDimention = *source.ui++;\t// set reduceDimention to the # of elements of A.\n\n\tif(dim == 1)\n\t{\n\t\t\t// calculate dimentions and prepare A.\n\t\totherDimention = reduceDimention / *source.ui;\t// set otherDimention to the # of columns of A.\n\t\treduceDimention = *source.ui++;\t// set reduceDimantion to the # of rows of A.\n\t\t\t// prepare v.\n\t\tdestination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));\t// allocate memory for v.\n\t\t*destination.ui++ = otherDimention;\t// store the # of elements of v in v.\n\t\t\t// perform the actual computation.\n\t\twhile(otherDimention--)\t// for each column of A.\n\t\t{\n\t\t\tc = reduceDimention;\t// set c to the # of rows of A.\n\t\t\taccumulator = *source.d;\t// initialize accumulator with the first element of the current column.\n\t\t\twhile(--c)\t// reduce the current column\n\t\t\t\taccumulator = %s%saccumulator %s *++source.d%s;\n\t\t\t*destination.d++ = accumulator;\t// store the result from reducing this column in v.\n\t\t\t++source.d;\t// move the the next column.\n\t\t} // end outer while\n\t}\n\t",
		"else\n\t{\n\t\t\t// calculate dimentions and prepare A.\n\t\treduceDimention = reduceDimention / *source.ui;\t// set reduceDimention to the # of columns of A.\n\t\totherDimention = *source.ui++;\t// set otherDimantion to the # of rows of A.\n\t\t\t// prepare v.\n\t\tdestination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));\t// allocate memory for v.\n\t\t*destination.ui++ = otherDimention;\t// store the # of elements of v in v.\n\t\t\t// perform the actual computation.\n\t\tunsigned int rows = otherDimention;\t// keep the # of rows of A in rows.\n\t\tdouble *row = source.d;\t// set row to the beginning of the first row.\n\n\t\twhile(otherDimention--)\t// for each row of A.\n\t\t{\n\t\t\tc = reduceDimention;\t// set c to the # of columns of A.\n\t\t\taccumulator = *source.d;\t// initialize accumulator with the first element of the current row.\n\t\t\twhile(--c)\t// reduce the current row\n\t\t\t\taccumulator = %s%saccumulator %s *(source.d += rows)%s;\n\t\t\t*destination.d++ = accumulator;\t// store the result from reducing this row in v.\n\t\t\tsource.d = ++row;\t// move the the next row.\n\t\t} // end outer while\n\t}\n\n\tfree(A.data.v);\t// free A\'s memory.\n\treturn v;\n} // end function %s overloaded version %s\n\n#pragma endregion\n"
	,0};
	char **currentDefinition;


		// write the prototypes for the functions - TempVector.
	if(headerFile = fopen("generated code/header parts/TempVector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend double %s(Vector);\t\t\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fprintf(headerFile,"\tfriend double %s(TempVector);\t\t\t\t\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(Matrix,unsigned int = 1);\t\t\t\t\t\t\t//\t>>\n",outerFunctionName);
		fprintf(headerFile,"\tfriend TempVector %s(TempMatrix,unsigned int = 1);\t\t\t\t\t\t//\t>>\n",outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


		// write the prototypes for the functions - Vector.
	if(headerFile = fopen("generated code/header parts/Vector.part.h","a"))
	{
		fprintf(headerFile,"\tfriend double %s(Vector);\t\t\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


		// write the prototypes for the functions - TempMatrix.
	if(headerFile = fopen("generated code/header parts/TempMatrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector %s(TempMatrix,char);\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


		// write the prototypes for the functions - Matrix.
	if(headerFile = fopen("generated code/header parts/Matrix.part.h","a"))
	{
		fprintf(headerFile,"\tfriend TempVector %s(Matrix,char);\t\t\t\t\t\t\t\t// %s\n",outerFunctionName,outerFunctionName);
		fclose(headerFile);
	}
	else fprintf(stderr,"error opening file.\n");


		// write some initial code in the file.
	fprintf(fileToWrite,"%s",(*mode == 'a')?"":"#include \"../Real Matrices and Vectors.h\"\nusing namespace ASL;\n\n\n");

		// enter loop to generate the rest of the code.
		// primary loop.
	c = countStart;	
	currentDefinition = definitions;
	while(*currentDefinition)
	{
		if(*alternativeCode)
			fprintf(fileToWrite,*alternativeCode);
		else
		{
			sprintf(temp,"%u",c);
			fprintf(fileToWrite,*currentDefinition,outerFunctionName,temp,
				function(innerFunctionName)?innerFunctionName:"",
				function(innerFunctionName)?"(":"",
				function(innerFunctionName)?",":innerFunctionName,
				function(innerFunctionName)?")":"",
				outerFunctionName,temp);
		}
		++c , ++currentDefinition , ++alternativeCode;
	} // end while

		// secondary loop.
	++currentDefinition;
	while(*currentDefinition)
	{
		if(*alternativeCode)
		{
			fprintf(fileToWrite,*alternativeCode);
			++currentDefinition;
		}
		else
		{
			sprintf(temp,"%u",c);
			fprintf(fileToWrite,*currentDefinition,outerFunctionName,temp,
				function(innerFunctionName)?innerFunctionName:"",
				function(innerFunctionName)?"(":"",
				function(innerFunctionName)?",":innerFunctionName,
				function(innerFunctionName)?")":"");
			++currentDefinition;
			fprintf(fileToWrite,*currentDefinition,
				function(innerFunctionName)?innerFunctionName:"",
				function(innerFunctionName)?"(":"",
				function(innerFunctionName)?",":innerFunctionName,
				function(innerFunctionName)?")":"",
				outerFunctionName,temp);
		}
		++c , ++currentDefinition , ++alternativeCode;
	} // end while
} // end function generateBinaryFunctions
