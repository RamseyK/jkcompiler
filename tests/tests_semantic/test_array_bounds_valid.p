program testArrayBoundsValid;

class testArrayBoundsValid

BEGIN
   
VAR
   bad	: ARRAY[4..9] OF integer; 
   good	: ARRAY[78..123] OF integer;

FUNCTION testArrayBoundsValid;
BEGIN
   bad[5] := 67;
   bad[6] := 70;
   good[99] := bad[9]
END

END
.
