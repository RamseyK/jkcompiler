program testFunctionNotDeclared;

class testFunctionNotDeclared

BEGIN
   
   VAR
      compilerWorks	      : integer;

FUNCTION setCompilerWorks;
BEGIN
   compilerWorks := 5
END ;

FUNCTION testFunctionNotDeclared;
BEGIN
   compilerWorks := setLightSwitchWorks(0)
END

END
.
