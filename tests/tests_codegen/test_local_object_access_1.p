program testObjectAccess;

class testObject

BEGIN   
   VAR
      numberOfHours	      : integer;
END

class testObjectAccess

BEGIN

FUNCTION testObjectAccess;
VAR
      classPtr	: testObject;
      a1	: integer;
BEGIN
   classPtr := new testObject;
   classPtr.numberOfHours := 1337;
   print classPtr.numberOfHours
END

END
.