program testIndexedVariableComplex;

class room

BEGIN
   VAR doors  : integer;
      windows : integer;
END	      


class house

BEGIN
   VAR users	 : integer;
      rooms	 : ARRAY[1..2] OF room;
END		 


class testIndexedVariableComplex

BEGIN
   
   VAR renters : integer;
       my      : house;
       yours   : house;

FUNCTION testIndexedVariableComplex;
BEGIN
   yours.rooms[1].doors := my.rooms[2].doors
END

END
.
