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

#include "../Real Matrices and Vectors.h"
using namespace ASL;


#pragma region Vector

// doesn't work on vectors with 0 elements.
double ASL::$(Vector v) // overloaded version $
{
	register unsigned int c;
	register anticast p;
	register double accumulator;

	p = v.data;	// set p at the beginning of v.
	c = *p.ui++;	// set c to the # of elements of v.

	accumulator = *p.d;
	while(--c)
		accumulator = $$accumulator $ *++p.d$;

	v.data.v = 0;	// prevent the destractor from deallocating v's memory.
	return accumulator;
} // end function $ overloaded version $


@// doesn't work on vectors with 0 elements.
double ASL::$(TempVector v) // overloaded version $
{
	register unsigned int c;
	register anticast p;
	register double accumulator;

	p = v.data;	// set p at the beginning of v.
	c = *p.ui++;	// set c to the # of elements of v.

	accumulator = *p.d;
	while(--c)
		accumulator = $$accumulator $ *++p.d$;

	free(v.data.v);	// free v's memory.
	return accumulator;
} // end function $ overloaded version $

#pragma endregion
#pragma region Matrix

@// doesn't work on matrices with 0 elements.
TempVector ASL::$(Matrix A , unsigned int dim) // overloaded version $
{
	register unsigned int reduceDimention,c;
	register anticast source,destination;
	register double accumulator;
	unsigned int otherDimention;
	TempVector v;

	source = A.data;	// set source at the first element of the first column of A.
	reduceDimention = *source.ui++;	// set reduceDimention to the # of elements of A.

	if(dim == 1)
	{
			// calculate dimentions and prepare A.
		otherDimention = reduceDimention / *source.ui;	// set otherDimention to the # of columns of A.
		reduceDimention = *source.ui++;	// set reduceDimantion to the # of rows of A.
			// prepare v.
		destination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));	// allocate memory for v.
		*destination.ui++ = otherDimention;	// store the # of elements of v in v.
			// perform the actual computation.
		while(otherDimention--)	// for each column of A.
		{
			c = reduceDimention;	// set c to the # of rows of A.
			accumulator = *source.d;	// initialize accumulator with the first element of the current column.
			while(--c)	// reduce the current column
				accumulator = $$accumulator $ *++source.d$;
			*destination.d++ = accumulator;	// store the result from reducing this column in v.
			++source.d;	// move the the next column.
		} // end outer while
	}
	@else
	{
			// calculate dimentions and prepare A.
		reduceDimention = reduceDimention / *source.ui;	// set reduceDimention to the # of columns of A.
		otherDimention = *source.ui++;	// set otherDimantion to the # of rows of A.
			// prepare v.
		destination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));	// allocate memory for v.
		*destination.ui++ = otherDimention;	// store the # of elements of v in v.
			// perform the actual computation.
		unsigned int rows = otherDimention;	// keep the # of rows of A in rows.
		double *row = source.d;	// set row to the beginning of the first row.

		while(otherDimention--)	// for each row of A.
		{
			c = reduceDimention;	// set c to the # of columns of A.
			accumulator = *source.d;	// initialize accumulator with the first element of the current row.
			while(--c)	// reduce the current row
				accumulator = $$accumulator $ *(source.d += rows)$;
			*destination.d++ = accumulator;	// store the result from reducing this row in v.
			source.d = ++row;	// move the the next row.
		} // end outer while
	}

	A.data.v = 0;	// prevent the destractor from deallocating A's memory.
	return v;
} // end function $ overloaded version $


@// doesn't work on matrices with 0 elements.
TempVector ASL::$(TempMatrix A , unsigned int dim) // overloaded version $
{
	register unsigned int reduceDimention,c;
	register anticast source,destination;
	register double accumulator;
	unsigned int otherDimention;
	TempVector v;

	source = A.data;	// set source at the first element of the first column of A.
	reduceDimention = *source.ui++;	// set reduceDimention to the # of elements of A.

	if(dim == 1)
	{
			// calculate dimentions and prepare A.
		otherDimention = reduceDimention / *source.ui;	// set otherDimention to the # of columns of A.
		reduceDimention = *source.ui++;	// set reduceDimantion to the # of rows of A.
			// prepare v.
		destination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));	// allocate memory for v.
		*destination.ui++ = otherDimention;	// store the # of elements of v in v.
			// perform the actual computation.
		while(otherDimention--)	// for each column of A.
		{
			c = reduceDimention;	// set c to the # of rows of A.
			accumulator = *source.d;	// initialize accumulator with the first element of the current column.
			while(--c)	// reduce the current column
				accumulator = $$accumulator $ *++source.d$;
			*destination.d++ = accumulator;	// store the result from reducing this column in v.
			++source.d;	// move the the next column.
		} // end outer while
	}
	@else
	{
			// calculate dimentions and prepare A.
		reduceDimention = reduceDimention / *source.ui;	// set reduceDimention to the # of columns of A.
		otherDimention = *source.ui++;	// set otherDimantion to the # of rows of A.
			// prepare v.
		destination.v = v.data.v = malloc(sizeof(unsigned int) + otherDimention*sizeof(double));	// allocate memory for v.
		*destination.ui++ = otherDimention;	// store the # of elements of v in v.
			// perform the actual computation.
		unsigned int rows = otherDimention;	// keep the # of rows of A in rows.
		double *row = source.d;	// set row to the beginning of the first row.

		while(otherDimention--)	// for each row of A.
		{
			c = reduceDimention;	// set c to the # of columns of A.
			accumulator = *source.d;	// initialize accumulator with the first element of the current row.
			while(--c)	// reduce the current row
				accumulator = $$accumulator $ *(source.d += rows)$;
			*destination.d++ = accumulator;	// store the result from reducing this row in v.
			source.d = ++row;	// move the the next row.
		} // end outer while
	}

	free(A.data.v);	// free A's memory.
	return v;
} // end function $ overloaded version $

#pragma endregion
