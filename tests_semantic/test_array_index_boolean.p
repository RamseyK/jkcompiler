program testArrayIndexBoolean;

class testArrayIndexBoolean

BEGIN

   VAR 
      rows	   : ARRAY[0 .. 9] OF boolean;
      tests	   : boolean;

FUNCTION testArrayIndexBoolean;
BEGIN	   
   tests := rows[True]
END

END
.

