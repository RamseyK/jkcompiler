program testThisVariableNotDeclared;


class testThisVariableNotDeclared

BEGIN
   VAR
      someOtherVar : integer;


FUNCTION testThisVariableNotDeclared;
VAR someVar : integer;
BEGIN
   this.someVar := 7;
   someVar := 5;
   this.someVar := someVar;

   PRINT this.someVar
END

END
.
