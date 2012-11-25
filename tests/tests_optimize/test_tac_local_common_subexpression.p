program testLce;

class testLce

BEGIN
   VAR aa, bb, cc, dd: integer;

FUNCTION testLce;
BEGIN   
   aa := cc + 1;
   dd := cc + 1;
   cc := 5;
   dd := cc + 1;
   cc := 7;

   if aa = 0 THEN
      bb := cc + 1
   ELSE
      bb := 0

END
   
END   
.

