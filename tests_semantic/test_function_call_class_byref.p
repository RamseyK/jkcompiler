program testFunctionCallClassByRef;

class testFunctionCallClassByRef

BEGIN
   
   VAR
      retval : integer;
      counter : integer;


FUNCTION setCompilerWorks(VAR value : integer ): integer;
BEGIN
   value := 8;
   setCompilerWorks := value
END ;

FUNCTION testFunctionCallClassByRef;
BEGIN
   counter := 9;
   PRINT counter;
   
   retval := setCompilerWorks(counter);
   
   PRINT counter
END

END
.
