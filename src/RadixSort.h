/*
 * Copyright (c) 2013-2014 Daniel Kirchner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE ANDNONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "common.h"
#include "ShaderProgram.h"

/** Radix sort class.
 * This class is responsible for sorting the particle buffer
 * with respect to their grid id that is computed from their
 * position.
 */
class RadixSort
{
public:
	/** Constructor.
	 * \param blocksize Block size used for sortig the particles.
	 * \param numblocks Number of blocks of values to sort.
	 * \param gridsize size of the particle grid
	 */
	 RadixSort (GLuint blocksize, GLuint numblocks, const glm::ivec3 &gridsize);
	 /** Destuctor.
	  */
	 ~RadixSort (void);
	 /** Get content buffer.
	  * Returns the internal buffer object.
	  * Warning: As two buffer objects are used internally (the data is swapped
	  * between them while sorting) the returned value is not always the same!
	  * Note: This function is guaranteed to return the correct buffer object
	  * that is valid until the next call of Run(...).
	  * \returns the internal buffer object.
	  */
	 GLuint GetBuffer (void) const;
	 /** Sort the buffer.
	  * Sorts the buffer.
	  */
	 void Run (void);
private:
	 /** Sort bits.
	  * Sorts the internal buffer with respect to two bits.
	  * \param bits specifies less significant bit with respect to which to sort
	  */
	 void SortBits (int bits);

	 /** Number of relevant bits.
	  * Number of relevant bits that have to be sorted.
	  */
	 unsigned int numbits;

	 /** Counting shader program.
	  * Shader program used to count the key bits and thereby generate a prefix sum.
	  */
	 ShaderProgram counting;
	 /** Block scan shader program.
	  * Shader program used to create a prefix sum of a block of data.
	  */
	 ShaderProgram blockscan;
	 /** Global sort shader program.
	  * Shader program used to map the values to their correct global position.
	  */
	 ShaderProgram globalsort;
	 /** Add block sum shader program.
	  * Shader program used to add the (separately computed) sum of all preceding blocks
	  * to some blocks.
	  */
	 ShaderProgram addblocksum;
	 union {
		 struct {
			 /** Source buffer.
			  * This buffer is used as the input for the next sorting operation.
			  * During each sorting oparation this value is swapped with result.
			  */
			 GLuint buffer;
			 /** Prefix sum buffer.
			  * This buffer stores the prefix sums used to determine the global positions.
			  */
			 GLuint prefixsums;
			 /** Destination buffer.
			  * This buffer is used as the output for the next sorting operation.
			  * During each sorting operation this value is swapped with buffer.
			  */
			 GLuint result;
		 };
		 /** Buffer objects.
		  * The buffer objects are stored in a union, so that it is possible
		  * to create/delete all buffer objects with a single OpenGL call.
		  */
		 GLuint buffers[3];
	 };
	 /** Blocksums array.
	  * Stores the number of blocks to sum up at each level.
	  */
	 std::vector<GLuint> blocksums;

	 /** Block size.
	  * Stores the size of one block.
	  */
	 const uint32_t blocksize;
	 /** Number of blocks.
	  * Stores the number of blocks to be sorted.
	  */
	 const uint32_t numblocks;
	 /** Bit shift uniform location (counting shader).
	  * Uniform location for the bit shift variable in the counting shader.
	  */
	 int counting_bitshift;
	 /** Bit shift uniform location (global sort shader).
	  * Uniform location for the bit shift variable in the global sort shader.
	  */
	 int globalsort_bitshift;
};

#endif /* !defined RADIXSORT_H */
