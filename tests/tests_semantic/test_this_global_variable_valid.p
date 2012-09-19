program testThisGlobalVariableValid;


class testThisGlobalVariableValid

BEGIN
   VAR
      this : integer;


FUNCTION testThisGlobalVariableValid;
VAR someVar : integer;
BEGIN
   this.this := 6;

   PRINT this.this
END

END
.
