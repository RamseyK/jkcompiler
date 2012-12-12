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
   classPtr.numberOfHours := 1337;
   print classPtr.numberOfHours
END

END
.