program testFunctionCallArgArray;

class testFunctionCallArgArray

BEGIN
   
   VAR
      dummyArray : ARRAY[0..9] OF integer;
      retval	 : integer;

FUNCTION setCompilerWorks(value	: ARRAY[0..9] OF integer ): integer;
BEGIN
   PRINT value[6]
END   
   

FUNCTION testFunctionCallArgArray;
BEGIN
   dummyArray[6] := 8;
   retval := setCompilerWorks(dummyArray)
END

END
.
