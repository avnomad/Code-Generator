#include "../Real Matrices and Vectors.h"
using namespace ASL;


#pragma region Vector & Vector

// vectors v and u are assumed to be of the same size.
TempVector ASL::operator $ (Vector v, Vector u)	// overloaded version $
{
	TempVector w;
	unsigned int c;
	anticast p;

	c = *v.data.ui++;
	++u.data.ui;
	p.v = w.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));
	*p.ui++ = c;
	while(c--)
		*p.d++ = *v.data.d++ $ *u.data.d , ++u.data.d;

	v.data.v = u.data.v = 0;
	return w;
} // end function operator $ overloaded version $


@// vectors v and u are assumed to be of the same size.
TempVector ASL::operator $ (TempVector v, Vector u)	// overloaded version $
{
	unsigned int c;
	anticast p;

	p = v.data;
	c = *p.ui++;
	++u.data.ui;

	while(c--)
		*p.d = *p.d $ *u.data.d , ++p.d , ++u.data.d;

	u.data.v = 0;
	return v;
} // end function operator $ overloaded version $


@// vectors v and u are assumed to be of the same size.
TempVector ASL::operator $ (Vector u, TempVector v)	// overloaded version $
{
	unsigned int c;
	anticast p;

	p = v.data;
	c = *p.ui++;
	++u.data.ui;

	while(c--)
		*p.d = *u.data.d++ $ *p.d , ++p.d;

	u.data.v = 0;
	return v;
} // end function operator $ overloaded version $


@// vectors v and u are assumed to be of the same size.
TempVector ASL::operator $ (TempVector v, TempVector u)	// overloaded version $
{
	unsigned int c;
	anticast p,q;

	p = v.data;
	q = u.data;
	c = *p.ui++;
	++q.ui;

	while(c--)
		*p.d = *p.d $ *q.d , ++p.d , ++q.d;

	free(u.data.v);
	return v;
} // end function operator $ overloaded version $

#pragma endregion
#pragma region Vector & Scalar

@TempVector ASL::operator $ (double x, Vector v)	// overloaded version $
{
	unsigned int c;
	TempVector u;
	anticast p;

	c = *v.data.ui++;	// get the size of v.
	p.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));	// allocate memory for a new TempVector of that size.
	*p.ui++ = c;		// save the size of the new TempVector.

	while(c--)		// for each element of v.
		*p.d++ = x $ *v.data.d , ++v.data.d;	
	
	v.data.v = 0;	// prevents the destractor from deallocating v's memory.
	return u;
} // end function operator $ overloaded version $


@TempVector ASL::operator $ (Vector v, double x)	// overloaded version $
{
	unsigned int c;
	TempVector u;
	anticast p;

	c = *v.data.ui++;	// get the size of v.
	p.v = u.data.v = malloc(sizeof(unsigned int) + c*sizeof(double));	// allocate memory for a new TempVector of that size.
	*p.ui++ = c;		// save the size of the new TempVector.

	while(c--)		// for each element of v.
		*p.d++ = *v.data.d++ $ x;	
	
	v.data.v = 0;	// prevents the destractor from deallocating v's memory.
	return u;
} // end function operator $ overloaded version $


@TempVector ASL::operator $ (double x, TempVector v)	// overloaded version $
{	/* we will recycle the memory used by v. */
	unsigned int c;
	anticast p;

	p = v.data;		// set p to point at the begining of v.
	c = *p.ui++;	// get the size of v.

	while(c--)		// for each element of v.
		*p.d = x $ *p.d , ++p.d;

	return v;
} // end function operator $ overloaded version $


@TempVector ASL::operator $ (TempVector v, double x)	// overloaded version $
{	/* we will recycle the memory used by v. */
	unsigned int c;
	anticast p;

	p = v.data;		// set p to point at the begining of v.
	c = *p.ui++;	// get the size of v.

	while(c--)		// for each element of v.
		*p.d = *p.d $ x , ++p.d;

	return v;
} // end function operator $ overloaded version $

#pragma endregion
#pragma region Matrix & Matrix

@// matrices A and B are assumed to be of the same size.
TempMatrix ASL::operator $ (Matrix A, Matrix B)	// overloaded version $
{
	register unsigned int c;
	register anticast p;
	TempMatrix C;

	c = *A.data.ui++;	// set c to the # of elements of A.
	++B.data.ui;	// skip the # of elements of B.
	p.v = C.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));
	*p.ui++ = c;	// store the # of elements of A in C.
	*p.ui++ = *A.data.ui++;	// store the # of rows of A in C.
	++B.data.ui;	// skip the # of rows of B.

	while(c--)
		*p.d++ = *A.data.d++ $ *B.data.d , ++B.data.d;

	A.data.v = B.data.v = 0;	// prevent the destractor from deallocating A's and B's memory.
	return C;
} // end function operator $ overloaded version $


@// matrices A and B are assumed to be of the same size.
TempMatrix ASL::operator $ (TempMatrix A, Matrix B)	// overloaded version $
{	/* we will recycle the memory used by A. */
	register unsigned int c;
	register anticast p;

	p = A.data;		// set p at the begining of A.
	c = *p.ui++;	// set c to the # of elements of A.
	++B.data.ui;	// skip the # of elements of B.
	++p.ui , ++B.data.ui;	// skip the # of rows of A and B.

	while(c--)
		*p.d = *p.d $ *B.data.d , ++p.d , ++B.data.d;

	B.data.v = 0;	// prevent the destractor from deallocating B's memory.
	return A;
} // end function operator $ overloaded version $


@// matrices A and B are assumed to be of the same size.
TempMatrix ASL::operator $ (Matrix B, TempMatrix A)	// overloaded version $
{	/* we will recycle the memory used by A. */
	register unsigned int c;
	register anticast p;

	p = A.data;		// set p at the begining of A.
	c = *p.ui++;	// set c to the # of elements of A.
	++B.data.ui;	// skip the # of elements of B.
	++p.ui , ++B.data.ui;	// skip the # of rows of A and B.

	while(c--)
		*p.d = *B.data.d++ $ *p.d , ++p.d;

	B.data.v = 0;
	return A;
} // end function operator $ overloaded version $


@// matrices A and B are assumed to be of the same size.
TempMatrix ASL::operator $ (TempMatrix A, TempMatrix B)	// overloaded version $
{	/* we will recycle the memory used by A. */
	register unsigned int c;
	register anticast p,q;

	p = A.data;		// set p at the begining of A.
	q = B.data;		// set q at the begining of B.
	c = *p.ui++;	// set c to the # of elements of A.
	++q.ui;	// skip the # of elements of B.
	++p.ui , ++q.ui;	// skip the # of rows of A and B.

	while(c--)
		*p.d = *p.d $ *q.d , ++p.d , ++q.d;

	free(B.data.v);	// free the memory occupied by the temporary matric B.
	return A;
} // end function operator $ overloaded version $

#pragma endregion
#pragma region Matrix & Scalar

@TempMatrix ASL::operator $ (double x, Matrix A)	// overloaded version $
{
	register unsigned int c;
	register anticast p;
	TempMatrix B;

	c = *A.data.ui++;	// set c to the # of elements of A.
	p.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));	// allocate memory for a new TempMatrix of that many elements.
	*p.ui++ = c;	// store the # of elements of A in B.
	*p.ui++ = *A.data.ui++;	// store the # of rows of A in B.

	while(c--)		// for each element of A.
		*p.d++ = x $ *A.data.d , ++A.data.d;	
	
	A.data.v = 0;	// prevents the destractor from deallocating A's memory.
	return B;
} // end function operator $ overloaded version $


@TempMatrix ASL::operator $ (Matrix A, double x)	// overloaded version $
{
	register unsigned int c;
	TempMatrix B;
	register anticast p;

	c = *A.data.ui++;	// set c to the # of elements of A.
	p.v = B.data.v = malloc(2*sizeof(unsigned int) + c*sizeof(double));	// allocate memory for a new TempMatrix of that many elements.
	*p.ui++ = c;	// store the # of elements of A in B.
	*p.ui++ = *A.data.ui++;	// store the # of rows of A in B.

	while(c--)		// for each element of A.
		*p.d++ = *A.data.d++ $ x;	
	
	A.data.v = 0;	// prevents the destractor from deallocating A's memory.
	return B;
} // end function operator $ overloaded version $


@TempMatrix ASL::operator $ (double x, TempMatrix A)	// overloaded version $
{	/* we will recycle the memory used by A. */
	register unsigned int c;
	register anticast p;

	p = A.data;		// set p to point at the begining of A.
	c = *p.ui++;	// set c to the # of elements of A.
	++p.ui;	// skip the # of rows of A.

	while(c--)		// for each element of A.
		*p.d = x $ *p.d , ++p.d;

	return A;
} // end function operator $ overloaded version $


@TempMatrix ASL::operator $ (TempMatrix A, double x)	// overloaded version $
{	/* we will recycle the memory used by A. */
	register unsigned int c;
	register anticast p;

	p = A.data;		// set p to point at the begining of A.
	c = *p.ui++;	// set c to the # of elements of A.
	++p.ui;	// skip the # of rows of A.

	while(c--)		// for each element of A.
		*p.d = *p.d $ x , ++p.d;

	return A;
} // end function operator $ overloaded version $

#pragma endregion
