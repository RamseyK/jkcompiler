program testTypeMismatchArrayInteger;

class testTypeMismatchArrayInteger

BEGIN

   VAR 
      compilerWorks : boolean;
      timeToWrite   : ARRAY[4..9] OF integer;

FUNCTION testTypeMismatchArrayInteger;
BEGIN
   compilerWorks := timeToWrite[4]
END

END
.

