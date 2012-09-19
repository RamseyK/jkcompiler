program testThisVariableDisambiguation;


class testThisVariableDisambiguation

BEGIN
   VAR
      someVar	: integer;


FUNCTION testThisVariableDisambiguation;
VAR someVar : integer;
BEGIN
   this.someVar := 7;
   someVar := 5;
   this.someVar := someVar;

   PRINT this.someVar
END

END
.
