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

#ifndef OPERATOR_PARAMETERS_H
#define OPERATOR_PARAMETERS_H

namespace scidb
{
/**
 * Used to access the operator's parameters: fileName, interleave, and
 * byteOrder. These member variables are public so simply access them after
 * contructing the object.
 * 
 * Example:
 * EnviParameters param(_parameters, true, query);
 * cout << param.fileName;
 * cout << param.interleave;
 * cout << param.byteOrder;
 */
class EnviParameters
{
public:
  // These member variables are public and set at object construction.
  string fileName;
  string interleave;
  string byteOrder;
  
  /**
   * Extracts the operator's parameters and places them in fileName, interleave
   * and byteOrder member variables.
   * @param parameters Parameters passed to the operator
   * @param phase 'true' if called in logical, 'false' if called in physical
   * @param query Query context
   */
  EnviParameters(vector<shared_ptr<OperatorParam> > const& parameters,
                         bool phase,
                         shared_ptr<Query>& query)
  {
    // There is different logic if this function is called in the logical phase
    // verses the physical phase.
    if (phase)
    {
      fileName = evaluate(((shared_ptr<OperatorParamLogicalExpression>&)
        parameters[0])->getExpression(), query, TID_STRING).getString();
      interleave = evaluate(((shared_ptr<OperatorParamLogicalExpression>&)
        parameters[1])->getExpression(), query, TID_STRING).getString();
      byteOrder = evaluate(((shared_ptr<OperatorParamLogicalExpression>&)
        parameters[2])->getExpression(), query, TID_STRING).getString();
    }
    else
    {
      fileName = ((shared_ptr<OperatorParamPhysicalExpression>&)
        parameters[0])->getExpression()->evaluate().getString();
      interleave = ((shared_ptr<OperatorParamPhysicalExpression>&)
        parameters[1])->getExpression()->evaluate().getString();
      byteOrder = ((shared_ptr<OperatorParamPhysicalExpression>&)
        parameters[2])->getExpression()->evaluate().getString();
    }
  }
};  // class EnviParameters
} // namespace scidb

#endif  // OPERATOR_PARAMETERS_H
