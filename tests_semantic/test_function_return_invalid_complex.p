program testFunctionReturnInvalidComplex;

class testFunctionReturnInvalidComplex

BEGIN
   VAR someVar : integer;
FUNCTION testFunctionReturnInvalidComplex: integer;
BEGIN
   someVar := 1 + testFunctionReturnInvalidComplex;
   testFunctionReturnInvalidComplex := 6
END

END
.
