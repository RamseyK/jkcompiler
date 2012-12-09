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

FUNCTION testObjectTwoFunction: integer;
BEGIN
	testObjectTwoFunction := 2 * secondField
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
   classPtr.firstField.secondField := 1337;
   a1 := classPtr.firstField.testObjectTwoFunction;
   a2 := a1 + 1
END

END
.