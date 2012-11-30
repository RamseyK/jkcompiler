program testObjectAccess;

class testObject

BEGIN   
   VAR
      numberOfHours	      : integer;
END

class testObjectAccess

BEGIN
   VAR
      classPtr	: testObject;
      a1	: integer;

FUNCTION testObjectAccess;
BEGIN
   classPtr := new testObject;
   a1 := classPtr.numberOfHours
END

END
.