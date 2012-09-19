program testClassEquivalenceInvalidTypes;

class BB

BEGIN
   VAR row     : integer;
      yourBoat : boolean;
      streams  : ARRAY[0 .. 5] OF boolean;
END	       


class AA

BEGIN
   VAR row     : integer;
      yourBoat : boolean;
      streams  : ARRAY[0 .. 5] OF integer;
END		  


class testClassEquivalenceInvalidTypes

BEGIN
   
   VAR objectA	: AA;
       objectB	: BB;

FUNCTION testClassEquivalenceInvalidTypes;
BEGIN
   objectA := objectB
END

END
.
