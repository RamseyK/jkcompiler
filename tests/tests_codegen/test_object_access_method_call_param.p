program testObjectAccessMethodCall;

class testObject

BEGIN   
   VAR
      firstField	: testObjectTwo;
END

class testObjectTwo

BEGIN
   VAR
      secondField	: integer;

FUNCTION testObjectTwoFunction(par1 : integer): integer;
BEGIN
	testObjectTwoFunction := 2 * par1
END
END

class testObjectAccessMethodCall

BEGIN
   VAR
      classPtr	: testObject;
      a1	: integer;
      a2	: integer;

FUNCTION testObjectAccessMethodCall;
BEGIN
   classPtr := new testObject;
   classPtr.firstField := new testObjectTwo;
   a1 := classPtr.firstField.testObjectTwoFunction(1337);
   a2 := a1 + 1
END

END
.