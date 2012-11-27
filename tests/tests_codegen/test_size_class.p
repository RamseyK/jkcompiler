program testSizeClassPointer;

class testSizePrimitives

BEGIN   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;
      numberOfLines	      : integer;
      numberOfHours	      : integer;
      timeToWrite	      : ARRAY[4..70] OF integer;
      timeToTest	      : ARRAY[9..12] OF integer;
END

class testSizeClassPointer

BEGIN
   VAR
      classPtr	: testSizePrimitives;
      classArr : ARRAY[0..8] OF testSizePrimitives;

FUNCTION testSizeClassPointer;
BEGIN
   classPtr := new testSizePrimitives
END

END
.