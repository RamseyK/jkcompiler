program testFunctionLocalVariableList;

class testFunctionLocalVariableList

BEGIN
   
VAR
   global: integer; 

FUNCTION testFunctionLocalVariableList;
VAR local  : integer;
   local2  : integer;
   counter : integer;
BEGIN
   global := 77;
   local := 88;
   local2 := 99;
   counter := 0
END

END
.
