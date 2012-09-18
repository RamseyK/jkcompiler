program testVariableDeclaredGlobalExtend;

class baseClass

BEGIN
   VAR
      someVar	: integer;

END

class testVariableDeclaredGlobalExtend extends baseClass

BEGIN

FUNCTION testVariableDeclaredGlobalExtend;
VAR someVar : integer;
BEGIN
   someVar := 5;
   PRINT someVar
END

END
.
