program testObjectAccess;

class testObject

BEGIN   
   VAR
      firstField	: testObjectTwo;
END

class testObjectTwo

BEGIN
   VAR
      secondField	: integer;
END

class testObjectAccess

BEGIN
   VAR
      classPtr	: testObject;
      a1	: testObjectTwo;
      a2	: integer;

FUNCTION testObjectAccess;
BEGIN
   classPtr := new testObject;
   a1 := classPtr.firstField;
   a2 := a1.secondField;
   a2 := a1.secondField + 1
END

END
.