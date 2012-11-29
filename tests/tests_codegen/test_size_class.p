program testSizeClassPointer;

class testSizePrimitives

BEGIN   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;
      numberOfLines	      : integer;
      numberOfHours	      : integer;
END

class testSizeClassPointer

BEGIN
   VAR
      classPtr	: testSizePrimitives;

FUNCTION testSizeClassPointer;
BEGIN
   classPtr := new testSizePrimitives
END

END
.