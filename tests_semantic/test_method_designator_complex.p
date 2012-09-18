program testMethodDesignatorComplex;

class main

BEGIN
   VAR value : integer;
END			  


class testMethodDesignatorComplex

BEGIN
   VAR object	 : main;
      newObject	 : main;
      dummyValue : integer;
   
FUNCTION initMainObject(value :integer): main;
BEGIN
   newObject := new main;
   newObject.value := value;
   
   initMainObject := newObject
END;

FUNCTION testMethodDesignatorComplex;
BEGIN
   object := initMainObject(5);

   PRINT object.value
END

END
.

