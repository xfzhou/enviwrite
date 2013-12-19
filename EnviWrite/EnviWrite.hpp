/**
 * BEGIN_COPYRIGHT
 *
 * This file is part of SciDB.
 * Copyright (C) 2008-2013 SciDB, Inc.
 *
 * SciDB is free software: you can redistribute it and/or modify
 * it under the terms of the AFFERO GNU General Public License as published by
 * the Free Software Foundation.
 *
 * SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
 * NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
 * the AFFERO GNU General Public License for the complete license terms.
 *
 * You should have received a copy of the AFFERO GNU General Public License
 * along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
 *
 * END_COPYRIGHT
 */

#include <fstream>
#include <query/Operator.h>
#include <stdlib.h>

#ifndef ENVI_WRITE_H
#define ENVI_WRITE_H

namespace scidb
{
/**
 * Used to write the ENVI array stored in SciDB to a hyperspectral image file
 * of format bsq, bil, or bip.
 */
class EnviWrite
{
public:

  /**
   * Exports the ENVI array to the specified file.
   * @param enviArray The ENVI array
   * @param fileName Name of the file to export to
   * @param interleave Hyperspectral image encoding: bsq | bil | bip
   * @param byteOrder Byte order: big endian or little endian
   */
  EnviWrite(shared_ptr<Array>& inputArray, const string& fileName,
            const string& interleave, const string& byteOrder)
  {
    // Create a file and write to it in the home directory.
    char* home = getenv("HOME");
    string fullPath;
    
    // FIXME This may need to be changed when code is finalized.
    if (home != NULL)
    {
      string temp(home);
      fullPath = temp + "/" + fileName + ".txt";
    }
    else
    {
      fullPath = "/home/scidb/" + fileName;
    }
    
    ofstream outputFile(fullPath.c_str(), ios::out/*ios::binary*/);
    
    // The ConstArrayIterator will iterate once for every chunk in the array in
    // row-major order.
    shared_ptr<ConstArrayIterator> arrayIter = inputArray->getConstIterator(0);
    while (!arrayIter->end())
    {
      // The ConstChunkIterator will iterate once for every cell in the chunk in
      // row-major order.
      shared_ptr<ConstChunkIterator> chunkIter = arrayIter->
        getChunk().getConstIterator(ChunkIterator::IGNORE_EMPTY_CELLS |
                                    ChunkIterator::IGNORE_OVERLAPS);
      while (!chunkIter->end())
      {
        Value const& item = chunkIter->getItem();
        outputFile << item.getInt16() << " ";
        
        ++(*chunkIter); // Advance the chunk iterator.
      }
      ++(*arrayIter); // Advance the array iterator.
    }
    outputFile.close();
  }
};  // class EnviWrite
} // namespace scidb

#endif  // ENVI_WRITE_H
