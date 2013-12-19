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

/**
 * This operator takes an ENVI array and exports it back out into a bsq, bip, or
 * bil file. TODO Make a note of the ENVI codefication.
 */

#include <query/Operator.h>
#include "EnviParameters.hpp"

namespace scidb
{
class LogicalEnviWrite : public LogicalOperator
{
public:
  LogicalEnviWrite(const string& logicalName, const string& alias):
    LogicalOperator(logicalName, alias)
  {
    ADD_PARAM_INPUT();              // array name
    ADD_PARAM_CONSTANT("string");   // file name
    ADD_PARAM_CONSTANT("string");   // interleave
    ADD_PARAM_CONSTANT("string");   // byte order
  }
  
  ArrayDesc inferSchema(vector<ArrayDesc> schemas, shared_ptr<Query> query)
  {
    // Throw an error if the schema of the input array does not match that of
    // an ENVI hyperspectral image array.
    ArrayDesc const& inputSchema = schemas[0];
    
    if (!(inputSchema.getAttributes(true).size() == 1 && (
            inputSchema.getAttributes(true)[0].getType() == TID_UINT8 ||
            inputSchema.getAttributes(true)[0].getType() == TID_UINT16 ||
            inputSchema.getAttributes(true)[0].getType() == TID_INT16 ||
            inputSchema.getAttributes(true)[0].getType() == TID_INT32 ||
            inputSchema.getAttributes(true)[0].getType() == TID_FLOAT ||
            inputSchema.getAttributes(true)[0].getType() == TID_DOUBLE) ||
          inputSchema.getAttributes(true).size() == 2 &&
            inputSchema.getAttributes(true)[0].getType() == TID_DOUBLE &&
            inputSchema.getAttributes(true)[1].getType() == TID_DOUBLE))
    {
      throw SYSTEM_EXCEPTION(SCIDB_SE_OPERATOR, SCIDB_LE_ILLEGAL_OPERATION)
        << "Input array does not hold an ENVI hyperspectral image.";
    }
    
    // Extract operator parameters.
    EnviParameters enviParameters(_parameters, true, query);
    
    // Throw an error if interleave argument is not valid.
    if (enviParameters.interleave.compare("bsq") != 0 &&
        enviParameters.interleave.compare("bil") != 0 &&
        enviParameters.interleave.compare("bip") != 0)
    {
      throw SYSTEM_EXCEPTION(SCIDB_SE_OPERATOR, SCIDB_LE_ILLEGAL_OPERATION)
        << "Invalid interleave type.";
    }
    
    // Throw an error if byte order argument is not valid.
    if (enviParameters.byteOrder.compare("ieee-le") != 0 &&
        enviParameters.byteOrder.compare("ieee-be") != 0)
    {
      throw SYSTEM_EXCEPTION(SCIDB_SE_OPERATOR, SCIDB_LE_ILLEGAL_OPERATION)
        << "Invalid byte order type.";
    }
    
    // Return a one element array that says "done".
    // The construction of the array is in PhysicalEnviWrite.cpp.
    AttributeDesc outputAttribute(0, "value", TID_STRING, 0, 0);
    Attributes outputAttributes(1, outputAttribute);
    outputAttributes = addEmptyTagAttribute(outputAttributes);
    
    DimensionDesc outputDimension("i", 0, 0, 1, 0);
    Dimensions outputDimensions(1, outputDimension);
    
    return ArrayDesc("return", outputAttributes, outputDimensions);
  }
};

REGISTER_LOGICAL_OPERATOR_FACTORY(LogicalEnviWrite, "enviwrite");
} // namespace scidb
