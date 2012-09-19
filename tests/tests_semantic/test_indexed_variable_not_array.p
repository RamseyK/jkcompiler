program testIndexedVariableNotArray;

class testIndexedVariableNotArray

BEGIN

   VAR 
      rows	: ARRAY[0 .. 9] OF boolean;
      tests	: boolean;
      moreTests	: boolean;

FUNCTION testIndexedVariableNotArray;
BEGIN	   
   tests := moreTests[5]
END

END
.

