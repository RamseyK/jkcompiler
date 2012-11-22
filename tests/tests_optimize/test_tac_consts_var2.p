program testConstsVar;

class testConstsVar

BEGIN
   VAR aa, bb, cc, dd, ee, ff, gg, xx, yy, zz: integer;

FUNCTION testConstsVar;
BEGIN
   xx := 0*aa;
   yy := xx+xx;
   zz := bb AND bb;
   cc := dd OR dd; 
   dd := ee-ee;
   ff := gg-0;
   ff := gg+0;
   ff := gg/1
END

END
.

