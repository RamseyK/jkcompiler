program testVariableDeclaredBaseClass;

class BB

BEGIN
   VAR row : integer;
END

class AA extends BB

BEGIN
   VAR col: integer;
END


class testVariableDeclaredBaseClass

BEGIN
   
   VAR test: AA;
       test2: BB;

FUNCTION testVariableDeclaredBaseClass;
BEGIN
   test := new AA;
   test2 := new BB
END

END
.
