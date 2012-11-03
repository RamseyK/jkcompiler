program testConsts;

class testConsts

BEGIN
   VAR aa, bb, cc, dd, ee, ff, gg : integer;

FUNCTION testConsts;
BEGIN
   aa := 10;
   bb := aa / 2;
   aa := 12;
   cc := aa / 6;
   dd := aa + cc;
   ee := 1;
   ff := ee;
   gg := ff;
END

END
.

