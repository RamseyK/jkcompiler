program testArrayMultiBoundsInvalid;

class testArrayMultiBoundsInvalid

BEGIN
   
VAR
   bad	: ARRAY[4..9] OF ARRAY[28..38] OF integer;

FUNCTION testArrayMultiBoundsInvalid;
BEGIN
   PRINT bad[5,28];
   bad[5,27] := 67;
   PRINT bad[3,39]

END

END
.
