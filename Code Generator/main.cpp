#include <stdio.h>
#include <string.h>

#include <iostream>
using namespace std;

#include "code generators.h"


int main()
{
	FILE *writeFile;
	// unary functions.
	char *outerUnaryFunctionNames[] = {
						"sin","cos","tan",
						"asin","acos","atan",
						"sinh","cosh","tanh",
						"exp","log","log10",
						"sqrt","ceil","floor",
						"abs",0		//must end with a Null pointer.
					};
	char *innerUnaryFunctionNames[] = {
						"::sin","::cos","::tan",
						"::asin","::acos","::atan",
						"::sinh","::cosh","::tanh",
						"::exp","::log","::log10",
						"::sqrt","::ceil","::floor",
						"::fabs",0		//must end with a Null pointer.
					};
	// unary operators.
	char *unaryOperatorFileNames[] = {
						"unary plus","unary minus","logical not",0		//must end with a Null pointer.
					};
	char *outerUnaryOperatorNames[] = {
						"+","-","!",0		//must end with a Null pointer.
					};
	char *innerUnaryOperatorNames[] = {
						"","-","!",0		//must end with a Null pointer.
					};
	// binary operators.
	char *ip1 = {"#pragma region Vector & Vector\n\n// vectors v and u are assumed to be of the same size.\ndouble ASL::operator * (Vector v, Vector u)\t// overloaded version 1\n{\n\tregister unsigned int c;\n\tregister double innerProduct;\n\n\tc = *v.data.ui++;\t// set c to the # of elements of v.\n\tif(!c) return 0;\t// cope with the case where v is empty.\n\t++u.data.ui;\t// skip the # of elements of u.\n\t\n\tinnerProduct = *v.data.d * *u.data.d;\n\twhile(--c)\n\t\tinnerProduct += *++v.data.d * *++u.data.d;\n\n\tv.data.v = u.data.v = 0;\t// prevent the destractor from deallocating v\'s and u\'s memory.\n\treturn innerProduct;\n} // end function operator * overloaded version 1\n\n\n"};
	char *ip2 = {"// vectors v and u are assumed to be of the same size.\ndouble ASL::operator * (TempVector v, Vector u)\t// overloaded version 2\n{\n\tregister unsigned int c;\n\tregister double innerProduct;\n\tregister anticast p;\n\n\tp = v.data;\t// set p at the beginning of v.\n\tc = *p.ui++;\t// set c to the # of elements of v.\n\tif(!c) return 0;\t// cope with the case where v is empty.\n\t++u.data.ui;\t// skip the # of elements of u.\n\t\n\tinnerProduct = *p.d * *u.data.d;\n\twhile(--c)\n\t\tinnerProduct += *++p.d * *++u.data.d;\n\n\tfree(v.data.v);\t// free v\'s memory.\n\tu.data.v = 0;\t// prevent the destractor from deallocating u\'s memory.\n\treturn innerProduct;\n} // end function operator * overloaded version 2\n\n\n"};
	char *ip3 = {"// vectors v and u are assumed to be of the same size.\ndouble ASL::operator * (Vector v, TempVector u)\t// overloaded version 3\n{\n\tregister unsigned int c;\n\tregister double innerProduct;\n\tregister anticast p;\n\n\tc = *v.data.ui++;\t// set c to the # of elements of v.\n\tif(!c) return 0;\t// cope with the case where v is empty.\n\tp = u.data;\t// set p at the beginning of u.\n\t++p.ui;\t// skip the # of elements of u.\n\t\n\tinnerProduct = *v.data.d * *p.d;\n\twhile(--c)\n\t\tinnerProduct += *++v.data.d * *++p.d;\n\n\tfree(u.data.v);\t// free u\'s memory.\n\tv.data.v = 0;\t// prevent the destractor from deallocating v\'s memory.\n\treturn innerProduct;\n} // end function operator * overloaded version 3\n\n\n"};
	char *ip4 = {"// vectors v and u are assumed to be of the same size.\ndouble ASL::operator * (TempVector v, TempVector u)\t// overloaded version 4\n{\n\tregister unsigned int c;\n\tregister double innerProduct;\n\tregister anticast p,q;\n\n\tp = v.data;\t// set p at the beginning of v.\n\tq = u.data;\t// set q at the beginning of u.\n\tc = *p.ui++;\t// set c to the # of elements of v.\n\tif(!c) return 0;\t// cope with the case where v is empty.\n\t++q.ui;\t// skip the # of elements of u.\n\t\n\tinnerProduct = *p.d * *q.d;\n\twhile(--c)\n\t\tinnerProduct += *++p.d * *++q.d;\n\n\tfree(v.data.v);\t// free v\'s memory.\n\tfree(u.data.v);\t// free u\'s memory.\n\treturn innerProduct;\n} // end function operator * overloaded version 4\n\n#pragma endregion\n#pragma region Vector & Scalar\n\n"};
	char *binaryOperatorFileNames[] = {
								"addition","subtraction","less than",
								"greater than","less than or equal","greater than or equal",
								"equality","unequality","logical and",
								"logical or","bitwise and","bitwise or",
								"multiplication assignment","division assignment","multiplication",
								"division",0	//must end with a Null pointer.
							};
	char *outerBinaryOperatorNames[] = {
								"+","-","<",
								">","<=",">=",
								"==","!=","&&",
								"||","&","|",
								"*=","/=","*",
								"/",0
							};
	char *innerBinaryOperatorNames[] = {
								"+","-","<",
								">","<=",">=",
								"==","!=","&&",
								"||","&&","||",
								"*","/","*",
								"/",0
							};
	char *alternativeOperatorCode[][16] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ip1,ip2,ip3,ip4,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	// binary functions.
	char *binaryFunctionFileNames[] = {
								"bitwise Xor operator","bitwise Xor assignment operator","modulus operator",
								"max","min",0	//must end with a Null pointer.
							};
	char *outerBinaryFunctionNames[] = {
								"operator ^ ","operator ^= ","operator % ",
								"max","min",0	//must end with a Null pointer.
							};
	char *innerBinaryFunctionNames[] = {
								"pow","pow","fmod",
								"ASL::max","ASL::min",0	//must end with a Null pointer.
							};
	char *alternativeFunctionCode[][16] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	// reduce-based functions.
	char *reduceBasedFunctionFileNames[] = {
									"max","min","sum",
									"prod","any","all",
									0
								};
	char *outerReduceBasedFunctionNames[] = {
								"max","min","sum",
								"prod","any","all",
								0
							};
	char *innerReduceBasedFunctionNames[] = {
								"ASL::max","ASL::min","+",
								"*","||","&&",
								0
							};
	char *alternativeReduceBasedFunctionCode[][4] = {
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	char *reduceBasedFunctionModes[] = {
								"a","a","w",
								"w","w","w",
								0
							};
	unsigned int reduceBasedFunctionCountStart[] = {17,17,1,1,1,1};



	// pointers for iterations.
	char **p,**q,**r,**m;
	char *(*s)[16];
	char *(*t)[4];
	unsigned int *cs;
	// other
	char buffer[1024] = "generated code/source files/";
	unsigned int l;

	l = strlen(buffer);

	// remove old files
	remove("generated code/header parts/TempVector.part.h");
	remove("generated code/header parts/Vector.part.h");
	remove("generated code/header parts/TempMatrix.part.h");
	remove("generated code/header parts/Matrix.part.h");

	 /* test input generator */
	//if(writeFile = fopen("c:\\Library\\main.txt","w"))
	//{
	//	p = outerBinaryOperatorNames;
	//	while(*p)
	//	{
	//		fprintf(writeFile,"\tcout<<(((((x %s v) %s (u %s x)) %s u) %s (v %s((linspace(-4,4,9) %s x) %s (x %s linspace(-5,5,9))))) %s ((v %s u) %s w));\n",*p,*p,*p,*p,*p,*p,*p,*p,*p,*p,*p,*p);
	//		++p;
	//	}
	//	fprintf(writeFile,"\n");
	//	/*p = operatorNames;
	//	while(*p)
	//	{
	//		fprintf(writeFile,"\tw = (v %s u);\n\tcout<<w;\n",*p);
	//		++p;
	//	}
	//	fprintf(writeFile,"\n");*/

	//	fclose(writeFile);
	//}
	//else fprintf(stderr,"error opening file.\n");*/


	/* actual code generator */

									/* binary function generation loop */

	p = binaryFunctionFileNames;			//generated succesfully.
	q = outerBinaryFunctionNames;
	r = innerBinaryFunctionNames;
	s = alternativeFunctionCode;
	while(*p)
	{
		sprintf(&buffer[l],"%s.cpp",*p,*p);
		if(writeFile = fopen(buffer,"w"))
		{
			generateBinaryFunctions(writeFile,*q,*r,*s);
			fclose(writeFile);
		}
		else
			fprintf(stderr,"error opening the file \"%s\".\n",buffer);
		++p , ++q , ++r , ++s;
	}

									/* binary operator generation loop */

	p = binaryOperatorFileNames;			//generated succesfully.
	q = outerBinaryOperatorNames;
	r = innerBinaryOperatorNames;
	s = alternativeOperatorCode;
	while(*p)
	{
		sprintf(&buffer[l],"%s operator.cpp",*p,*p);
		if(writeFile = fopen(buffer,"w"))
		{
			generateBinaryOperators(writeFile,*q,*r,*s);
			fclose(writeFile);
		}
		else
			fprintf(stderr,"error opening the file \"%s\".\n",buffer);
		++p , ++q , ++r , ++s;
	}

									/* unary operator generation loop */

	p = unaryOperatorFileNames;			//generated succesfully.
	q = outerUnaryOperatorNames;
	r = innerUnaryOperatorNames;
	while(*p)
	{
		sprintf(&buffer[l],"%s operator.cpp",*p,*p);
		if(writeFile = fopen(buffer,"w"))
		{
			generateUnaryOperators(writeFile,*q,*r);
			fclose(writeFile);
		}
		else
			fprintf(stderr,"error opening the file \"%s\".\n",buffer);
		++p , ++q , ++r;
	}

									/* unary function generation loop */

	p = outerUnaryFunctionNames;			//generated succesfully.
	q = innerUnaryFunctionNames;
	while(*p)
	{
		sprintf(&buffer[l],"%s.cpp",*p,*p);
		if(writeFile = fopen(buffer,"w"))
		{
			generateUnaryFunctions(writeFile,*p,*q);
			fclose(writeFile);
		}
		else
			fprintf(stderr,"error opening the file \"%s\".\n",buffer);
		++p , ++q;
	}

									/* reduce-based function generation loop */

	p = reduceBasedFunctionFileNames;			//generated succesfully.
	q = outerReduceBasedFunctionNames;
	r = innerReduceBasedFunctionNames;
	m = reduceBasedFunctionModes;
	cs = reduceBasedFunctionCountStart;
	t = alternativeReduceBasedFunctionCode;
	while(*p)
	{
		sprintf(&buffer[l],"%s.cpp",*p,*p);
		if(writeFile = fopen(buffer,*m))
		{
			generateReduceBasedFunctions(writeFile,*q,*r,*t,*m,*cs);
			fclose(writeFile);
		}
		else
			fprintf(stderr,"error opening the file \"%s\".\n",buffer);
		++p , ++q , ++r , ++m , ++cs , ++t;
	}


	system("PAUSE");
	return 0;
}                                                                                                                                     