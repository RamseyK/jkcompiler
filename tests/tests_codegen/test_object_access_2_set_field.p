program testObjectAccess;

class testObject

BEGIN   
   VAR
      spacer		: integer;
      firstField	: testObjectTwo;
END

class testObjectTwo

BEGIN
   VAR
      secondField	: integer;
END

class testObjectAccess

BEGIN

FUNCTION testObjectAccess;
   VAR
      a1	: integer;
      a2	: integer;
      classPtr	: testObject;
BEGIN
   classPtr := new testObject;
   classPtr.firstField := new testObjectTwo;
   classPtr.firstField.secondField := 6
END

END
.