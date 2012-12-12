program testObjectAccess;

class testObject

BEGIN   
   VAR
      compilerIs      : integer;
END

class testObjectAccess

BEGIN

FUNCTION testObjectAccess;
VAR
      classPtr	: testObject;
      a1	: integer;
BEGIN
   a1 := 69;
   a1 := a1 + 182;
   a1 := a1 + 420;
   a1 := a1 + 666;
   classPtr := new testObject;
   classPtr.compilerIs := a1;
   print classPtr.compilerIs
END

END
.