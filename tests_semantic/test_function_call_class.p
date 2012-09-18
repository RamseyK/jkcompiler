program testFunctionCallClass;

class testFunctionCallClass

BEGIN
   
   VAR
      compilerWorks : boolean;
      counter	    : integer;


FUNCTION setCompilerWorks(value	: integer ): integer;
BEGIN
   counter := value;
   setCompilerWorks := 5
END ;

FUNCTION testFunctionCallClass;
BEGIN
   compilerWorks := setCompilerWorks(8)
END

END
.
