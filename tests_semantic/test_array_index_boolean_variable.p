program testArrayIndexBooleanVariable;

class testArrayIndexBooleanVariable

BEGIN

   VAR 
      rows    : ARRAY[0 .. 9] OF boolean;
      tests   : boolean;
      someVar : boolean;

FUNCTION testArrayIndexBooleanVariable;
BEGIN	   
   tests := rows[someVar]
END

END
.

