program testClassEquivalenceInvalidOrder;

class BB

BEGIN
   VAR row     : integer;
      yourBoat : boolean;
      streams  : ARRAY[0 .. 5] OF boolean;
END	       


class AA

BEGIN
   VAR elite	  : boolean;
      number	  : integer;
      pfChangsMarathonsWon : ARRAY[4 .. 9] OF boolean;
END		  


class testClassEquivalenceInvalidOrder

BEGIN
   
   VAR objectA	: AA;
       objectB	: BB;

FUNCTION testClassEquivalenceInvalidOrder;
BEGIN
   objectA := objectB
END

END
.
