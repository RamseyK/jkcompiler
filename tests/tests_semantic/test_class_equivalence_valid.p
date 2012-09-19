program testClassEquivalenceValid;

class BB

BEGIN
   VAR row     : integer;
      yourBoat : boolean;
      streams  : ARRAY[0 .. 5] OF boolean;
END	       


class AA

BEGIN
   VAR number	   : integer;
       elite	   : boolean;
       ifSixWasNine : ARRAY[4 .. 9] OF boolean;
END		   


class testClassEquivalenceValid

BEGIN
   
   VAR objectA	: AA;
       objectB	: BB;

FUNCTION testClassEquivalenceValid;
BEGIN
   objectA := objectB
END

END
.
