program testAttributeDesignatorMultiThis;

class room

BEGIN
   VAR doors  : integer;
      windows : integer;
END	      


class house

BEGIN
   VAR users	 : integer;
      this	 : room;
      livingroom : room;
      garage	 : room;
END		 


class testAttributeDesignatorMultiThis

BEGIN
   
   VAR renters : integer;
       my      : house;
       yours   : house;

FUNCTION testAttributeDesignatorMultiThis;
BEGIN

   my.this.doors := yours.livingroom.doors
END

END
.
