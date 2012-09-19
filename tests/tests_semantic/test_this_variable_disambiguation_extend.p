program testThisVariableDisambiguationExtend;

class baseClass

BEGIN
   VAR
      someVar	: integer;

END

class testThisVariableDisambiguationExtend extends baseClass

BEGIN

FUNCTION testThisVariableDisambiguationExtend;
VAR someVar : integer;
BEGIN
   this.someVar := 7;
   someVar := 5;

   PRINT this.someVar
END

END
.
