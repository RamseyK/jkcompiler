program testFunctionCallMulti;

class testFunctionCallMulti

BEGIN
   
   VAR
      compilerWorks : boolean;
      counter	    : integer;


FUNCTION setCompilerWorks(value1: integer; value2: integer ): integer;
BEGIN
   counter := value1 + value2;
   setCompilerWorks := 5
END ;

FUNCTION testFunctionCallMulti;
BEGIN
   compilerWorks := setCompilerWorks(2,8)
END

END
.
