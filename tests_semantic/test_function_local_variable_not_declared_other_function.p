program testFunctionLocalVariableNotDeclaredOtherFunction;


class testFunctionLocalVariableNotDeclaredOtherFunction

BEGIN
   VAR otherVar	: integer;
      

FUNCTION testFunctionLocalVariableNotDeclaredOtherFunction(value : integer): integer;
VAR
   someVar : integer;
BEGIN
   someVar := 6;
   testFunctionLocalVariableNotDeclaredOtherFunction := 2
END;

FUNCTION testFunctionLocalVariableNotDeclaredOtherFunction;
BEGIN
   someVar := 5
END

END
.
