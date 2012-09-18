program testFunctionCallClassNoargs;

class testFunctionCallClassNoargs

BEGIN
   
   VAR
      compilerWorks : boolean;
      counter	    : integer;


FUNCTION setCompilerWorks;
BEGIN
   counter := counter + 1
END ;

FUNCTION testFunctionCallClassNoargs;
BEGIN
   counter := 0;
   setCompilerWorks;
   setCompilerWorks
END

END
.
