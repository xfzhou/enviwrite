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

#include <query/Operator.h>
#include "EnviParameters.hpp"
#include "EnviWrite.hpp"

namespace scidb
{
class PhysicalEnviWrite : public PhysicalOperator
{
public:
  PhysicalEnviWrite(string const& logicalName,
                    string const& physicalName,
                    Parameters const& parameters,
                    ArrayDesc const& schema) :
    PhysicalOperator(logicalName, physicalName, parameters, schema)
  {
    
  }
  
  /**
   * This method is called once per instance.
   */
  shared_ptr<Array> execute(vector<shared_ptr<Array> >& inputArrays,
    shared_ptr<Query> query)
  {
    // If you are not the coordinator node, sent chunks to the coordinator.
    // If you are the coordinator node, write the info to the file.
    if (query->getInstanceID() != 0)
    {
      
    }
    else
    {
      // Extract operator parameters.
      EnviParameters enviParameters(_parameters, false, query);
      
      // TODO Need to pull all instance's data to the coordinator for the write.
      
      // FIXME Write ENVI hyperspectral array to file.
      EnviWrite enviOut(inputArrays[0], enviParameters.fileName,
        enviParameters.interleave, enviParameters.byteOrder);
    }
    
    // Return an empty array.
    return shared_ptr<Array>(new MemArray(_schema, query));
  }
};

REGISTER_PHYSICAL_OPERATOR_FACTORY(PhysicalEnviWrite, "enviwrite",
  "PhysicalEnviWrite");
} // namespace scidb
