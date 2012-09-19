program testMissingBaseClass;

class AA

BEGIN
   VAR row : integer;
END

class BB extends AAAA

BEGIN
   VAR column : integer;
END

class testMissingBaseClass extends BB

BEGIN
   
   VAR test: integer;

FUNCTION testMissingBaseClass;
BEGIN
   test := 8
END

END
.
