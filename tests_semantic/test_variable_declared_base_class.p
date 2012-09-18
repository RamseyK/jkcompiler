program testVariableDeclaredBaseClass;

class BB

BEGIN
   VAR row : integer;
END

class AA extends BB

BEGIN
   VAR row: integer;
END


class testVariableDeclaredBaseClass

BEGIN
   
   VAR test: integer;

FUNCTION testVariableDeclaredBaseClass;
BEGIN
   test := 129
END

END
.
