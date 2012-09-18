program testArrayBoundsInvalid;

class testArrayBoundsInvalid

BEGIN
   
VAR
   bad	: ARRAY[4..9] OF integer; 
   good	: ARRAY[78..123] OF integer;

FUNCTION testArrayBoundsInvalid;
BEGIN
   bad[3] := 67;
   bad[4] := 67;
   bad[5] := 67;
   bad[9] := 67;
   bad[10] := 67;
   bad[6] := 70;
   good[99] := bad[5]
END

END
.
