program testLiveVars;

class testLiveVars

BEGIN
   VAR aa, bb, cc: integer;

FUNCTION testLiveVars;
BEGIN   
   aa := 0;
   cc := 0;

   if aa = 0 THEN
      cc := 1
   ELSE
      bb := 0
   ;
   cc := 2;
   cc := 3

END
   
END   
.

