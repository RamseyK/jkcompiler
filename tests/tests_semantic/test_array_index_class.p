program testArrayIndexClass;

class someClass

BEGIN
   VAR irrelevant : integer;
END		  

class testArrayIndexClass

BEGIN

   VAR 
      rows   : ARRAY[0 .. 9] OF boolean;
      tests  : boolean;
      object : someClass;

FUNCTION testArrayIndexClass;
BEGIN	   
   tests := rows[object]
END

END
.

