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
   VAR
      a1	: integer;
      a2	: integer;
      classPtr	: testObject;
FUNCTION testObjectAccess;
BEGIN
   classPtr := new testObject;
   classPtr.firstField := new testObjectTwo;
   classPtr.firstField.secondField := 6;
   a1 := classPtr.firstField.secondField;
   a2 := a1 + 1;
   PRINT a2
END

END
.