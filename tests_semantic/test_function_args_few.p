program testFunctionCallObject;


class testFunctionCallObject

BEGIN
   
   VAR
      retval  : integer;
      counter : integer;

FUNCTION test(value1 : integer; value2: integer): integer;
BEGIN
   counter := value2;
   test := 3
END;

FUNCTION testFunctionCallObject;
BEGIN
   retval := test(4)
END

END
.
