program testArrayMultiBoundsValid;

class testArrayMultiBoundsValid

BEGIN
   
VAR
   bad	: ARRAY[4..9] OF ARRAY[28..38] OF integer;

FUNCTION testArrayMultiBoundsValid;
BEGIN
   PRINT bad[5,28];
   bad[5,28] := 67;
   PRINT bad[5,28]
END

END
.
